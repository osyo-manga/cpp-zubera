#ifndef HELPER_H
#define HELPER_H

#include "./catch/catch.hpp"

namespace zubera{

template<typename T>
std::ostream&
operator <<(std::ostream& os, std::optional<T> const& op){
	if( op ){
		return os << *op;
	}
	else {
		return os << "nil";
	}
}

}
using zubera::operator <<;


#include "../lib/zubera.hpp"

#define CHECK_TYPE(a, b) CHECK_TYPE(std::is_same<decltype(a), decltype(b)>{});


namespace test{


constexpr auto is_even = [](auto it) constexpr { return it % 2 == 0; };
constexpr auto is_odd  = [](auto it) constexpr { return it % 2 != 0; };

constexpr auto is_over = [](auto x) constexpr {
	return [x](auto n) constexpr { return n >= x; };
};

constexpr auto equal_to = [](auto x) constexpr {
	return [x](auto it) constexpr { return x == it; };
};

constexpr auto is_under = [](auto x) constexpr {
	return [x](auto n) constexpr { return n <= x; };
};

constexpr auto is_equal = [](auto x) constexpr {
	return [x](auto n) constexpr { return n == x; };
};

constexpr auto plus = [](auto a, auto b) constexpr {
	return a + b;
};

constexpr auto twice = [](auto it) constexpr { return it + it; };
constexpr auto to_s  = [](auto it) constexpr { return std::to_string(it); };

constexpr struct p_{
	template<typename T>
	std::decay_t<T>
	operator <<(T&& t) const{
		std::cout << t;
		return t;
	}
} p;


template<typename T>
struct X : zubera::enumerable<X<T>, T>{
	X() = default;
	X(X const&) = default;

	X(std::initializer_list<T> vec) : value_(vec){}
	X(std::vector<T> const& vec) : value_(vec){}

	auto
	get() const{
		return value_;
	}

	template<typename F>
	auto
	each(F f) const{
		for(auto n : value_){
			f(n);
		}
		return *this;
	}

	auto
	push(T const& new_value) const{
		std::vector<T> v = value_;
		v.push_back(new_value);
		return X{ v };
	}

	template<typename U>
	bool
	operator ==(X<U> const& rhs) const{
		return get() == rhs.get();
	}
private:
	std::vector<T> value_;
};

template<typename T, typename ...Args>
X<T>
// zubera::vector<T>
v(T t, Args... args){
	return { t, args... };
}


template<typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

}  // namespace test


#endif /* HELPER */
