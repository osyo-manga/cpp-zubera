#ifndef HELPER_H
#define HELPER_H

#include "./catch/catch.hpp"
#include "../lib/zubera.hpp"


namespace test{


constexpr auto is_even = [](auto it) constexpr { return it % 2 == 0; };
constexpr auto is_odd  = [](auto it) constexpr { return it % 2 != 0; };

constexpr auto is_over = [](auto x) constexpr {
	return [x](auto n) constexpr { return n >= x; };
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
