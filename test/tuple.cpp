#define CATCH_CONFIG_MAIN
#include "./helper.hpp"

TEST_CASE("zubera::tuple", "[zubera][enumerable][tuple]"){
	using namespace test;
	using zubera::tuple;

	constexpr auto equal_to_variant = [](auto var, auto value) constexpr {
		bool result = false;
		std::visit([&](auto it){
			result = it == value;
		}, var);
		return result;
	};

	auto is_int = test::overloaded{
		[](int){ return true; },
		[](auto...){ return false; }
	};

	SECTION("all_of"){
		static_assert(tuple(1, 2, 3).all_of(is_int), "");
		static_assert(!tuple(1, 2, 3.0f).all_of(is_int), "");
	}

	SECTION("select"){
		auto t = tuple(1, 'c', 3.14).concat(tuple(1, 'c'));
		assert(t.equal_to(tuple(1, 'c', 3.14, 1, 'c'), equal_to_variant));
	}
}
