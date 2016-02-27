#pragma once
#include "Object.h"
#include "CppCom.h"
#include <iostream>

class FooNative : public Com::Object<FooNative, Com::InterfaceList<IFoo>, &CLSID_FooNative>
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
