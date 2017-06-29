#define CATCH_CONFIG_MAIN
#include "./helper.hpp"


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


constexpr auto is_even = [](auto it) constexpr { return it % 2 == 0; };
constexpr auto is_odd  = [](auto it) constexpr { return it % 2 != 0; };

constexpr auto is_over = [](auto x) constexpr {
	return [x](auto n) constexpr { return n >= x; };
};

constexpr auto is_equal = [](auto x) constexpr {
	return [x](auto n) constexpr { return n == x; };
};

constexpr auto plus = [](auto a, auto b) constexpr {
	return a + b;
};

constexpr auto twice = [](auto it) constexpr { return it + it; };
constexpr auto to_s  = [](auto it) constexpr { return std::to_string(it); };


template<typename Maker>
void
test_enumerable(Maker make){
	SECTION("each_with_index"){
		using vec_t = std::vector<int>;
		vec_t result_indices;
		vec_t result_values;

		make(5, 4, 3, 2, 1).each_with_index([&](auto it, auto i){
			result_indices.push_back(i);
			result_values.push_back(it);
		});

		REQUIRE((result_indices == vec_t{0, 1, 2, 3, 4}));
		REQUIRE((result_values  == vec_t{5, 4, 3, 2, 1}));
	}
}



TEST_CASE("zubera::enumerable", "[zubera][enumerable]"){
	auto make_x = [](auto x, auto... xs){ return X<decltype(x)>{ x, xs... }; };
	auto make_vector = [](auto x, auto... xs){ return zubera::vector<decltype(x)>{ x, xs... }; };
	auto make_tuple = [](auto... xs){ return zubera::tuple{xs...}; };

	test_enumerable(make_x);
	test_enumerable(make_vector);
	test_enumerable(make_tuple);
}
