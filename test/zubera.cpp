#include "../lib/zubera.hpp"
#include "./helper.hpp"
#include <cassert>

#include <string>
#include <vector>
#include <array>


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


template<typename T, typename U>
constexpr bool
type_of(U&&){
	return std::is_same<T, std::decay_t<U>>{};
}


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


const auto is_all = [](auto make){
	
};

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
	assert(!a.equal(make(1, 2)));
	assert(!a.equal(v(1, 2)));

	assert(a == a);
	assert(a == v(1, 2, 3));
	assert(a != v(1, 2));
	assert(a != v(1, 2, 4));

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


namespace static_{

void
test(){
	constexpr auto t = [](auto... xs) constexpr { return zubera::tuple(xs...); };

	static_assert(t(1, 3.14f, 'c') == t(1, 3.14f, 'c'), "");
	static_assert(t(1, 3.14f, 'c') != t(1, 3.14f, 'd'), "");
	static_assert(t(1, 2, 3).inject(0, plus) == 6, "");
	static_assert(t(1, 2.5f, 3.5, 2.5f).inject(0.0, plus) == 9.5, "");
	static_assert(t(1, 2.5f, 3.5).map(twice) == t(2, 5.0f, 7.0), "");

	using namespace std::literals::string_literals;
	assert((t(1, 2.5f, 3.5).map(to_s) == t("1"s, "2.500000"s, "3.500000"s)));
}

}  // namespace static_

int
main(){
	auto make_x = [](auto x, auto... xs){ return dynarray::X<decltype(x)>{ x, xs... }; };
	auto make_vector = [](auto x, auto... xs){ return zubera::vector<decltype(x)>{ x, xs... }; };
	auto make_tuple = [](auto... xs){ return zubera::tuple{xs...}; };

	dynarray::test(make_x, make_vector, make_tuple);

	static_::test();


	constexpr auto t = zubera::tuple(42, 3.14f, "homu");
	

	t.each([](auto x){
		std::cout << x << std::endl;
	});

// 	t.each_with_index([](auto x, auto i){
// 		std::cout << i << ":" << x << std::endl;
// 	});

	return 0;
}
