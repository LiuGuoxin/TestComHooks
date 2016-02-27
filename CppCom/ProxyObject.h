#pragma once
#include <objbase.h>
#include <atlbase.h>
#include "ComObject.h"
#include "ComInterfaces.h"

#if defined(ReportEvent)
#undef ReportEvent
#endif
#import <mscorlib.tlb> raw_interfaces_only named_guids

template <typename Type, typename Interface, const CLSID* Clsid, const CLSID* InnerClsid>
class ProxyObject : public ComObject<Type, ComInterfaces<Interface>, Clsid>
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
		CComQIPtr<mscorlib::IDisposable> disposable(inner);
		if (disposable)
			disposable->Dispose();
	}
};
