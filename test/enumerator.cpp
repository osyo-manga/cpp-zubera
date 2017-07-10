#define CATCH_CONFIG_MAIN
#include "./helper.hpp"
#include <vector>


TEST_CASE("zubera::enumerator", "[zubera][enumerator]"){
	using namespace test;
	using namespace std::literals::string_literals;

	auto range0_5 = zubera::make_enumerator<int>([](auto y){
		for(int i = 0 ; i < 5 ; ++i){
			y.yield(i + i);
		}
	});

	SECTION("make_enumerator"){
		auto enum_ = zubera::make_enumerator<int>([](auto y){
			for(int i = 0 ; i < 5 ; ++i){
				y.yield(i + i);
			}
		});
		CHECK((enum_.to_a() == zubera::vector{0, 2, 4, 6, 8}));
	}

	SECTION("with_index"){
		std::vector<int> indices;
		range0_5.with_index([&](auto, auto i){
			indices.push_back(i);
		});
		CHECK((indices == zubera::vector{0, 1, 2, 3, 4}));

		SECTION("with args"){
			std::vector<int> indices;
			range0_5.with_index(-3, [&](auto, auto i){
				indices.push_back(i);
			});
			CHECK((indices == zubera::vector{-3, -2, -1, 0, 1}));
		}
	}

	SECTION("with_object"){
		std::vector<std::string> result;
		range0_5.with_object("homu"s, [&](auto, auto obj){
			result.push_back(obj);
		});
		CHECK((result == zubera::vector{"homu"s, "homu"s, "homu"s, "homu"s, "homu"s}));
	}
}
