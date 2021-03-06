#include <Windows.h>
#include <Com/Com.h>
#include "FooNative.h"
#include "FooWrapper.h"
#include "FooLate.h"
#include "FooBoth.h"
#include "CppCom.h"
#include "ClrWrapper.h"

DECLARE_CLR_WRAPPER(CsCom, FooAggregate);

extern "C" BOOL __stdcall DllMain(HINSTANCE instance, DWORD reason, void* reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
		Com::Module::GetInstance().Initialize(instance);
	return TRUE;
}

HRESULT __stdcall DllCanUnloadNow()
{
	return Com::Module::GetInstance().CanUnload() ? S_OK : S_FALSE;
}

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	return Com::ObjectList<
		FooNative,
		FooWrapper,
		FooLate,
		FooBoth,
		FooAggregate
	>::Create(rclsid, riid, ppv);
}
