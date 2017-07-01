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
	using namespace test;
	using namespace std::literals::string_literals;

	{
		auto make = test::overloaded{
			[](auto x, auto... xs){ return zubera::vector<decltype(x)>{ x, xs... }; },
			[](){ return zubera::vector<int>{}; }
		};
		
		auto result = make(1, 2, 3).find(equal_to(-1));
		std::cout << *result << std::endl;
	}

	auto make = [](auto... xs){ return zubera::tuple{xs...}; };

// 	std::cout << make().all_of(is_under(3));
// 	std::cout << zubera::vector<int>{}.all_of(is_under(3));

// 	make().concat(make(3, 4)) == make(3, 4);
	return 0;
}
