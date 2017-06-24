#ifndef ZUBERA_TUPLE_H
#define ZUBERA_TUPLE_H

#include <tuple>
#include <variant>
#include <utility>
#include "./enumerable.hpp"

namespace zubera{

namespace tuple_detail{

template<typename Tuple, typename F, std::size_t... Indices>
constexpr void
apply(Tuple&& t, F f, std::index_sequence<Indices...>){
	[](...){}((f(std::get<Indices>(t)), 0)...);
}

}  // namespace tuple_detail

template<typename... Args>
struct tuple : std::tuple<Args...>, enumerable<tuple<Args...>, std::variant<Args...>>{
	using std::tuple<Args...>::tuple;

	template<typename F>
	constexpr auto
	each(F f) const{
		tuple_detail::apply(*this, f, std::index_sequence_for<Args...>{});
	}
};

template<typename... Args>
constexpr tuple<typename std::decay<Args>::type...>
make_tuple(Args&&... args){
	return { std::forward<Args>(args)... };
}

}  // namespace zubera

#endif /* ZUBERA_TUPLE */
