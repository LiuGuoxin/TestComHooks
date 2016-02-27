#pragma once
#include "Wrapper.h"
#include "CppCom.h"
#import "c:\save\code\tests\TestComHooks\CsCom\bin\Debug\CsCom.tlb" raw_interfaces_only named_guids

class FooWrapper : public Com::Wrapper<FooWrapper, &CLSID_FooWrapper, &CsCom::CLSID_Foo, IFoo>
{
public:
	HRESULT __stdcall Bar() final
	{
		return inner->Bar();
	}
};
