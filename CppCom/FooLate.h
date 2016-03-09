#pragma once
#include <Com/Com.h>
#include "CppCom.h"
#include <iostream>

class FooLate : public Com::Object<FooLate, &CLSID_FooLate, IFooLate>
{
public:
	FooLate()
	{
		std::cout << "C++ FooLate(ctor)" << std::endl;
	}

	~FooLate()
	{
		std::cout << "C++ ~FooLate()" << std::endl;
	}

	HRESULT __stdcall BarLate() final
	{
		std::cout << __FUNCTION__ << std::endl;
		return S_OK;
	}
};
