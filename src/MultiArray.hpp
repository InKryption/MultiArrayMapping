#ifndef INK_MULTIARRAY_HEADER_FILE_GUARD
#define INK_MULTIARRAY_HEADER_FILE_GUARD

#include <cstdint>
#include <tuple>
#include <math.h>

namespace ink {
	
	/**
	 * @param absolute: the absolute index into an N-dimensional array.
	 * @param dimensions: paremeters describing the sizes of all the the dimensions up-to the relevant dimension. 
	 * @returns The index into the Nth dimension of the theoretical array which is mapped to the given absolute index of the 1D array. The Nth dimension is the last in the series "dimensions".
	 */
	template<typename... _Size_Ts> requires( (std::convertible_to<_Size_Ts , size_t>&&...) && (sizeof...(_Size_Ts) > 0) )
	static constexpr size_t
	MapFromAbsolute(size_t absolute, _Size_Ts... dimensions)
	{
		const auto tuple = std::tie(dimensions...);
		
		const auto dimensionSize = std::get<sizeof...(dimensions) - 1>(tuple);
		const auto factor = (dimensions*...) / dimensionSize;
		
		const auto divFact = absolute / factor;
		
		return divFact - dimensionSize * (divFact / dimensionSize);
	}
	
	template<typename... _Size_Ts> requires( (std::convertible_to<_Size_Ts , size_t>&&...) && (sizeof...(_Size_Ts) > 0) )
	static constexpr size_t
	MapFromDimension(_Size_Ts... t)
	{
		const auto tuple = std::tie(t...);
		
		/*
			 
		*/
		
	}
	
	namespace detail {
		
		template<typename T>
		struct ARRAY_MAP_LAYER {
			enum { ELEMENT_COUNT = 1 };
			using type = std::remove_reference_t<T>;
			using sub_type = void;
		};
		
		template<typename T, size_t N>
		struct ARRAY_MAP_LAYER<T[N]> {
			enum { ELEMENT_COUNT = N };
			using type = T[N];
			using sub_type = ARRAY_MAP_LAYER<decltype(*std::declval<T[N]>())>;
		};
		
		template<typename T, size_t N>
		struct ARRAY_MAP_LAYER<T(&)[N]> {
			enum { ELEMENT_COUNT = N };
			using type = T[N];
			using sub_type = ARRAY_MAP_LAYER<decltype(std::declval<T[N]>()[0])>;
		};
		
		template<typename T>
		struct ARRAY_MAP_INFO {
			
			private:
			
			// First: total number of elements in (multidimensional) array T.
			// Second: total number of layers in multidimensional array T.
			static constexpr auto
			GetMappingArrayInfo_impl(size_t size = 1, size_t depth = 0)
			{
				
				using impl = ARRAY_MAP_LAYER<T>;
				size_t size_out = size * impl::ELEMENT_COUNT;
				
				if constexpr(std::is_array_v<typename impl::type>)
				{ return GetMappingArrayInfo_impl<typename impl::sub_type::type>(size_out, depth + 1); }
				else
				{ return std::make_pair(size_out, depth); }
				
			}
			
			static constexpr auto
			INFO = GetMappingArrayInfo_impl<T>();
			
			public:
			using TOP_LAYER = ARRAY_MAP_LAYER<T>;
			static constexpr size_t
				ELEMENT_COUNT = INFO.first,
				DEPTH = INFO.second
			;
			
		};
		
		template<typename T>
		struct ArrayMap_impl {
			
			public: using
			info = ARRAY_MAP_INFO<T>;
			
		};
		
	}
	
}

#endif
