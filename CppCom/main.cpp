#include <Windows.h>
#include "ComModule.h"
#include "ComObjectMap.h"
#include "FooNative.h"
#include "FooWrapper.h"
#include "CppCom_i.c"

extern "C" BOOL __stdcall DllMain(HINSTANCE instance, DWORD reason, void* reserved)
{
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
		FooWrapper
	>::Create(rclsid, riid, ppv);
}
