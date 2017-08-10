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

template<typename T, typename ...Args>
constexpr auto is_include_v = (std::is_same_v<T, Args> || ...);

template<template<class...> class Ts, typename... Args1, typename... Args2>
constexpr Ts<Args1..., Args2...>
cat(Ts<Args1...>, Ts<Args2...>){ return {}; }

template<typename... Args>
struct parameter_pack{};

template<typename Head, typename... Args, template<class...> class Result = parameter_pack>
constexpr auto
unique_impl(){
	if constexpr (sizeof...(Args) == 0){
		return Result<Head>{};
	}
	else if constexpr (is_include_v<Head, Args...>){
		return unique_impl<Args...>();
	}
	else {
		return cat(Result<Head>{}, unique_impl<Args...>());
	}
}

template<typename... Args, template<class...> class Result = parameter_pack>
constexpr auto
unique(){
	if constexpr (sizeof...(Args) == 0){
		return Result<>{};
	}
	else {
		return unique_impl<Args...>();
	}
}

template<typename... Args>
using unique_t = decltype(unique<Args...>());


template<typename... Args>
struct variant;

template<typename T, typename U, typename... Args>
struct variant<parameter_pack<T, U, Args...>>{
	using type = std::variant<T, U, Args...>;
};

template<typename T>
struct variant<parameter_pack<T>>{
	using type = T;
};

template<>
struct variant<parameter_pack<>>{
	using type = std::any;
};

template<typename... Args>
using variant_t = typename variant<unique_t<Args...>>::type;


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
struct tuple
	: std::tuple<Args...>
	, enumerable<tuple<Args...>, tuple_detail::variant_t<Args...>> {
	using enum_ = enumerable<tuple<Args...>, tuple_detail::variant_t<Args...>>;
	using value_t = typename enum_::value_t;

	using std::tuple<Args...>::tuple;

	template<typename F>
	constexpr auto
	each(F f) const{
		std::apply([&](auto... args) constexpr{
			(f(args), ...);
		}, std::tuple<Args...>(*this));
	}

	constexpr auto
	each() const{
		return enum_::to_enum([](auto self, auto y) constexpr{ return self.each(y); });
	}

	template<typename F>
	constexpr auto
	map(F&& f) const{
		if constexpr(sizeof...(Args) == 0){
			return zubera::vector<decltype(f(std::declval<value_t>()))>{};
		}
		else {
			return std::apply([&](auto... args){
				return zubera::tuple{ f(args)... };
			}, (std::tuple<Args...>)(*this));
		}
	}

	constexpr auto
	map() const{
		return make_enumerator<typename enum_::value_t>([this](auto y) constexpr{ return this->map(y); });
	}
};

template<typename... Args>
tuple(Args&&... args) -> tuple<tuple_detail::special_decay_t<Args>...>;


}  // namespace zubera


namespace std{

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmismatched-tags"
#endif

template<typename... Args>
class tuple_size<zubera::tuple<Args...>> : public tuple_size<std::tuple<Args...>>{};

template<std::size_t N, typename... Args>
class tuple_element<N, zubera::tuple<Args...>> : public tuple_element<N, std::tuple<Args...>>{};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace std




#endif /* ZUBERA_TUPLE */
