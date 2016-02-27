#pragma once
#include "CppCom.h"
#include "ComObject.h"
#include <iostream>

class FooBoth : public ComObject<FooBoth, ComInterfaces<IFoo, IFooLate>, &CLSID_FooBoth>
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
