#pragma once
#include <Com/Com.h>
#include "CppCom.h"
#include <iostream>

class FooBoth : public Com::Object<FooBoth, &CLSID_FooBoth, IFoo, IFooLate>
{
public:
	FooBoth()
	{
		std::cout << "C++ FooBoth(ctor)" << std::endl;
	}

	~FooBoth()
	{
		std::cout << "C++ ~FooBoth()" << std::endl;
	}

	HRESULT __stdcall Bar() final
	{
		std::cout << __FUNCTION__ << std::endl;
		return S_OK;
	}

	HRESULT __stdcall BarLate() final
	{
		std::cout << __FUNCTION__ << std::endl;
		return S_OK;
	}
};
