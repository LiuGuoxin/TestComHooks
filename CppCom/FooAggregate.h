#pragma once
#include <Com/Com.h>
#include "CppCom.h"
#include <atomic>

#if defined(ReportEvent)
#undef ReportEvent
#endif
#import <mscorlib.tlb> raw_interfaces_only named_guids

#define IID_IObjectHandle IID_IObjectHandleFuckYouDuplicateComDat
#include "mscoree.h"
#pragma comment(lib, "mscoree.lib")

inline void CheckWindowsError(bool isError, const char* function, const char* location)
{
	if (!isError)
		return;
	auto errorCode = ::GetLastError();
	auto description = Com::Error::GetWindowsErrorDescription(errorCode);
	throw Com::Error{ static_cast<HRESULT>(errorCode), function, location, description };
}

class Library
{
private:
	HMODULE module = nullptr;

public:
	Library(const std::string& name)
	{
		module = ::LoadLibraryA(name.c_str());
		CheckWindowsError(module == nullptr, __FUNCTION__, "LoadLibrary");
	}
	~Library()
	{
		if (module)
			::FreeLibrary(module);
	}

	template <typename Function>
	Function GetFunction(const std::string& name) const
	{
		auto address = ::GetProcAddress(module, name.c_str());
		CheckWindowsError(address == nullptr, __FUNCTION__, "GetProcAddress");
		return reinterpret_cast<Function>(address);
	}
};

class ClrLibrary : public Library
{
private:
	HRESULT(__stdcall* dllGetClassObject)(REFCLSID, REFIID, void**);

public:
	ClrLibrary() : Library("mscoree.dll")
	{
		dllGetClassObject = GetFunction<decltype(dllGetClassObject)>("DllGetClassObject");
	}

	HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppvObject) const
	{
		return dllGetClassObject(rclsid, riid, ppvObject);
	}

	void CreateInstance(REFCLSID rclsid, IUnknown* outer, REFIID riid, void** ppvObject) const
	{
		Com::Pointer<IClassFactory> factory;
		auto hr = DllGetClassObject(rclsid, IID_IClassFactory, reinterpret_cast<void**>(&factory));
		Com::CheckError(hr, __FUNCTION__, "DllGetClassObject");
		hr = factory->CreateInstance(outer, riid, ppvObject);
		Com::CheckError(hr, __FUNCTION__, "factory->CreateInstance");
	}
};

template <typename Type, const CLSID* Clsid>
class FooAggregateBase : public IUnknown
{
private:
	std::atomic<int> referenceCount = 0;

protected:
	IUnknown* inner = nullptr;
	mscorlib::IDisposable* disposable = nullptr;

public:
	FooAggregateBase()
	{
		Com::Module::GetInstance().AddRef();
	}
	virtual ~FooAggregateBase()
	{
		if (disposable)
			disposable->Dispose();
		if (inner)
			inner->Release();
		Com::Module::GetInstance().Release();
	}

	HRESULT OnFinalConstruct()
	{
		return Com::RunAction([&](){ CreateInner(); });
	}

	virtual void CreateInner() = 0;

	static bool Is(REFCLSID rclsid)
	{
		return rclsid == *Clsid;
	}

	static HRESULT TryCreateInstance(REFIID riid, void** ppvObject)
	{
		if (ppvObject == nullptr)
			return E_POINTER;
		auto object = new Type();
		object->AddRef();
		auto hr = object->OnFinalConstruct();
		if (SUCCEEDED(hr))
			hr = object->QueryInterface(riid, ppvObject);
		object->Release();
		return hr;
	}

	HRESULT __stdcall QueryInterface(REFIID riid, void** ppv)
	{
		if (ppv == nullptr)
			return E_POINTER;
		if (riid == IID_IUnknown)
		{
			*ppv = static_cast<IUnknown*>(this);
			++referenceCount;
			return S_OK;
		}
		if (inner == nullptr)
			return E_NOINTERFACE;
		return inner->QueryInterface(riid, ppv);
	}

	ULONG __stdcall AddRef()
	{
		return ++referenceCount;
	}

	ULONG __stdcall Release()
	{
		auto result = --referenceCount;
		if (result == 0)
			delete this;
		return result;
	}
};

class FooAggregate1 : public FooAggregateBase<FooAggregate1, &CLSID_FooAggregate>
{
public:
	//This fails because CsCom has not been loaded (or there is no known way to tie the CLSID to CsCom.FooAggregate).
	void CreateInner() final
	{
		ClrLibrary clrLibrary;
		Com::Pointer<IClassFactory> factory;
		auto hr = clrLibrary.DllGetClassObject(CLSID_FooAggregate, IID_IClassFactory, reinterpret_cast<void**>(&factory));
		Com::CheckError(hr, __FUNCTION__, "clrLibrary.DllGetClassObject");
		hr = factory->CreateInstance(this, IID_IUnknown, reinterpret_cast<void**>(&inner));
		Com::CheckError(hr, __FUNCTION__, "factory->CreateInstance");
		inner->QueryInterface(&disposable);
		if (disposable)
			disposable->Release();
	}
};

class FooAggregate2 : public FooAggregateBase<FooAggregate2, &CLSID_FooAggregate>
{
public:
	//This works, but it means the inner and outer clsids must be different
	void CreateInner() final
	{
		const GUID CLSID_Foo = { 0xd0ad54ba,0x7405,0x49ee,{ 0xb3,0x80,0x49,0xd6,0xe0,0xd5,0x1f,0x1c } };
		auto hr = ::CoCreateInstance(CLSID_Foo, this, CLSCTX_INPROC_SERVER, IID_IUnknown, reinterpret_cast<void**>(&inner));
		Com::CheckError(hr, __FUNCTION__, "CoCreateInstance(CLSID_Foo)");
		inner->QueryInterface(&disposable);
		if (disposable)
			disposable->Release();
	}
};

class FooAggregate3 : public FooAggregateBase<FooAggregate3, &CLSID_FooAggregate>
{
public:
	//This performs the final release before the ISupportErrorInfo interface is released.
	//Not sure if this is a bug in my code, a "feature", or aggressive garbage collection.
	//It also calls 2 deprecated CLR functions that say to create via CoCreateInstance.
	void CreateInner() final
	{
#pragma warning(push)
#pragma warning(disable:4996) //Deprecated functions
		CComPtr<ICorRuntimeHost> runtime;
		auto hr = ::CorBindToRuntimeEx(L"v4.0.30319", L"svr", STARTUP_SERVER_GC, CLSID_CorRuntimeHost, IID_ICorRuntimeHost, reinterpret_cast<void**>(&runtime));
		Com::CheckError(hr, __FUNCTION__, "CorBindToRuntimeEx");

		hr = ::ClrCreateManagedInstance(L"CsCom.FooAggregate, CsCom", IID_IUnknown, reinterpret_cast<void**>(&inner));
		Com::CheckError(hr, __FUNCTION__, "ClrCreateManagedInstance");

		inner->QueryInterface(&disposable);
		if (disposable)
			disposable->Release();
#pragma warning(pop)
	}
};
