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

template<
	typename T,
	typename Each
>
struct enumerator_ : zubera::enumerable<enumerator_<T, Each>, T>{
	enumerator_(Each each) : each_(each){}

	template<typename F>
	constexpr auto
	each(F f) const{
		return each_(f);
	}
	
	Each each_;
};

template<typename T, typename Each>
constexpr enumerator_<T, Each>
enumerator(Each each){
	return { each };
}

int
main(){
	static_::test();
	using namespace test;
	using namespace std::literals::string_literals;
	auto make_v = test::overloaded{
		[](auto x, auto... xs){ return zubera::vector<decltype(x)>{ x, xs... }; },
		[](){ return zubera::vector<int>{}; }
	};

// 	auto v = make_v(1, 2, 3, 4, 5);

	auto enum_ = enumerator<int>([&](auto y){
		make_v(1, 2, 3, 4, 5, 6, 7, 8, 9, 10).each([&](auto it){
			if( it % 2 == 0 ){
				y(it);
			}
		});
	});

	enum_.each([](auto it){
		std::cout << it << std::endl;
	});


	return 0;
}
