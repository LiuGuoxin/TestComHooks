#pragma once
#include <type_traits>

template <typename... Interfaces>
class DispatchInterfaceSelector
{
public:
	using DispatchInterface = void;
};

template <typename Interface, typename... Interfaces>
class DispatchInterfaceSelector<Interface, Interfaces...>
{
public:
	using DispatchInterface = typename std::conditional<
		std::is_base_of<IDispatch, Interface>::value,
		Interface,
		typename DispatchInterfaceSelector<Interfaces...>::DispatchInterface
	>::type;
};
