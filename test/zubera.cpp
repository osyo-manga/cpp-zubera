#include "../lib/zubera.hpp"
#include "./helper.hpp"
#include <cassert>

#include <string>
#include <vector>
#include <array>

namespace static_{

void
test(){
	constexpr auto t = [](auto... xs) constexpr { return zubera::tuple(xs...); };

	static_assert(t(1, 3.14f, 'c') == t(1, 3.14f, 'c'), "");
	static_assert(t(1, 3.14f, 'c') != t(1, 3.14f, 'd'), "");

	static_assert(t(1, 2, 3).inject(0, test::plus) == 6, "");
	static_assert(t(1, 2.5f, 3.5, 2.5f).inject(0.0, test::plus) == 9.5, "");
	static_assert(t(1, 2.5f, 3.5).map(test::twice) == t(2, 5.0f, 7.0), "");
//
// 	using namespace std::literals::string_literals;
// 	assert((t(1, 2.5f, 3.5).map(to_s) == t("1"s, "2.500000"s, "3.500000"s)));
}

}  // namespace static_

int
main(){
	static_::test();
		using namespace std::literals::string_literals;

	auto make = [](auto... xs){ return zubera::tuple{xs...}; };

	auto to_string = [](auto it){ return std::to_string(it); };
	make(1, 2, 3).map(to_string) == make("1"s, "2"s, "3"s);

	auto t = zubera::tuple{};

	make(1, 2, 3, 4, 5, 6).select(test::is_odd)  == make(1, 3, 5);
// 	constexpr auto t = zubera::tuple(42, 3.14f, "homu");
//
//
// 	t.each([](auto x){
// 		std::cout << x << std::endl;
// 	});

// 	t.each_with_index([](auto x, auto i){
// 		std::cout << i << ":" << x << std::endl;
// 	});

	return 0;
}
