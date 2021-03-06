#ifndef ZUBERA_ENUMERABLE_H
#define ZUBERA_ENUMERABLE_H

#include <iostream>
#include <functional>
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


template<typename... Args>
struct tuple;


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
	cycle(std::size_t count, F&& f) const{
// 		Result<Result<Value>> result{};
// 		auto a = self().to_a();
		for(std::size_t i = 0 ; i < count ; ++i){
			self().each(f);
		}
// 		return result;
	}

	constexpr auto
	cycle(std::size_t count) const{
		return self().to_enum([=](auto self, auto y){
			return self.cycle(count, y);
		});
	}

	template<typename Pred>
	constexpr auto
	detect(Pred&& pred) const{
		return self().find(std::forward<Pred>(pred));
	}

	constexpr auto
	drop(std::size_t n) const{
		return self().find_all().with_index([&](auto, std::size_t i) constexpr{
			return i >= n;
		});
	}

	template<typename Pred>
	constexpr auto
	drop_while(Pred&& pred) const{
		auto index = self().find_index(std::not_fn(std::forward<Pred>(pred)));
		return self().drop(index ? *index : self().count());
	}

	template<typename F>
	constexpr auto
	each_cons(std::size_t num, F&& f) const{
		auto count = self().count();
		if( count < num ){
			return;
		}
		if( count == num ){
			f(self().take(num));
			return;
		}
		self().each_with_index([&](auto, auto i) constexpr{
			if( count - i < num ){
				return;
			}
			f(self().drop(i).take(num));
		});
	}

	constexpr auto
	each_cons(std::size_t num) const{
		using value_t = decltype(self().take(0));
		return make_enumerator<value_t>([self = this->self(), num](auto y) constexpr{
			return self.each_cons(num, y);
		});
	}

	template<typename F>
	constexpr auto
	each_slice(std::size_t num, F&& f) const{
		if( self().is_empty() ){
			return;
		}
		f(self().take(num));
		self().drop(num).each_slice(num, f);
	}

	constexpr auto
	each_slice(std::size_t num) const{
		using value_t = decltype(self().take(0));
		return make_enumerator<value_t>([self = this->self(), num](auto y) constexpr{
			return self.each_slice(num, y);
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

	template<typename Obj, typename F>
	constexpr auto
	each_with_object(Obj&& obj, F&& f){
		self().each([&](auto it) constexpr{
			f(it, obj);
		});
		return obj;
	}

	template<typename Obj>
	constexpr auto
	each_with_object(Obj&& obj){
		return to_enum([=](auto self, auto y){ return self.each_with_object(obj, y); });
	}

	constexpr auto
	entries() const{
		return self().to_a();
	}

	template<typename Pred>
	constexpr auto
	find(Pred&& pred) const{
		// NOTE:Not working by constexpr
		using opt_t = std::optional<value_t>;
		return self().inject(opt_t{ std::nullopt }, [&](auto result, auto it) constexpr{
			return result   ? result
				 : pred(it) ? std::make_optional(it)
				 : std::nullopt;
		});
	}

	constexpr auto
	find() const{
		return self().to_enum([](auto self, auto y) constexpr{ return self.find(y); });
	}

	template<typename Pred>
	constexpr auto
	find_all(Pred&& pred) const{
		return self().inject(array_t{}, [&pred](auto sum, auto it) constexpr{
			return pred(it) ? sum.push(it) : sum;
		});
	}

	constexpr auto
	find_all() const{
		return self().to_enum([](auto self, auto y) constexpr{ return self.find_all(y); });
	}

	template<typename Pred>
	constexpr auto
	find_index(Pred&& pred) const{
		int result = -1;
		self().each_with_index([&](auto it, auto i){
			if( result == -1 && pred(it) ){
				result = i;
			}
		});
		return result == -1 ? std::nullopt : std::make_optional(std::size_t(result));
	}

	constexpr auto
	first() const{
		return self().find().with_index([](auto, auto i){
			return i == 0;
		});
	}

	constexpr auto
	first(std::size_t n) const{
		return self().take(n);
	}

	template<typename T>
	constexpr bool
	include(T&& t) const{
		return count(std::forward<T>(t)) != 0;
	}

	template<typename Init, typename F>
	constexpr auto
	inject(Init init, F f) const{
		self().each([&](auto it) constexpr{
			init = f(init, it);
			return it;
		});
		return init;
	}

	template<typename F>
	constexpr auto
	inject(F f) const{
		return self().inject(value_t{}, std::forward<F>(f));
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
		return self().to_enum([](auto self, auto y) constexpr{ return self.map(y); });
	}

	template<typename Comp,
		std::enable_if_t<!std::is_integral_v<std::decay_t<Comp>>, std::nullptr_t> = nullptr
	>
	constexpr auto
	max(Comp&& comp) const{
		return self().inject(self().first(), [&](auto max, auto it) constexpr{
			return comp(*max, it) > 0 ? *max : it;
		});
	}

	template<typename Comp>
	constexpr auto
	max(std::size_t n, Comp&& comp) const{
		return self().sort(comp).reverse_each().take(n);
	}

	constexpr auto
	max(std::size_t n) const{
		return self().sort().reverse_each().take(n);
	}

	constexpr auto
	max() const{
		return self().max([](auto a, auto b){ return a > b; });
	}

	template<typename F,
		std::enable_if_t<!std::is_integral_v<std::decay_t<F>>, std::nullptr_t> = nullptr
	>
	constexpr auto
	max_by(F&& f) const{
		auto result = self().map([&](auto it) constexpr{
			return std::make_pair(f(it), it);
		}).max([](auto a, auto b) constexpr{
			return a.first > b.first;
		});
		return result ? std::make_optional(result->second) : std::nullopt;
	}

	template<typename F>
	constexpr auto
	max_by(std::size_t n, F&& f){
		return self().sort_by(f).reverse_each().take(n);
	}

	constexpr auto
	max_by() const{
		return self().to_enum([](auto self, auto y) constexpr{ return self.max_by(y); });
	}

	constexpr auto
	max_by(std::size_t n) const{
		return self().to_enum([=](auto self, auto y) constexpr{ return self.max_by(n, y); });
	}

	template<typename... Args>
	constexpr auto
	member(Args&&... args) const{
		return self().include(std::forward<Args>(args)...);
	}

	template<typename Comp,
		std::enable_if_t<!std::is_integral_v<std::decay_t<Comp>>, std::nullptr_t> = nullptr
	>
	constexpr auto
	min(Comp&& comp) const{
		return self().inject(self().first(), [&](auto min, auto it) constexpr{
			return comp(*min, it) > 0 ? it : *min;
		});
	}

	template<typename Comp>
	constexpr auto
	min(std::size_t n, Comp&& comp) const{
		return self().sort(comp).take(n);
	}

	constexpr auto
	min(std::size_t n) const{
		return self().sort().take(n);
	}

	constexpr auto
	min() const{
		return self().min([](auto a, auto b){ return a > b; });
	}

	template<typename F,
		std::enable_if_t<!std::is_integral_v<std::decay_t<F>>, std::nullptr_t> = nullptr
	>
	constexpr auto
	min_by(F&& f) const{
		auto result = self().map([&](auto it) constexpr{
			return std::make_pair(f(it), it);
		}).min([](auto a, auto b) constexpr{
			return a.first > b.first;
		});
		return result ? std::make_optional(result->second) : std::nullopt;
	}

	template<typename F>
	constexpr auto
	min_by(std::size_t n, F&& f){
		return self().sort_by(f).take(n);
	}

	constexpr auto
	min_by() const{
		return self().to_enum([](auto self, auto y) constexpr{ return self.min_by(y); });
	}

	constexpr auto
	min_by(std::size_t n) const{
		return self().to_enum([=](auto self, auto y) constexpr{ return self.min_by(n, y); });
	}

	template<typename Comp>
	constexpr auto
	minmax(Comp&& comp) const{
		auto first = self().first();
		return self().inject(zubera::tuple{first, first} , [&](auto result, auto it) constexpr{
			auto [min, max] = result;
			return zubera::tuple{
				comp(*min, it) > 0 ? it   : *min,
				comp(*max, it) > 0 ? *max : it,
			};
		});
	}

	constexpr auto
	minmax() const{
		return self().minmax([](auto a, auto b){ return a > b; });
	}

	template<typename F,
		std::enable_if_t<!std::is_integral_v<std::decay_t<F>>, std::nullptr_t> = nullptr
	>
	constexpr auto
	minmax_by(F&& f) const{
		return self().map([&](auto it) constexpr{
			return std::make_pair(f(it), it);
		}).minmax([](auto a, auto b) constexpr{
			return a.first > b.first;
		}).map([](auto it){
			return it ? std::make_optional(it->second) : std::nullopt;
		});
	}

	constexpr auto
	minmax_by() const{
		return self().to_enum([](auto self, auto y) constexpr{ return self.minmax_by(y); });
	}

	template<typename Pred>
	constexpr bool
	none_of(Pred&& pred) const{
		return self().count_if(std::forward<Pred>(pred)) == 0;
	}

	constexpr bool
	none_of() const{
		return self().none_of([](auto it) constexpr{ return it; });
	}

	template<typename Pred>
	constexpr bool
	one_of(Pred&& pred) const{
		return self().count_if(std::forward<Pred>(pred)) == 1;
	}

	constexpr bool
	one_of() const{
		return self().one_of([](auto it) constexpr{ return it; });
	}

	template<typename Pred>
	constexpr auto
	partition(Pred&& pred) const{
		return self().inject(zubera::tuple{ array_t{}, array_t{} }, [&](auto memo, auto it){
			auto&& [left, right] = memo;
			if( pred(it) ){
				return zubera::tuple{ left.push(it), right };
			}
			else{
				return zubera::tuple{ left, right.push(it) };
			}
		});
	}

	constexpr auto
	partition() const{
		return self().to_enum([](auto self, auto y) constexpr{ return self.partition(y); });
	}

	template<typename... Args>
	constexpr auto
	reduce(Args&&... args){
		return self().inject(std::forward<Args>(args)...);
	}

	template<typename Pred>
	constexpr auto
	reject(Pred&& pred) const{
		return self().find_all(std::not_fn(std::forward<Pred>(pred)));
	}

	constexpr auto
	reject() const{
		return self().to_enum([](auto self, auto y) constexpr{ return self.reject(y); });
	}

	template<typename F>
	constexpr auto
	reverse_each(F&& f) const{
		if( self().is_empty() ){
			return *this;
		}
		self().drop(1).reverse_each(f);
		f(*self().first());
		return *this;
	}

	constexpr auto
	reverse_each() const{
		return self().to_enum([](auto self, auto y) constexpr{ return self.reverse_each(y); });
	}

	template<typename... Args>
	constexpr auto
	select(Args&&... args) const{
		return self().find_all(std::forward<Args>(args)...);
	}

	template<typename Comp>
	constexpr auto
	sort(Comp&& comp) const{
		if( self().is_empty() ){
			return self().to_a();
		}
		auto first = *self().first();
		auto [left, right] = self().drop(1).partition([&](auto it) constexpr{ return comp(first, it) >= 1; });
		return left.sort(comp) + array_t{}.push(first) + right.sort(comp);
	}

	constexpr auto
	sort() const{
		return self().sort([](auto a, auto b) constexpr{ return a > b; });
	}

	template<typename F>
	constexpr auto
	sort_by(F&& f) const{
		return self().map([&](auto it) constexpr{
			return std::make_pair(f(it), it);
		}).sort([](auto a, auto b) constexpr{
			return a.first > b.first;
		}).map([](auto it) constexpr{
			return it.second;
		});
	}

	constexpr auto
	sort_by() const{
		return self().to_enum([](auto self, auto y) constexpr{ return self.sort_by(y); });
	}
	
	constexpr auto
	take(std::size_t n) const{
		return self().find_all().with_index([&](auto, std::size_t i) constexpr{
			return i < n;
		});
	}

	template<typename Pred>
	constexpr auto
	take_while(Pred&& pred) const{
		auto index = self().find_index(std::not_fn(std::forward<Pred>(pred)));
		return self().take(index ? *index : self().count());
	}

	constexpr auto
	to_a() const{
		return self().find_all([](auto) constexpr{ return true; });
	}


	template<typename TResult, typename... Arrays>
	static constexpr auto
	zip_impl(TResult&& result, Arrays&&... arrays){
		if((... && arrays.is_empty())){
			return result;
		}
		else {
			return zip_impl(result.push(zubera::tuple{ arrays.first()... }), arrays.drop(1)...);
		}
	}

	template<typename... Args>
	constexpr auto
	zip(Args&&... args) const{
		using result_t = zubera::vector<decltype(zubera::tuple{ self().first(), args.first()... })>;
		return enumerable::zip_impl(result_t{}, self(), std::forward<Args>(args)...);
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

	template<typename D, typename V, template<class> class R>
	constexpr auto
	operator +(enumerable<D, V, R> const& enum_) const{
		return self().concat(enum_);
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
	auto last_index = e.count() - 1;
	os << "[";
	e.each_with_index([&](auto it, std::size_t i){
		os << it << (i == last_index ? "" : ", ");
	});
	os << "]";
	return os;
}

}  // namespace zubera

#include "./enumerator.hpp"
#include "./vector.hpp"

#endif /* ZUBERA_ENUMERABLE */
