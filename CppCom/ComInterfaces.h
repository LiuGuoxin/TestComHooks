#pragma once
#include <objbase.h>
#include "DispatchInterfaceSelector.h"
#include <type_traits>

template <typename... Interfaces>
class ComInterfaces
{
public:
	bool QueryInterfaceInternal(REFIID riid, void** ppvObject)
	{
		return false;
	}
};

template <typename Interface, typename... Interfaces>
class ComInterfaces<Interface, Interfaces...> : public Interface, public ComInterfaces<Interfaces...>
{
public:
	using PrimaryInterface = Interface;
	using DispatchInterface = typename DispatchInterfaceSelector<Interface, Interfaces...>::DispatchInterface;
	static constexpr bool ObjectSupportsDispatch = !std::is_void<DispatchInterface>::value;

	bool QueryInterfaceInternal(REFIID riid, void** ppvObject)
	{
		if (riid != __uuidof(Interface))
			return ComInterfaces<Interfaces...>::QueryInterfaceInternal(riid, ppvObject);
		*ppvObject = static_cast<Interface*>(this);
		return true;
	}
};
