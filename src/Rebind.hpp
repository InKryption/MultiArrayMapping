#ifndef INK_REBIND_IMPL_HEADER_FILE_GUARD
#define INK_REBIND_IMPL_HEADER_FILE_GUARD

#include <tuple>

namespace ink::Rebind {
	
	namespace detail {
		
		// A type parameter pack struct
		template<typename...>
		struct type_list
		{};
		
		
		
		template<typename A, template<typename...> typename B> struct rebind_impl;
		
		template<template<typename...> typename A, template<typename...> typename B, typename... T>
		struct rebind_impl<A<T...>, B>
		{ using type = B<T...>; };
		
		// Represents the instantiation of template B with the parameters of the instantiated template type A
		template<typename A, template<typename...> typename B>
		using rebind = typename rebind_impl<A,B>::type;
		
		
		
		template<size_t N, typename T>
		using get_n = std::tuple_element<N, rebind<T, std::tuple>>::type;
		
		
		
		template<typename A, typename B> struct concat_impl;
		
		template<template<typename...> typename A, template<typename...> typename B, typename...T1, typename...T2>
		struct concat_impl<A<T1...>, B<T2...>>
		{ using type = type_list<T1..., T2...>; };
		
		// Concatenate the type arguments of A and B into a type_list.
		template<typename A, typename B>
		using concat = concat_impl<A,B>::type;
		
		
		
		template<typename T> struct remove_first_impl;
		
		template<template<typename...> typename T, typename discard, typename... ts>
		struct remove_first_impl<T<discard, ts...>>
		{ using type = type_list<ts...>; };
		
		template<typename T>
		using remove_first = remove_first_impl<T>::type;
		
		
		
	}
	
	using detail::type_list;
	using detail::rebind;
	using detail::get_n;
	using detail::concat;
	
	using detail::remove_first;
	
}

#endif
