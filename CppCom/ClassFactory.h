#pragma once
#include <objbase.h>
#include "ComObject.h"
#include "ComModule.h"

template <typename Type>
class ClassFactory : public ComObject<ClassFactory<Type>, ComInterfaces<IClassFactory>>
{
public:
	HRESULT __stdcall CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) final
	{
		if (pUnkOuter != nullptr)
			return CLASS_E_NOAGGREGATION;
		return Type::TryCreateInstance(riid, ppvObject);
	}

	HRESULT __stdcall LockServer(BOOL fLock) final
	{
		if (fLock)
			ComModule::GetInstance().AddRef();
		else
			ComModule::GetInstance().Release();
		return S_OK;
	}
};
