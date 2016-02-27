#pragma once
#include <atlbase.h>
#include "ComModule.h"
#include "ComObject.h"

template <typename Type, typename Interface, const CLSID* Clsid = nullptr>
class DispatchObject : public ComObject<Type, Interface, Clsid>
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
		return typeLibrary->GetTypeInfoOfGuid(__uuidof(Interface), &typeInfo);
	}

public:
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override
	{
		if (riid != IID_IDispatch)
			return ComObject<Type, Interface, Clsid>::QueryInterface(riid, ppvObject);
		if (ppvObject == nullptr)
			return E_POINTER;
		*ppvObject = static_cast<IDispatch*>(this);
		++referenceCount;
		return S_OK;
	}

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
			static_cast<IDispatch*>(this),
			dispIdMember,
			wFlags,
			pDispParams,
			pVarResult,
			pExcepInfo,
			puArgErr);
	}
};
