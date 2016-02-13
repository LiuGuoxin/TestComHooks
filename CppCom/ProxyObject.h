#pragma once
#include <objbase.h>
#include <atlbase.h>
#include "ComObject.h"
#include "IDisposable.h"

template <typename Type, typename Interface, const CLSID* Clsid, const CLSID* InnerClsid>
class ProxyObject : public ComObject<Type, Interface, Clsid>
{
protected:
	CComPtr<Interface> inner;

public:
	HRESULT OnFinalConstruct() override
	{
		return inner.CoCreateInstance(*InnerClsid, nullptr, CLSCTX_INPROC_SERVER);
	}

	void OnFinalRelease() override
	{
		CComQIPtr<IDisposable> disposable(inner);
		if (disposable)
			disposable->Dispose();
	}
};
