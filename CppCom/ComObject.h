#pragma once
#include <objbase.h>
#include <atomic>
#include "ComModule.h"
#include "DispatchObject.h"
#include "ComInterfaces.h"

template <typename Type, typename Interfaces, const CLSID* Clsid = nullptr>
class ComObject : public DispatchObject<Interfaces>, public ISupportErrorInfo
{
protected:
	std::atomic<int> referenceCount = 0;

public:
	ComObject()
	{
		ComModule::GetInstance().AddRef();
	}
	ComObject(const ComObject<Type, Interfaces, Clsid>& rhs) = delete;
	virtual ~ComObject()
	{
		ComModule::GetInstance().Release();
	}

	ComObject<Type, Interfaces, Clsid>& operator=(const ComObject<Type, Interfaces, Clsid>& rhs) = delete;

	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) final
	{
		if (ppvObject == nullptr)
			return E_POINTER;
		if (riid == IID_IUnknown)
			*ppvObject = static_cast<IUnknown*>(static_cast<PrimaryInterface*>(this));
		else if (ObjectSupportsDispatch && riid == IID_IDispatch)
			*ppvObject = GetDispatchInterface();
		else if (riid == IID_ISupportErrorInfo)
			*ppvObject = static_cast<ISupportErrorInfo*>(this);
		else if (!QueryInterfaceInternal(riid, ppvObject))
			return E_NOINTERFACE;
		++referenceCount;
		return S_OK;
	}

	ULONG __stdcall AddRef() final
	{
		return ++referenceCount;
	}

	ULONG __stdcall Release() final
	{
		auto result = --referenceCount;
		if (result == 0)
		{
			OnFinalRelease();
			delete this;
		}
		return result;
	}

	HRESULT __stdcall InterfaceSupportsErrorInfo(REFIID riid)
	{
		return S_OK;
	}

	virtual HRESULT OnFinalConstruct()
	{
		return S_OK;
	}

	virtual void OnFinalRelease()
	{
	}

	static HRESULT TryCreateInstance(REFIID riid, void** ppvObject)
	{
		if (ppvObject == nullptr)
			return E_POINTER;
		auto object = new Type();
		auto hr = object->OnFinalConstruct();
		if (SUCCEEDED(hr))
			hr = object->QueryInterface(riid, ppvObject);
		if (FAILED(hr))
			delete object;
		return hr;
	}

	static bool Is(REFCLSID rclsid)
	{
		return Clsid != nullptr && rclsid == *Clsid;
	}
};
