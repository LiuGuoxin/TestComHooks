#pragma once
#include <objbase.h>
#include <atlbase.h>
#include "ComModule.h"

template <typename Interfaces, bool ObjectSupportsDispatch = Interfaces::ObjectSupportsDispatch>
class DispatchObject : public Interfaces
{
private:
	CComPtr<ITypeInfo> typeInfo;

	HRESULT LoadTypeInfo()
	{
		if (typeInfo != nullptr)
			return S_OK;
		CComPtr<ITypeLib> typeLibrary;
		auto hr = ComModule::GetInstance().LoadTypeLibrary(&typeLibrary);
		if (FAILED(hr))
			return hr;
		return typeLibrary->GetTypeInfoOfGuid(__uuidof(DispatchInterface), &typeInfo);
	}

protected:
	void* GetDispatchInterface()
	{
		return static_cast<IDispatch*>(static_cast<DispatchInterface*>(this));
	}

public:
	HRESULT __stdcall GetTypeInfoCount(
		UINT* pctinfo) final
	{
		if (pctinfo == nullptr)
			return E_POINTER;
		*pctinfo = 1;
		return S_OK;
	}

	HRESULT __stdcall GetTypeInfo(
		UINT iTInfo,
		LCID lcid,
		ITypeInfo** ppTInfo) final
	{
		if (ppTInfo == nullptr)
			return E_POINTER;
		if (iTInfo != 0)
			return DISP_E_BADINDEX;
		auto hr = LoadTypeInfo();
		if (FAILED(hr))
			return hr;
		return typeInfo->QueryInterface(ppTInfo);
	}

	HRESULT __stdcall GetIDsOfNames(
		REFIID riid,
		LPOLESTR* rgszNames,
		UINT cNames,
		LCID lcid,
		DISPID* rgDispId) final
	{
		auto hr = LoadTypeInfo();
		if (FAILED(hr))
			return hr;
		return typeInfo->GetIDsOfNames(rgszNames, cNames, rgDispId);
	}

	HRESULT __stdcall Invoke(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS* pDispParams,
		VARIANT* pVarResult,
		EXCEPINFO* pExcepInfo,
		UINT* puArgErr) final
	{
		auto hr = LoadTypeInfo();
		if (FAILED(hr))
			return hr;
		return typeInfo->Invoke(
			GetDispatchInterface(),
			dispIdMember,
			wFlags,
			pDispParams,
			pVarResult,
			pExcepInfo,
			puArgErr);
	}
};

template <typename Interfaces>
class DispatchObject<Interfaces, false> : public Interfaces
{
protected:
	void* GetDispatchInterface()
	{
		return nullptr;
	}
};
