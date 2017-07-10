#ifndef ZUBERA_ENUMERABLE_H
#define ZUBERA_ENUMERABLE_H

#include <iostream>
#include <optional>

namespace zubera{


template<
	typename T,
	typename Eacher
>
struct enumerator;

template<typename T, typename Eacher>
constexpr enumerator<T, std::decay_t<Eacher>>
make_enumerator(Eacher&& eacher);


template<typename T>
struct vector;


template<
	typename Derived, typename Value, template<class> class Result = vector
>
struct enumerable{
	using self_t = Derived;
	using array_t = Result<Value>;
	using value_t = Value;

	constexpr self_t const&
	self() const{
		return static_cast<self_t const&>(*this);
	}


	template<typename Pred>
	constexpr bool
	all_of(Pred&& pred) const{
		return count() == count_if(std::forward<Pred>(pred));
	}

	constexpr bool
	all_of() const {
		return self().all_of([](auto it) constexpr{ return it; });
	}

	template<typename Pred>
	constexpr bool
	any_of(Pred&& pred) const{
		return count_if(std::forward<Pred>(pred)) >= 1;
	}

	constexpr bool
	any_of() const{
		return self().any_of([](auto it) constexpr{ return it; });
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
		return self().count_if([&t](auto it) constexpr{ return it == t; });
	}

	template<typename F>
	constexpr auto
	cycle(std::size_t size, F&& f) const{
// 		Result<Result<Value>> result{};
// 		auto a = self().to_a();
		for(std::size_t i = 0 ; i < size ; ++i){
			self().each(f);
		}
// 		return result;
	}

	template<typename Pred>
	constexpr auto
	detect(Pred&& pred) const{
		return self().find(std::forward<Pred>(pred));
	}

	constexpr auto
	drop(std::size_t n) const{
		return self().select().with_index([&](auto, std::size_t i) constexpr{
			return i >= n;
		});
	}

	template<typename F>
	constexpr auto
	each_with_index(F f) const{
		return self().inject(0, [&](auto i, auto it) constexpr{
			f(it, i);
			return ++i;
		});
	}

	template<typename Pred>
	constexpr auto
	find(Pred&& pred) const{
		using opt_t = std::optional<value_t>;
		return self().inject(opt_t{ std::nullopt }, [&](auto result, auto it) constexpr{
			return result   ? result
				 : pred(it) ? std::make_optional(it)
				 : std::nullopt;
		});
	}

	template<typename Init, typename F>
	constexpr auto
	inject(Init&& init, F f) const{
		self().each([&](auto it) constexpr{
			init = f(init, it);
			return it;
		});
		return init;
	}

	template<typename F>
	constexpr auto
	map(F&& f) const{
		using result_t = Result<decltype(f(std::declval<value_t>()))>;
		return self().inject(result_t{}, [&f](auto result, auto it) constexpr{
			return result.push(f(it));
		});
	}

	constexpr auto
	map() const{
		return to_enum([](auto self, auto y) constexpr{ return self.map(y); });
	}

	template<typename Pred>
	constexpr auto
	select(Pred&& pred) const{
		return self().inject(array_t{}, [&pred](auto sum, auto it) constexpr{
			return pred(it) ? sum.push(it) : sum;
		});
	}

	constexpr auto
	select() const{
		return to_enum([](auto self, auto y) constexpr{ return self.select(y); });
	}

	constexpr auto
	take(std::size_t n) const{
		return self().select().with_index([&](auto, std::size_t i) constexpr{
			return i < n;
		});
	}

	constexpr auto
	to_a() const{
		return self().select([](auto) constexpr{ return true; });
	}



	template<typename F>
	constexpr auto
	to_enum(F&& f) const{
		return make_enumerator<value_t>([self = this->self(), f](auto y) constexpr{
			return f(self, y);
		});
	}



	constexpr bool
	is_empty() const{
		return count() == 0;
	}

	template<typename T>
	constexpr auto
	concat(T&& t) const{
		return t.inject(self().to_a(), [](auto sum, auto it) constexpr{
			return sum.push(it);
		});
	}

	template<typename T, typename F>
	constexpr bool
	equal(T&& rhs, F f) const{
		if( self().count() != rhs.count() ){
			return false;
		}
		auto result = true;
		self().each_with_index([&](auto lhs, auto i) constexpr{
			rhs.each_with_index([&](auto rhs, auto j) constexpr{
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
		return self().equal(std::forward<T>(rhs), [](auto a, auto b) constexpr{ return a == b; });
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

#include "./enumerator.hpp"
#include "./vector.hpp"

#endif /* ZUBERA_ENUMERABLE */
