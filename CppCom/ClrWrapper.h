#pragma once
#include <Com/Com.h>
#include "IClrFactory.h"
#import <mscorlib.tlb> \
	raw_interfaces_only \
	named_guids \
	rename("ReportEvent", "ReportEventEx") \
	rename("IID_IObjectHandle", "IID_IObjectHandleEx")
#include <mscoree.h>
#pragma comment(lib, "mscoree.lib")
#include <atomic>

namespace Com
{
	template <typename Type, const CLSID* Clsid>
	class ClrWrapper : public IUnknown
	{
	private:
		std::atomic<int> referenceCount = 0;
		IUnknown* inner = nullptr;
		mscorlib::IDisposable* disposable = nullptr;

	public:
		ClrWrapper()
		{
			Module::GetInstance().AddRef();
		}
		virtual ~ClrWrapper()
		{
			if (disposable)
				disposable->Dispose();
			if (inner)
				inner->Release();
			Module::GetInstance().Release();
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
			auto hr = RunAction([&](){ object->CreateInner(); });
			if (SUCCEEDED(hr))
				hr = object->QueryInterface(riid, ppvObject);
			object->Release();
			return hr;
		}

	private:
		void CreateInner()
		{
#pragma warning(push)
#pragma warning(disable:4996) //Deprecated functions
			Pointer<ICorRuntimeHost> runtime;
			auto hr = ::CorBindToRuntimeEx(
				L"v4.0.30319",
				L"svr",
				STARTUP_SERVER_GC,
				CLSID_CorRuntimeHost,
				IID_ICorRuntimeHost,
				reinterpret_cast<void**>(&runtime));
			CheckError(hr, __FUNCTION__, "CorBindToRuntimeEx");

			Pointer<IClrFactory> factory;
			hr = ::ClrCreateManagedInstance(
				Type::GetClrFactoryTypeName(),
				__uuidof(IClrFactory),
				reinterpret_cast<void**>(&factory));
			CheckError(hr, __FUNCTION__, "ClrCreateManagedInstance(ClrFactory)");
#pragma warning(pop)

			hr = factory->Create(Type::GetClrClassTypeName(), this, &inner);
			CheckError(hr, __FUNCTION__, "ClrFactory.CreateInstance");

			inner->QueryInterface(&disposable);
			if (disposable)
				disposable->Release();
		}
	};
}

#define DECLARE_CLR_WRAPPER(library, name) \
	class name : public Com::ClrWrapper<name, &CLSID_##name> \
	{ \
	public: \
		static const wchar_t* GetClrFactoryTypeName() { return L###library L".ClrFactory, " L###library; } \
		static const char* GetClrClassTypeName() { return #library "." #name ", " #library; } \
	}
