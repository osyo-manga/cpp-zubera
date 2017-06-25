#include "../lib/zubera.hpp"
#include <cassert>

#include <string>
#include <vector>
#include <array>


const auto is_even = [](auto it){ return it % 2 == 0; };
const auto is_odd  = [](auto it){ return it % 2 != 0; };

const auto is_over = [](auto x){
	return [x](auto n){ return n >= x; };
};

const auto is_equal = [](auto x){
	return [x](auto n){ return n == x; };
};

const auto plus = [](auto a, auto b){
	return a + b;
};

namespace dynarray{

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

template<typename T, typename U>
auto
operator ==(X<T> const& v, U&& u)
->decltype(v.get() == u){
	return v.get() == u;
}

template<typename T, typename U>
auto
operator ==(zubera::vector<T> const& v, X<U> const& u)
->decltype(v == u.get()){
	return v == u.get();
}


const auto each_with_index = [](auto make){
	using vec_t = std::vector<int>;
	vec_t result_indices;
	vec_t result_values;
	make(5, 4, 3, 2, 1).each_with_index([&](auto it, auto i){
		result_indices.push_back(i);
		result_values.push_back(it);
	});

	assert((result_indices == vec_t{0, 1, 2, 3, 4}));
	assert((result_values  == vec_t{5, 4, 3, 2, 1}));
};

const auto equal_to = [](auto make){
	auto a = make(1, 2, 3);
	assert(a.equal(a));
	assert(a.equal(v(1, 2, 3)));
	assert((!a.equal(make(1, 2))));
	assert((!a.equal(v(1, 2))));

	assert(a.equal_to(make("1", "2", "3"), [](auto a, auto b){
		return std::to_string(a) == b;
	}));
};

const auto inject = [](auto make){
	assert(make(1, 2, 3).inject(0, plus) == 6);
};

const auto count = [](auto make){
	assert(X<int>{}.count() == 0);

	assert(make(1, 2, 3).count() == 3);
	assert(make(1, 2, 3, 4, 5).count_if(is_over(3)) == 3);
	assert(make(1, 1, 2, 2, 2, 3).count_if(is_equal(2)) == 3);
	assert(make(1, 1, 2, 2, 2, 3).count(2) == 3);
	assert(make("homu", "homu", "mami").count(std::string("homu")) == 2);
};

const auto select = [](auto make){
	assert(make(1, 2, 3, 4, 5, 6).select(is_even) == make(2, 4, 6));
	assert(make(1, 2, 3, 4, 5, 6).select(is_odd)  == make(1, 3, 5));
};

const auto map = [](auto make){
	using namespace std::literals::string_literals;
	auto twice = [](auto it){ return it + it; };
	auto to_string = [](auto it){ return std::to_string(it); };

	assert(make(1, 2, 3).map(twice) == make(2, 4, 6));
	assert(make(1, 2, 3).map(to_string) == make("1"s, "2"s, "3"s));
};


template<typename... Makers>
void
test(Makers... makers){
// 	auto dummy = [](...){};
	auto call  = [&](auto f){ (void)std::initializer_list<int>{ (f(makers), 0)...}; };

	call(inject);
	call(each_with_index);
	call(equal_to);
	call(count);
	call(select);
	call(map);
}

}  // namespace dynarray{



int
main(){
	auto make_x = [](auto x, auto... xs){ return dynarray::X<decltype(x)>{ x, xs... }; };
	auto make_vector = [](auto x, auto... xs){ return zubera::vector<decltype(x)>{ x, xs... }; };
	auto make_tuple = [](auto... xs){ return zubera::make_tuple(xs...); };

	dynarray::test(make_x, make_vector);

	auto xs = dynarray::v(1, 2, 3);

// 	std::cout << xs << std::endl;
// 	std::cout << xs.inject(0, [](auto sum, auto n){ return sum + n; }) << std::endl;
// 	std::cout << xs.map([](auto it){ return "homu:" + std::to_string(it); }) << std::endl;


	dynarray::inject(make_tuple);
	dynarray::count(make_tuple);

// 	static_assert([](auto sum, auto n) constexpr { return sum + n; }(1, 2) == 3, "");
	constexpr auto result = zubera::make_tuple(1, 2, 3).inject(0, [](auto sum, auto n) constexpr { return sum + n; });
	static_assert(result == 6, "");

	constexpr auto t = zubera::make_tuple(42, 3.14f, "homu");

	t.each([](auto x){
		std::cout << x << std::endl;
	});

	t.each_with_index([](auto x, auto i){
		std::cout << i << ":" << x << std::endl;
	});

	return 0;
}
