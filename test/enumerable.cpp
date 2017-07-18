#define CATCH_CONFIG_MAIN
#include "./helper.hpp"
#include <vector>


template<typename Maker, typename Range>
void
test_enumerable_functions(Maker make, Range range1_5){
	using namespace test;
	using namespace std::literals::string_literals;
	auto to_string = [](auto it){ return std::to_string(it); };

	SECTION("all_of"){
		CHECK(make().all_of(is_under(3)));
		CHECK(make(false, false, false).all_of([](auto...){ return true; }));
		CHECK_FALSE(make(1, 2, 3, 4).all_of(is_under(3)));

		CHECK(make(1, 2, 3, 4).all_of());
		CHECK(make(true).all_of());
		CHECK(make().all_of());
		CHECK(make("test").all_of());
		CHECK(make("").all_of());
		CHECK_FALSE(make(false, true).all_of());

		CHECK(range1_5.all_of());
		CHECK(range1_5.all_of(is_under(5)));
	}

	SECTION("any_of"){
		CHECK(range1_5.any_of(equal_to(1)));
		CHECK(range1_5.any_of(equal_to(2)));
		CHECK(range1_5.any_of(equal_to(3)));
		CHECK(range1_5.any_of(is_over(0)));

		CHECK_FALSE(make().any_of(equal_to(1)));
		CHECK_FALSE(make().any_of([](auto...){ return false; }));
		CHECK_FALSE(range1_5.any_of(equal_to(6)));

		CHECK(range1_5.any_of());
		CHECK(make(1, 0, 3).any_of());
		CHECK_FALSE(make().any_of());
		CHECK_FALSE(make(false, false, false).any_of());
	}

	SECTION("collect"){
		CHECK(range1_5.collect(twice) == make(2, 4, 6, 8, 10));
		CHECK(range1_5.collect(to_string) == make("1"s, "2"s, "3"s, "4"s, "5"s));
	}

	SECTION("count"){
		CHECK(make().count() == 0);

		CHECK(range1_5.count() == 5);
		CHECK(range1_5.count_if(is_over(3)) == 3);
		CHECK(make(1, 1, 2, 2, 2, 3).count_if(is_equal(2)) == 3);
		CHECK(make(1, 1, 2, 2, 2, 3).count(2) == 3);
		CHECK(make("homu", "homu", "mami").count("homu"s) == 2);
	}

	SECTION("cycle"){
		std::vector<int> result;
		range1_5.cycle(3, [&result](auto it){
			result.push_back(it);
		});
		CHECK((result == std::vector<int>{1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5}));

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
		CHECK(range1_5.drop(0) == make(1, 2, 3, 4, 5));
		CHECK(range1_5.drop(3) == make(4, 5));
		CHECK(range1_5.drop(10) == make());
	}

	SECTION("drop_while"){
		auto v = make(3, 1, 5, 4, 9, 7, 8, 2);
		CHECK(v.drop_while(is_odd) == make(4, 9, 7, 8, 2));
		CHECK(v.drop_while(is_under(8)) == make(9, 7, 8, 2));
		CHECK(v.drop_while(is_equal(3)) == make(1, 5, 4, 9, 7, 8, 2));
		CHECK(v.drop_while(is_under(9999)) == make());
	}

	SECTION("each"){
		using vec_t = std::vector<int>;
		vec_t each_result;
		range1_5.each([&](auto n){
			each_result.push_back(n);
		});
		CHECK((each_result == vec_t{1, 2, 3, 4, 5}));
	}

	SECTION("each_cons"){
		using result_t = std::vector<std::vector<int>>;
		result_t result{};
		range1_5.each_cons(2, [&](auto it){
			result.push_back(it);
		});
		CHECK((result == result_t{ {1, 2}, {2, 3}, {3, 4}, {4, 5} }));

		CHECK((make(1, 2, 3).each_cons(1) == make( make(1), make(2), make(3) )));
		CHECK((make(1, 2, 3, 4).each_cons(2) == make( make(1, 2), make(2, 3), make(3, 4) )));
		CHECK((make(1, 2, 3, 4).each_cons(3) == make(make(1, 2, 3), make(2, 3, 4))));
		CHECK((make(1, 2, 3).each_cons(100).is_empty()));
		CHECK(make(1, 2, 3).each_cons(3).count() == 1);
	}

	SECTION("each_slice"){
		using result_t = std::vector<std::vector<int>>;
		result_t result{};
		range1_5.each_slice(2, [&](auto it){
			result.push_back(it);
		});
		CHECK((result == result_t{ {1, 2}, {3, 4}, {5} }));
		CHECK((range1_5.each_slice(3).to_a() == make(make(1, 2, 3), make(4, 5))));
		CHECK(make(1, 2, 3).each_slice(300).count() == 1);
		CHECK(make(1, 2, 3).each_slice(3).count() == 1);
	}

	SECTION("each_with_index"){
		using vec_t = std::vector<int>;
		vec_t result_indices;
		vec_t result_values;

		range1_5.each_with_index([&](auto it, auto i){
			result_indices.push_back(i);
			result_values.push_back(it);
		});

		CHECK((result_indices == vec_t{0, 1, 2, 3, 4}));
		CHECK((result_values  == vec_t{1, 2, 3, 4, 5}));
	}

	SECTION("each_with_object"){
		using vec_t = std::vector<int>;
		vec_t result_values;

		range1_5.each_with_object(5).with_index([&](auto, auto obj, auto i){
			result_values.push_back(obj + i);
		});
		CHECK((result_values  == vec_t{5, 6, 7, 8, 9}));

		auto value = 5;
		auto sum = range1_5.each_with_object(value, [](auto it, auto& obj){
			obj += it;
		});
		CHECK(sum == 20);
		CHECK(value == 20);
	}

	SECTION("find"){
		auto v = make(5, 3, 1, 4, 2);
		CHECK( v.find(is_over(3)));
		CHECK(*v.find(is_over(3)) == 5);
		CHECK( v.find(is_under(2)));
		CHECK(*v.find(is_under(2)) == 1);
		CHECK(*v.find(is_even) == 4);
		CHECK_FALSE(v.find(is_over(6)));
		CHECK_FALSE(make().find(is_over(6)));

		std::vector<int> eachs{};
		v.find([&](auto it){
			eachs.push_back(it);
			return it % 2 == 0;
		});
		CHECK((eachs == std::vector<int>{5, 3, 1, 4}));

		CHECK(*make(5, 6, 7, 8, 9).find().with_index([](auto, auto i){
			return i == 3;
		}) == 8);

		int count = 0;
		CHECK(v.find([&](auto it){
			count++;
			return it == 3;
		}));
		CHECK(count == 2);
	}

	SECTION("find_index"){
		auto v = make(5, 3, 1, 4, 2);
		CHECK( v.find_index(is_under(2)));
		CHECK(*v.find_index(is_under(2)) == 2);
		CHECK_FALSE(v.find_index(is_under(-1)));

		std::vector<int> eachs{};
		v.find_index([&](auto it){
			eachs.push_back(it);
			return it % 2 == 0;
		});
		CHECK((eachs == std::vector<int>{5, 3, 1, 4}));
	}

	SECTION("first"){
		CHECK( make(4, 2, 2).first());
		CHECK(*make(4, 2, 2).first() == 4);
		CHECK_FALSE(make().first());

		CHECK(range1_5.first(3) == make(1, 2, 3));
		CHECK(make().first(3).is_empty());
	}

	SECTION("include"){
		CHECK(range1_5.include(2));
		CHECK_FALSE(range1_5.include(0));
	}

	SECTION("inject"){
		CHECK((range1_5.inject(0, plus) == 15));
		CHECK((make('m', 'a', 'd', 'o').inject("homu"s, plus) == "homumado"));

		auto value = 5;
		CHECK((range1_5.inject(value, plus) == 20));
		CHECK(value == 5);
		
		CHECK(range1_5.inject(plus) == 15);
	}

	SECTION("map"){
		CHECK(range1_5.map(twice) == make(2, 4, 6, 8, 10));
		CHECK(make(1, 2, 3).map(to_string) == make("1"s, "2"s, "3"s));
		CHECK(make(1, 2, 3, 4).map().select(is_even) == make(2, 4));

		auto map = make(1, 2, 3).map();
		CHECK((map.to_a() == make(1, 2, 3)));
	}

	SECTION("max"){
		auto rand = make(3, 2, 5, 1, 4, 4);

		CHECK(range1_5.max());
		CHECK(*range1_5.max() == 5);
		CHECK(*range1_5.max([](auto a, auto b){ return a > b ? -1 : 1; }) == 1);
		CHECK(*rand.max() == 5);
		CHECK_FALSE(make().max());

		CHECK(rand.max(2) == make(5, 4));
		CHECK(rand.max(3) == make(5, 4, 4));
		CHECK(rand.max(0) == make());
		CHECK(rand.max(10) == make(5, 4, 4, 3, 2, 1));
		CHECK(rand.max(-1) == make(5, 4, 4, 3, 2, 1));
		CHECK(rand.max(3, [](auto a, auto b){ return a > b ? -1 : 1; }) == make(1, 2, 3));
	}

	SECTION("max_by"){
		auto animals = zubera::vector{"giraffe"s, "mouse"s, "hippopotamus"s, "cat"s};
		int count = 0;
		auto f = [&](auto it){ count++; return it.size(); };
		CHECK(animals.max_by(f));
		CHECK(count == 4);
		CHECK(*animals.max_by(f) == "hippopotamus"s);
		CHECK_FALSE(make().max_by([](auto){ return 0; }));

		count = 0;
		CHECK(animals.max_by(2, f) == make("hippopotamus"s, "giraffe"s));
		CHECK(count == 4);
		CHECK(make().max_by(2, [](auto){ return 0; }).count() == 0);

		auto v = zubera::vector{ 5, 3, 1, 4, 2 };
		CHECK( v.max_by().with_index(std::plus{}));
		CHECK(*v.max_by().with_index(std::plus{}) == 4);
		CHECK(v.max_by(3).with_index(std::plus{}) == make(4, 2, 5));
	}

	SECTION("member"){
		CHECK(range1_5.member(2));
		CHECK_FALSE(range1_5.member(0));
	}

	SECTION("none_of"){
		CHECK(range1_5.none_of(equal_to(0)));
		CHECK_FALSE(range1_5.none_of(equal_to(1)));
		CHECK_FALSE(range1_5.none_of(is_even));

		CHECK(make().none_of());
		CHECK(make(0, 0, 0).none_of());
		CHECK(make(0, false, 0).none_of());
		CHECK_FALSE(range1_5.none_of());
		CHECK_FALSE(make(1, 0).none_of());
	}

	SECTION("one_of"){
		CHECK(range1_5.one_of(equal_to(1)));
		CHECK_FALSE(range1_5.one_of(equal_to(0)));
		CHECK_FALSE(range1_5.one_of(is_even));

		CHECK(make(1, 0, 0).one_of());
		CHECK_FALSE(make(1, 1, 0).one_of());
		CHECK_FALSE(make().one_of());
	}

	SECTION("partition"){
		std::size_t value = 0;
		auto [even, odd] = range1_5.partition([&](auto it){
			value++;
			return it % 2 == 0;
		});
		CHECK(even == make(2, 4));
		CHECK(odd  == make(1, 3, 5));
		CHECK(value == range1_5.count());

		value = 0;
		auto [a, b] = range1_5.partition().with_index([&](auto, auto i){
			value++;
			return i < 3;
		});
		CHECK(a == make(1, 2, 3));
		CHECK(b == make(4, 5));
		CHECK(value == range1_5.count());
	}

	SECTION("reject"){
		CHECK(range1_5.reject(is_odd) == make(2, 4));
		CHECK(make(3, 1, 2, 5).reject().with_index([](auto, auto i){ return is_odd(i); }) == make(3, 2));
	}

	SECTION("reverse_each"){
		using vec_t = std::vector<int>;
		vec_t result{};
		range1_5.reverse_each([&result](auto it){
			result.push_back(it);
		});
		CHECK((result == vec_t{5, 4, 3, 2, 1}));
		
		result.clear();
		vec_t indices;
		range1_5.reverse_each().with_index([&](auto it, auto i){
			result.push_back(it);
			indices.push_back(i);
		});
		CHECK((result  == vec_t{5, 4, 3, 2, 1}));
		CHECK((indices == vec_t{0, 1, 2, 3, 4}));
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

	SECTION("sort"){
		CHECK(make(3, 5, 2, 4, 1).sort() == range1_5);
		CHECK(make(3, 1, 5, 4, 9, 7, 8, 2, 4, 1, -3, 2).sort() == make(-3, 1, 1, 2, 2, 3, 4, 4, 5, 7, 8, 9));
		CHECK(make(3, 5, 2, 4, 1).sort([](auto a, auto b){ return a < b; }) == make(5, 4, 3, 2, 1));

		auto animals = make("cat"s, "mouse"s, "giraffe"s, "hippopotamus"s);
		CHECK(animals.sort() == make("cat"s, "giraffe"s, "hippopotamus"s, "mouse"s));
	}

	SECTION("sort_by"){
		int count = 0;
		auto animals = make("giraffe"s, "mouse"s, "hippopotamus"s, "cat"s);
		CHECK(animals.sort_by([&](auto it){
			count++;
			return it.size();
		}) == make("cat"s, "mouse"s, "giraffe"s, "hippopotamus"s));
		CHECK(count == 4);

		auto v = make(5, 3, 1, 4, 2);
		CHECK(v.sort_by().with_index(std::plus{}) == make(1, 3, 5, 2, 4));
	}

	SECTION("take"){
		CHECK(range1_5.take(3) == make(1, 2, 3));
		CHECK(range1_5.take(10) == make(1, 2, 3, 4, 5));
	}

	SECTION("take_while"){
		auto v = make(3, 1, 5, 4, 9, 7, 8, 2);
		CHECK(v.take_while(is_odd) == make(3, 1, 5));
		CHECK(v.take_while(is_under(8)) == make(3, 1, 5, 4));
		CHECK(v.take_while(is_equal(3)) == make(3));
		CHECK(v.take_while(is_under(0)) == make());
	}

	SECTION("concat"){
		CHECK(range1_5.concat(make(3, 4)) == make(1, 2, 3, 4, 5, 3, 4));
		CHECK(range1_5.concat(make()) == make(1, 2, 3, 4, 5));

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

	test_enumerable_functions(make_x, make_x(1, 2, 3, 4, 5));
	test_enumerable_functions(make_vector, make_vector(1, 2, 3, 4, 5));
	test_enumerable_functions(make_tuple, make_tuple(1, 2, 3, 4, 5));

// 	test_enumerable_function(make_vector, zubera::irange(1, 6));
}
