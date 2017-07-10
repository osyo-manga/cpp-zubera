#define CATCH_CONFIG_MAIN
#include "./helper.hpp"
#include <vector>


template<typename Maker>
void
test_enumerable_functions(Maker make){
	using namespace test;
	using namespace std::literals::string_literals;
	auto to_string = [](auto it){ return std::to_string(it); };

	SECTION("all_of"){
		CHECK(make().all_of(is_under(3)));
		CHECK(make(1, 2, 3).all_of(is_under(3)));
		CHECK(make(false, false, false).all_of([](auto...){ return true; }));
		CHECK_FALSE(make(1, 2, 3, 4).all_of(is_under(3)));

		CHECK(make(1, 2, 3, 4).all_of());
		CHECK(make(true).all_of());
		CHECK(make().all_of());
		CHECK(make("test").all_of());
		CHECK(make("").all_of());
		CHECK_FALSE(make(false, true).all_of());
	}

	SECTION("any_of"){
		CHECK(make(1, 2, 3).any_of(equal_to(1)));
		CHECK(make(1, 2, 3).any_of(equal_to(2)));
		CHECK(make(1, 2, 3).any_of(equal_to(3)));
		CHECK(make(1, 2, 3).any_of(is_over(0)));
		CHECK_FALSE(make().any_of(equal_to(1)));
		CHECK_FALSE(make().any_of([](auto...){ return false; }));
		CHECK_FALSE(make(1, 2, 3).any_of(equal_to(4)));

		CHECK(make(1, 2, 3).any_of());
		CHECK(make(1, false, 3).any_of());
		CHECK_FALSE(make().any_of());
		CHECK_FALSE(make(false, false, false).any_of());
	}

	SECTION("collect"){
		CHECK(make(1, 2, 3).collect(twice) == make(2, 4, 6));
		CHECK(make(1, 2, 3).collect(to_string) == make("1"s, "2"s, "3"s));
	}

	SECTION("count"){
		CHECK(make().count() == 0);

		CHECK(make(1, 2, 3).count() == 3);
		CHECK(make(1, 2, 3, 4, 5).count_if(is_over(3)) == 3);
		CHECK(make(1, 1, 2, 2, 2, 3).count_if(is_equal(2)) == 3);
		CHECK(make(1, 1, 2, 2, 2, 3).count(2) == 3);
		CHECK(make("homu", "homu", "mami").count("homu"s) == 2);
	}

	SECTION("cycle"){
		std::vector<int> result;
		make(1, 2, 3).cycle(3, [&result](auto it){
			result.push_back(it);
		});
		CHECK((result == std::vector<int>{1, 2, 3, 1, 2, 3, 1, 2, 3}));

		CHECK(make(1, 2).cycle(3) == make(1, 2, 1, 2, 1, 2));
		CHECK(make(1, 2).cycle(0) == make());
	}

	SECTION("detect"){
		auto v = make(5, 3, 1, 4, 2);
		CHECK( v.detect(is_over(3)));
		CHECK(*v.detect(is_over(3)) == 5);
		CHECK(*v.detect(is_even) == 4);
		CHECK_FALSE(v.detect(is_over(6)));
		CHECK_FALSE(make().detect(is_over(6)));
	}

	SECTION("drop"){
		auto v = make(1, 2, 3, 4, 5);
		CHECK(v.drop(0) == make(1, 2, 3, 4, 5));
		CHECK(v.drop(3) == make(4, 5));
		CHECK(v.drop(10) == make());
	}

	SECTION("each"){
		using vec_t = std::vector<int>;
		vec_t each_result;
		make(1, 2, 3, 4, 5).each([&](auto n){
			each_result.push_back(n);
		});
		CHECK((each_result == vec_t{1, 2, 3, 4, 5}));
	}

	SECTION("each_with_index"){
		using vec_t = std::vector<int>;
		vec_t result_indices;
		vec_t result_values;

		make(5, 4, 3, 2, 1).each_with_index([&](auto it, auto i){
			result_indices.push_back(i);
			result_values.push_back(it);
		});

		CHECK((result_indices == vec_t{0, 1, 2, 3, 4}));
		CHECK((result_values  == vec_t{5, 4, 3, 2, 1}));
	}

	SECTION("find"){
		auto v = make(5, 3, 1, 4, 2);
		CHECK( v.find(is_over(3)));
		CHECK(*v.find(is_over(3)) == 5);
		CHECK(*v.find(is_even) == 4);
		CHECK_FALSE(v.find(is_over(6)));
		CHECK_FALSE(make().find(is_over(6)));
	}

	SECTION("inject"){
		CHECK((make(1, 2, 3).inject(0, plus) == 6));
		CHECK((make('m', 'a', 'd', 'o').inject("homu"s, plus) == "homumado"));

		auto value = 42;
		CHECK((make(1, 2, 3).inject(value, plus) == 48));
		CHECK(value == 42);
		
		CHECK(make(1, 2, 3).inject(plus) == 6);
	}

	SECTION("map"){
		CHECK(make(1, 2, 3).map(twice) == make(2, 4, 6));
		CHECK(make(1, 2, 3).map(to_string) == make("1"s, "2"s, "3"s));
		CHECK(make(1, 2, 3, 4).map().select(is_even) == make(2, 4));

		auto map = make(1, 2, 3).map();
		CHECK((map.to_a() == make(1, 2, 3)));
	}

	SECTION("select"){
		CHECK(make(1, 2, 3, 4, 5, 6).select(is_even) == make(2, 4, 6));
		CHECK(make(1, 2, 3, 4, 5, 6).select(is_odd)  == make(1, 3, 5));
		CHECK(make(1, 2, 3, 4, 5, 6).select(is_under(0)) == make());
		CHECK(make(1, 2, 3, 4, 5, 6).select(is_under(0)).is_empty());
		CHECK(make().select(is_under(0)).is_empty());

		auto select = make(1, 2, 3).select();
		CHECK((select.to_a() == make(1, 2, 3)));
	}

	SECTION("take"){
		auto v = make(1, 2, 3, 4, 5);
		CHECK(v.take(3) == make(1, 2, 3));
		CHECK(v.take(10) == make(1, 2, 3, 4, 5));
	}


	SECTION("concat"){
		CHECK(make(1, 2).concat(make(3, 4)) == make(1, 2, 3, 4));
		CHECK(make(1, 2).concat(make()) == make(1, 2));

		// zubera::tuple is not supported.
// 		CHECK(make().concat(make(3, 4)) == make(3, 4));
	}
}


TEST_CASE("zubera::enumerable", "[zubera][enumerable]"){
	auto make_x = test::overloaded{
		[](auto x, auto... xs){ return test::X<decltype(x)>{ x, xs... }; },
		[](){ return test::X<int>{}; }
	};
	auto make_vector = test::overloaded{
		[](auto x, auto... xs){ return zubera::vector<decltype(x)>{ x, xs... }; },
		[](){ return zubera::vector<int>{}; }
	};
	auto make_tuple = [](auto... xs){ return zubera::tuple{xs...}; };

	test_enumerable_functions(make_x);
	test_enumerable_functions(make_vector);
	test_enumerable_functions(make_tuple);
}
