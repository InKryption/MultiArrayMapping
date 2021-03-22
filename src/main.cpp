#include "main.hpp"

template<typename T, typename TT = typename std::remove_reference<T>::type>
static constexpr auto
reverse_tuple(T&& t)
{
	auto impl = []<size_t... I>(T&& t, std::index_sequence<I...>) constexpr
	{ return std::make_tuple(std::get<sizeof...(I) - 1 - I>(std::forward<T>(t))...); };
	
    return
	impl(
		std::forward<T>(t),
		std::make_index_sequence<std::tuple_size<TT>::value>()
	);
	
}

/**
 *	@param dimension_sizes Sizes corresponding to the sizes of the arrays to sub-arrays in-order
 *	@param indexes Indexes corresponding to the absolute index of interest.
*/
template<size_t... dimension_sizes>
static constexpr auto
TransposeToAbsolute(decltype(dimension_sizes)... indexes)
{
	constexpr auto iseq = std::make_index_sequence<sizeof...(dimension_sizes)>();
	const std::tuple TUP = std::make_tuple(std::make_pair(dimension_sizes,indexes)...);
	
	size_t	out = 0,
			factor = (dimension_sizes*...);
	
	std::apply(	[&](const auto&... i) constexpr
				{ ((factor/= i.first, out+= i.second * factor),...); }, TUP );
	
	return out;
}

// don't like how this is rn
template<size_t... dimension_sizes>
static constexpr auto
TransposeFromAbsolute(size_t index)
{
	std::tuple<decltype(dimension_sizes)...> OUT;
	size_t	factor = 1;
	
	std::apply(	[&](auto&... i) constexpr
				{ (( i= ( (index / factor) - (i / factor) ) % dimension_sizes, factor*=dimension_sizes),...); }, OUT );
	
	return reverse_tuple(OUT);
}

int main() {
	
	namespace rb = ink::Rebind;
	
	//rb::type_list<int,char>::concat<rb::type_list<bool, size_t>>::get_n<3>;
		
	return 0;
	
	constexpr size_t
	W = 3,
	H = 3,
	D = 3;
	
	constexpr size_t ARR1D[] =
	{
		0	,	1	,	2	,
		3	,	4	,	5	,
		6	,	7	,	8	,
		
		9	,	10	,	11	,
		12	,	13	,	14	,
		15	,	16	,	17	,
		
		18	,	19	,	20	,
		21	,	22	,	23	,
		24	,	25	,	26	,
	};
	
	constexpr size_t ARR3D[3][3][3] =
	{
		{
			{0	,	1	,	2	},
			{3	,	4	,	5	},
			{6	,	7	,	8	}
		},
		{
			{9	,	10	,	11	},
			{12	,	13	,	14	},
			{15	,	16	,	17	}
		},
		{
			{18	,	19	,	20	},
			{21	,	22	,	23	},
			{24	,	25	,	26	}
		},
	};
	
	constexpr auto indices = (TransposeFromAbsolute<D,H,W>(8));
	constexpr auto test3D = ARR3D[0][0][2];
	constexpr auto test1D = ARR1D[TransposeToAbsolute<D,H,W>(0,0,2)];
	return 0;
}
