#include <Windows.h>
#include "ComModule.h"
#include "ComObjectMap.h"
#include "FooNative.h"
#include "FooWrapper.h"
#include "FooLate.h"
#include "FooBoth.h"

extern "C" BOOL __stdcall DllMain(HINSTANCE instance, DWORD reason, void* reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
		ComModule::GetInstance().Initialize(instance);
	return TRUE;
}

HRESULT __stdcall DllCanUnloadNow()
{
	return ComModule::GetInstance().CanUnload() ? S_OK : S_FALSE;
}

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	return ComObjectMap<
		FooNative,
		FooWrapper,
		FooLate,
		FooBoth
	>::Create(rclsid, riid, ppv);
}
