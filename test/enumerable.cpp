#define CATCH_CONFIG_MAIN
#include "./helper.hpp"
#include <vector>


template<typename Maker>
void
test_enumerable_functions(Maker make){
	using namespace test;

	SECTION("count"){
		REQUIRE(test::X<int>{}.count() == 0);

		REQUIRE(make(1, 2, 3).count() == 3);
		REQUIRE(make(1, 2, 3, 4, 5).count_if(is_over(3)) == 3);
		REQUIRE(make(1, 1, 2, 2, 2, 3).count_if(is_equal(2)) == 3);
		REQUIRE(make(1, 1, 2, 2, 2, 3).count(2) == 3);
		REQUIRE(make("homu", "homu", "mami").count(std::string("homu")) == 2);
	}

	SECTION("each"){
		using vec_t = std::vector<int>;
		vec_t each_result;
		make(1, 2, 3, 4, 5).each([&](auto n){
			each_result.push_back(n);
		});
		REQUIRE((each_result == vec_t{1, 2, 3, 4, 5}));
	}

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

	SECTION("inject"){
		REQUIRE((make(1, 2, 3).inject(0, plus) == 6));
	}

	SECTION("map"){
		using namespace std::literals::string_literals;
		auto to_string = [](auto it){ return std::to_string(it); };

		REQUIRE(make(1, 2, 3).map(twice) == make(2, 4, 6));
		REQUIRE(make(1, 2, 3).map(to_string) == make("1"s, "2"s, "3"s));
	}

	SECTION("select"){
		REQUIRE(make(1, 2, 3, 4, 5, 6).select(is_even) == make(2, 4, 6));
		REQUIRE(make(1, 2, 3, 4, 5, 6).select(is_odd)  == make(1, 3, 5));
	}
}


TEST_CASE("zubera::enumerable", "[zubera][enumerable]"){
	auto make_x = [](auto x, auto... xs){ return test::X<decltype(x)>{ x, xs... }; };
	auto make_vector = [](auto x, auto... xs){ return zubera::vector<decltype(x)>{ x, xs... }; };
	auto make_tuple = [](auto... xs){ return zubera::tuple{xs...}; };

	test_enumerable_functions(make_x);
	test_enumerable_functions(make_vector);
	test_enumerable_functions(make_tuple);
}
