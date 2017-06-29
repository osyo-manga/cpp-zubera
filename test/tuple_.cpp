#define CATCH_CONFIG_MAIN
#include "./helper.hpp"

TEST_CASE("zubera::tuple", "[zubera][enumerable][tuple]"){
	using namespace test;

	auto is_int = test::overloaded{
		[](int){ return true; },
		[](auto...){ return false; }
	};

	SECTION("all_of"){
		static_assert(zubera::tuple(1, 2, 3).all_of(is_int), "");
		static_assert(!zubera::tuple(1, 2, 3.0f).all_of(is_int), "");
	}
}
