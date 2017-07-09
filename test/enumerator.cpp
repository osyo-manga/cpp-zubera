#define CATCH_CONFIG_MAIN
#include "./helper.hpp"
#include <vector>


TEST_CASE("zubera::enumerator", "[zubera][enumerator]"){
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
		auto enum_ = zubera::make_enumerator<int>([](auto y){
			for(int i = 0 ; i < 5 ; ++i){
				y.yield(i + i + i);
			}
		});
		enum_.with_index([&](auto it , auto i){
			indices.push_back(i);
		});
		CHECK((indices == zubera::vector{0, 1, 2, 3, 4}));

		SECTION("with args"){
			std::vector<int> indices;
			auto enum_ = zubera::make_enumerator<int>([](auto y){
				for(int i = 0 ; i < 5 ; ++i){
					y.yield(i + i + i);
				}
			});
			enum_.with_index(-3, [&](auto it , auto i){
				indices.push_back(i);
			});
			CHECK((indices == zubera::vector{-3, -2, -1, 0, 1}));
		}
	}
}
