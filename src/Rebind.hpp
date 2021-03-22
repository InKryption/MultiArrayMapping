#ifndef INK_REBIND_IMPL_HEADER_FILE_GUARD
#define INK_REBIND_IMPL_HEADER_FILE_GUARD

#include <tuple>

namespace ink::Rebind {
	
	namespace detail {
		
		template<typename T, typename TT = typename std::remove_reference<T>::type>
		using tuple_reverse =
		decltype(
			[]<size_t... I>(T&& t, std::index_sequence<I...>) constexpr
			{ return std::make_tuple(std::get<sizeof...(I) - 1 - I>(std::forward<T>(t))...); }
			(	std::forward<T>( std::declval<T&&>() ),
				std::make_index_sequence<std::tuple_size<TT>::value>()	)
		);
		
		template<typename...> struct type_list;
		
		template<typename A, template<typename...> typename B = type_list> struct rebind_impl;
		template<typename A, typename B> struct concat_impl;
		
		
		// A type parameter pack struct
		// Also holds an internal alias to rebind
		template<typename... T> struct type_list
		{
			
			using tuple = std::tuple<T...>;
			
			template<template<typename...> typename U>
			using rebind = typename rebind_impl<type_list, U>::type;
			
			template<typename B>
			using concat = typename concat_impl<type_list, B>::type;
			
			template<size_t N>
			using get_n = std::tuple_element_t< N, rebind<std::tuple> >;
			
			
			
		};
		
		template<template<typename...> typename B, typename... T>
		struct rebind_impl<type_list<T...>, B>
		{ using type = B<T...>; };
		
		template<template<typename...> typename A, typename... T>
		struct rebind_impl<A<T...>, type_list>
		{ using type = type_list<T...>; };
		
		// Alias a type_list with the type parameters of typename A
		template<typename A>
		using rebind = typename rebind_impl<A,type_list>::type;
		
		template<typename... a, typename... b>
		struct concat_impl<type_list<a...>,type_list<b...>>
		{ using type = type_list<a...,b...>; };
		
		template<typename T> struct pop_last_impl;
		
		template<typename... Keep, typename Discard>
		struct pop_last_impl<type_list<Discard, Keep...>>
		{
			using type = type_list<Keep..., Discard>;
		};
		
	}
	
	using detail::type_list;
	using detail::rebind;
	
}

#endif
