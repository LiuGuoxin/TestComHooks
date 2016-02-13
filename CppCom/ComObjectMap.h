#pragma once
#include <objbase.h>
#include "ClassFactory.h"

template <typename... Types>
class ComObjectMap
{
public:
	static HRESULT Create(REFCLSID rclsid, REFIID riid, void** ppv)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}
};

template <typename Type, typename... Types>
class ComObjectMap<Type, Types...>
{
public:
	static HRESULT Create(REFCLSID rclsid, REFIID riid, void** ppv)
	{
		return Type::Is(rclsid) ?
			ClassFactory<Type>::TryCreateInstance(riid, ppv) :
			ComObjectMap<Types...>::Create(rclsid, riid, ppv);
	}
};
