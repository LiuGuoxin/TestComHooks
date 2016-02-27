#pragma once
#include <objbase.h>
#include "Object.h"
#include "Pointer.h"

#if defined(ReportEvent)
#undef ReportEvent
#endif
#import <mscorlib.tlb> raw_interfaces_only named_guids

namespace Com
{
	template <typename Type, const CLSID* Clsid, const CLSID* InnerClsid, typename Interface>
	class Wrapper : public Object<Type, Clsid, Interface>
	{
	protected:
		Pointer<Interface> inner;

	public:
		HRESULT OnFinalConstruct() override
		{
			return inner.CoCreateInstance(*InnerClsid, nullptr, CLSCTX_INPROC_SERVER);
		}

		void OnFinalRelease() override
		{
			QueryPointer<mscorlib::IDisposable> disposable(inner);
			if (disposable)
				disposable->Dispose();
		}
	};
}
