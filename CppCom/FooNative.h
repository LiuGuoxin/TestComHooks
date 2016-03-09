#pragma once
#include <Com/Com.h>
#include "CppCom.h"
#include <iostream>

class FooNative : public Com::Object<FooNative, &CLSID_FooNative, IFoo>
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
