// Copyright (C) Dreamer's Tail

#include "GamePulseBlueprintLibrary.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"

THIRD_PARTY_INCLUDES_START
#include "Windows/AllowWindowsPlatformTypes.h"
#include "dxgi1_4.h"
#include <windows.h>
#include <psapi.h>
#include "Windows/HideWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_END
#endif

#if PLATFORM_WINDOWS
static size_t _GetUsedVideoMemory()
{
	IDXGIFactory4* PFactory;
	CreateDXGIFactory1(
		__uuidof(IDXGIFactory4),
		reinterpret_cast<void**>(&PFactory));

	IDXGIAdapter3* Adapter;
	PFactory->EnumAdapters(
		0, reinterpret_cast<IDXGIAdapter**>(&Adapter));

	DXGI_QUERY_VIDEO_MEMORY_INFO VideoMemoryInfo;
	Adapter->QueryVideoMemoryInfo(
		0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &VideoMemoryInfo);

	const size_t UsedVRAM =
		VideoMemoryInfo.CurrentUsage / 1024 / 1024;

	return UsedVRAM;
}


static size_t _GetUsedMemory()
{
	// Create a MEMORYSTATUSEX structure to hold memory information
	MEMORYSTATUSEX MemoryStatus;
	MemoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

	// Call GlobalMemoryStatusEx to fill the structure with memory information
	if (GlobalMemoryStatusEx(&MemoryStatus))
	{
		// Calculate used RAM
		const size_t UsedRAM =
			MemoryStatus.ullTotalPhys -
			MemoryStatus.ullAvailPhys;
		return UsedRAM / 1024 / 1024; // Convert bytes to megabytes
	}

	return 0;
}


size_t _GetProcessMemoryUsage(const DWORD ProcessID)
{
	const HANDLE hProcess = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, Windows::FALSE, ProcessID);

	if (hProcess == NULL)
	{
		return 0;
	}

	PROCESS_MEMORY_COUNTERS Pmc;
	if (GetProcessMemoryInfo(hProcess, &Pmc, sizeof(Pmc)))
	{
		CloseHandle(hProcess);
		return Pmc.WorkingSetSize / 1024 / 1024; // Convert to MB
	}

	CloseHandle(hProcess);
	return 0;
}
#endif


float UGamePulseBlueprintLibrary::GetUsedVideoMemory()
{
#if PLATFORM_WINDOWS
	return _GetUsedVideoMemory();
#else
	return 0.f;
#endif
}

float UGamePulseBlueprintLibrary::GetUsedMemory()
{
#if PLATFORM_WINDOWS
	return _GetUsedMemory();
#else
	return 0.f;
#endif
}

float UGamePulseBlueprintLibrary::GetProcessMemoryUsage()
{
#if PLATFORM_WINDOWS
	const DWORD UnrealProcessID =
		FPlatformProcess::GetCurrentProcessId();
	return _GetProcessMemoryUsage(UnrealProcessID);
#else
	return 0.f;
#endif
}


#if PLATFORM_WINDOWS
/* https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process */

CHAR GetCPUUsage()
{
	FILETIME               FT_Sys_Idle;
	FILETIME               FT_Sys_Kernel;
	FILETIME               FT_Sys_User;

	ULARGE_INTEGER         Ul_Sys_Idle;
	ULARGE_INTEGER         Ul_Sys_Kernel;
	ULARGE_INTEGER         Ul_Sys_User;

	static ULARGE_INTEGER  Ul_Sys_Idle_Old;
	static ULARGE_INTEGER  Ul_Sys_Kernel_Old;
	static ULARGE_INTEGER  Ul_Sys_User_Old;

	GetSystemTimes(
		&FT_Sys_Idle,    /* System idle time */
		&FT_Sys_Kernel,  /* system kernel time */
		&FT_Sys_User);   /* System user time */

	CopyMemory(&Ul_Sys_Idle  , &FT_Sys_Idle  , sizeof(FILETIME)); // Could been optimized away...
	CopyMemory(&Ul_Sys_Kernel, &FT_Sys_Kernel, sizeof(FILETIME)); // Could been optimized away...
	CopyMemory(&Ul_Sys_User  , &FT_Sys_User  , sizeof(FILETIME)); // Could been optimized away...

	const CHAR Usage =
		(Ul_Sys_Kernel.QuadPart - Ul_Sys_Kernel_Old.QuadPart +
		(Ul_Sys_User.QuadPart - Ul_Sys_User_Old.QuadPart) -
		(Ul_Sys_Idle.QuadPart - Ul_Sys_Idle_Old.QuadPart)) * 100 /
		(Ul_Sys_Kernel.QuadPart - Ul_Sys_Kernel_Old.QuadPart +
		(Ul_Sys_User.QuadPart - Ul_Sys_User_Old.QuadPart));

	Ul_Sys_Idle_Old.QuadPart   = Ul_Sys_Idle.QuadPart;
	Ul_Sys_User_Old.QuadPart   = Ul_Sys_User.QuadPart;
	Ul_Sys_Kernel_Old.QuadPart = Ul_Sys_Kernel.QuadPart;

	return Usage;
}
#endif


double UGamePulseBlueprintLibrary::GetCPULoad()
{
#if PLATFORM_WINDOWS
	return GetCPUUsage();
#else
	return 0.f;
#endif
}


#if PLATFORM_WINDOWS
// This code is from http://eliang.blogspot.de/2011/05/getting-nvidia-gpu-usage-in-c.html

// magic numbers, do not change them
#define NVAPI_MAX_PHYSICAL_GPUS   64
#define NVAPI_MAX_USAGES_PER_GPU  34

// function pointer types
typedef int *(*NvAPI_QueryInterface_t)(unsigned int Offset);
typedef int(*NvAPI_Initialize_t)();
typedef int(*NvAPI_EnumPhysicalGPUs_t)(int **Handles, int *Count);
typedef int(*NvAPI_GPU_GetUsages_t)(int *Handle, unsigned int *Usages);

// magic numbers, do not change them
#define NVAPI_MAX_PHYSICAL_GPUS   64
#define NVAPI_MAX_USAGES_PER_GPU  34

// function pointer types
typedef int *(*NvAPI_QueryInterface_t)(unsigned int Offset);
typedef int (*NvAPI_Initialize_t)();
typedef int (*NvAPI_EnumPhysicalGPUs_t)(int **Handles, int *Count);
typedef int (*NvAPI_GPU_GetUsages_t)(int *Handle, unsigned int *Usages);

int GetGPUUsage()
{
	const HMODULE Hmod = LoadLibraryA("nvapi64.dll");

	if (Hmod == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("LoadLibraryA failed"));
		return 1;
	}

	// nvapi.dll internal function pointers
	NvAPI_QueryInterface_t   NvAPI_QueryInterface = nullptr;
	NvAPI_Initialize_t       NvAPI_Initialize = nullptr;
	NvAPI_EnumPhysicalGPUs_t NvAPI_EnumPhysicalGPUs = nullptr;
	NvAPI_GPU_GetUsages_t    NvAPI_GPU_GetUsages = nullptr;

	// nvapi_QueryInterface is a function used to retrieve other internal functions in nvapi.dll
	NvAPI_QueryInterface = reinterpret_cast<NvAPI_QueryInterface_t>(reinterpret_cast<void*>(
		GetProcAddress(Hmod, "nvapi_QueryInterface")));

	// some useful internal functions that aren't exported by nvapi.dll
	NvAPI_Initialize = reinterpret_cast<NvAPI_Initialize_t>(
		(*NvAPI_QueryInterface)(0x0150E828));
	NvAPI_EnumPhysicalGPUs = reinterpret_cast<NvAPI_EnumPhysicalGPUs_t>(
		(*NvAPI_QueryInterface)(0xE5AC921F));
	NvAPI_GPU_GetUsages = reinterpret_cast<NvAPI_GPU_GetUsages_t>(
		(*NvAPI_QueryInterface)(0x189A1FDF));

	if (NvAPI_Initialize == nullptr ||
		NvAPI_EnumPhysicalGPUs == nullptr ||
		NvAPI_EnumPhysicalGPUs == nullptr ||
		NvAPI_GPU_GetUsages == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't get functions in nvapi64.dll"));
		return 2;
	}

	// initialize NvAPI library, call it once before calling any other NvAPI functions
	(*NvAPI_Initialize)();

	int          GPUCount = 0;
	int         *GPUHandles[NVAPI_MAX_PHYSICAL_GPUS] = {nullptr};
	unsigned int GPUUsages[NVAPI_MAX_USAGES_PER_GPU] = { 0 };

	// gpuUsages[0] must be this value, otherwise NvAPI_GPU_GetUsages won't work
	GPUUsages[0] = (NVAPI_MAX_USAGES_PER_GPU * 4) | 0x10000;

	(*NvAPI_EnumPhysicalGPUs)(GPUHandles, &GPUCount);

	(*NvAPI_GPU_GetUsages)(GPUHandles[0], GPUUsages);
	const int Usage = GPUUsages[3];

	return Usage;
}
#endif


int UGamePulseBlueprintLibrary::GetGPULoad()
{
#if PLATFORM_WINDOWS
	return GetGPUUsage();
#else
	return 0.f;
#endif
}
