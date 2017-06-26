#ifndef ZUBERA_TUPLE_H
#define ZUBERA_TUPLE_H

#include <functional>
#include <tuple>
#include <variant>
#include <utility>
#include "./enumerable.hpp"

namespace zubera{

namespace tuple_detail{

template<typename T, typename U, typename... Args>
constexpr bool
is_exist(){
	if constexpr (std::is_same<T, U>{}){
		return true;
	}
	else if constexpr (std::tuple_size<std::tuple<Args...>>{} == 0){
		return false;
	}
	else {
		return is_exist<T, Args...>();
	}
}


template<typename T, typename... Args>
constexpr auto
unique(){
	if constexpr (std::tuple_size<std::tuple<Args...>>{} == 0){
		return std::tuple<T>{};
	}
	else if constexpr (is_exist<T, Args...>()){
		return unique<Args...>();
	}
	else {
		return std::tuple_cat(std::tuple<T>{}, unique<Args...>());
	}
}

template<typename... Args>
constexpr std::variant<Args...>
tuple_to_variant(std::tuple<Args...>);

template<typename T, typename... Args>
struct variant{
	using params = decltype(unique<T, Args...>());
	using type = std::conditional_t<std::tuple_size<params>{} == 1, T, decltype(tuple_to_variant(params{}))>;
};

}  // namespace tuple_detail


template<typename... Args>
struct tuple : std::tuple<Args...>, enumerable<tuple<Args...>, typename tuple_detail::variant<Args...>::type>{
	using std::tuple<Args...>::tuple;

	template<typename F>
	constexpr auto
	each(F f) const{
		std::apply([&](auto... args){
			(f(args), ...);
		}, std::tuple<Args...>(*this));
	}

	template<typename F>
	constexpr auto
	map(F f) const{
		return std::apply([&](auto... args){
			return tuple<decltype(f(args))...>{ f(args)... };
		}, std::tuple<Args...>(*this));
	}
};

template<typename... Args>
tuple(Args&&... args) -> tuple<std::decay_t<Args>...>;


}  // namespace zubera

#endif /* ZUBERA_TUPLE */
