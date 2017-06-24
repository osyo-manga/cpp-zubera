#ifndef ZUBERA_ENUMERABLE_H
#define ZUBERA_ENUMERABLE_H

#include <iostream>

namespace zubera{

template<typename T>
struct vector;

template<
	typename Derived, typename Value, template<class> class Result = vector
>
struct enumerable{
	using self_t = Derived;
	using Tarray_t = Result<Value>;

	constexpr self_t const&
	self() const{
		return *static_cast<self_t const*>(this);
	}

	constexpr bool
	is_empty() const{
		return count() == 0;
	}

	template<typename F>
	constexpr auto
	each_with_index(F f) const{
		return self().inject(0, [&](auto i, auto it) constexpr {
			f(it, i);
			return ++i;
		});
	}



	template<typename Init, typename F>
	constexpr auto
	inject(Init init, F f) const{
		self().each([&](auto n) constexpr {
			init = f(init, n);
		});
		return init;
	}


	template<typename Pred>
	constexpr std::size_t
	count_if(Pred pred) const{
		return self().inject(0, [&pred](auto sum, auto it) constexpr {
			return pred(it) ? sum + 1 : sum;
		});
	}

	constexpr auto
	count() const{
		return self().count_if([](...) constexpr { return true; });
	}

	template<typename T>
	constexpr auto
	count(T&& t) const{
		return self().count_if([&t](auto it){ return it == t; });
	}

	template<typename Pred>
	auto
	select(Pred pred){
		return self().inject(Tarray_t{}, [&pred](auto sum, auto it){
			return pred(it) ? sum.push(it) : sum;
		});
	}

	template<typename F>
	auto
	map(F f){
		using result_t = Result<decltype(f(std::declval<Value>()))>;
		return self().inject(result_t{}, [&f](auto result, auto it){
			return result.push(f(it));
		});
	}

// 	template<typename Seq>
// 	bool
};

template<
	typename Derived, typename T, template<class U> class Result
>
std::ostream&
operator <<(std::ostream& os, enumerable<Derived, T, Result> const& e){
	e.self().each([&os](auto n){
		os << n << ", ";
	});
	return os;
}

}  // namespace zubera

#include "./vector.hpp"

#endif /* ZUBERA_ENUMERABLE */
