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
	using value_t = Value;

	constexpr self_t const&
	self() const{
		return *static_cast<self_t const*>(this);
	}


	template<typename Pred>
	constexpr bool
	all_of(Pred&& pred) const{
		return count() == count_if(std::forward<Pred>(pred));
	}

	constexpr bool
	all_of() const {
		return self().all_of([](auto it){ return it; });
	}

	template<typename Pred>
	constexpr bool
	any_of(Pred&& pred) const{
		return count_if(std::forward<Pred>(pred)) >= 1;
	}

	constexpr bool
	any_of() const{
		return self().any_of([](auto it){ return it; });
	}

	template<typename F>
	constexpr auto
	collect(F&& f) const{
		return self().map(std::forward<F>(f));
	}

	template<typename Pred>
	constexpr std::size_t
	count_if(Pred&& pred) const{
		return self().inject(0, [&pred](auto sum, auto it) constexpr {
			return pred(it) ? sum + 1 : sum;
		});
	}

	constexpr auto
	count() const{
		return self().count_if([](auto...) constexpr { return true; });
	}

	template<typename T>
	constexpr auto
	count(T&& t) const{
		return self().count_if([&t](auto it){ return it == t; });
	}

	template<typename F>
	constexpr auto
	cycle(std::size_t size, F&& f) const{
// 		Result<Result<Value>> result{};
// 		auto a = self().to_a();
		for(std::size_t i = 0 ; i < size ; ++i){
			self().each(std::forward<F>(f));
		}
// 		return result;
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
	inject(Init&& init, F f) const{
		self().each([&](auto n) constexpr {
			init = f(std::forward<Init>(init), n);
		});
		return init;
	}

	template<typename F>
	auto
	map(F&& f) const{
		using result_t = Result<decltype(f(std::declval<Value>()))>;
		return self().inject(result_t{}, [&f](auto result, auto it){
			return result.push(f(it));
		});
	}

	template<typename Pred>
	auto
	select(Pred pred) const{
		return self().inject(Tarray_t{}, [&pred](auto sum, auto it){
			return pred(it) ? sum.push(it) : sum;
		});
	}

	constexpr auto
	to_a() const{
		return self().select([](auto){ return true; });
	}

	constexpr bool
	is_empty() const{
		return count() == 0;
	}

	template<typename T>
	constexpr auto
	concat(T&& t) const{
		return t.inject(self().to_a(), [](auto sum, auto it) constexpr {
			return sum.push(it);
		});
	}

	template<typename T, typename F>
	constexpr bool
	equal_to(T&& rhs, F f) const{
		if( self().count() != rhs.count() ){
			return false;
		}
		auto result = true;
		self().each_with_index([&](auto lhs, auto i){
			rhs.each_with_index([&](auto rhs, auto j){
				if( result && i == j ){
					result = f(lhs, rhs);
				}
			});
		});
		return result;
	}

	template<typename T>
	constexpr bool
	equal(T&& rhs) const{
		return self().equal_to(std::forward<T>(rhs), [](auto a, auto b) constexpr { return a == b; });
	}

	template<typename T>
	constexpr bool
	operator ==(T&& t) const{
		return self().equal(std::forward<T>(t));
	}

	template<typename T>
	constexpr bool
	operator !=(T&& t) const{
		return !self().equal(std::forward<T>(t));
	}
};

template<
	typename Derived, typename T, template<class U> class Result
>
std::ostream&
operator <<(std::ostream& os, enumerable<Derived, T, Result> const& e){
	os << "[ ";
	e.self().each([&os](auto n){
		os << n << ", ";
	});
	os << " ]";
	return os;
}

}  // namespace zubera

#include "./vector.hpp"

#endif /* ZUBERA_ENUMERABLE */
