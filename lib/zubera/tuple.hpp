#ifndef ZUBERA_TUPLE_H
#define ZUBERA_TUPLE_H

#include <functional>
#include <tuple>
#include <variant>
#include <any>
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


template<typename... Args>
struct variant;

template<typename T, typename... Args>
struct variant<T, Args...>{
	using params = decltype(unique<T, Args...>());
	using type = std::conditional_t<std::tuple_size<params>{} == 1, T, decltype(tuple_to_variant(params{}))>;
};

template<>
struct variant<>{
	using type = std::any;
// 	using type = std::variant<>;
};

template<typename... Args>
using variant_t = typename variant<Args...>::type;

template<typename T>
struct unwrap_refwrapper{
	using type = T;
};

template<typename T>
struct unwrap_refwrapper<std::reference_wrapper<T>>{
	using type = T&;
};

template<typename T>
using special_decay_t = typename unwrap_refwrapper<typename std::decay<T>::type>::type;


}  // namespace tuple_detail


template<typename... Args>
struct tuple : std::tuple<Args...>, enumerable<tuple<Args...>, tuple_detail::variant_t<Args...>>{
	using enum_ = enumerable<tuple<Args...>, tuple_detail::variant_t<Args...>>;

	using std::tuple<Args...>::tuple;

	template<typename F>
	constexpr auto
	each(F f) const{
		std::apply([&](auto... args){
			(f(args), ...);
		}, std::tuple<Args...>(*this));
	}

	// [WIP]
// 	template<typename F>
// 	constexpr auto
// 	map(F&& f) const{
// 		return std::apply([&](auto... args){
// 			return zubera::tuple{ f(args)... };
// 		}, (std::tuple<Args...>)(*this));
// 	}
//
// 	constexpr auto
// 	map() const{
// 		return make_enumerator<typename enum_::value_t>([this](auto y) constexpr{ return this->map(y); });
// 	}
};

template<typename... Args>
tuple(Args&&... args) -> tuple<tuple_detail::special_decay_t<Args>...>;


}  // namespace zubera

#endif /* ZUBERA_TUPLE */
