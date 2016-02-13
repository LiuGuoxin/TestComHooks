#pragma once
#include "ComObject.h"
#include "CppCom_h.h"
#include <iostream>

class FooNative : public ComObject<FooNative, IFoo, &CLSID_FooNative>
{
public:
	FooNative()
	{
		std::cout << "C++ FooNative(ctor)" << std::endl;
	}

	~FooNative()
	{
		std::cout << "C++ ~FooNative()" << std::endl;
	}

	HRESULT __stdcall Bar() final
	{
		std::cout << "C++ FooNative.Bar" << std::endl;
		return S_OK;
	}
};
