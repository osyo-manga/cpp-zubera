#define CATCH_CONFIG_MAIN
#include "./helper.hpp"


template<typename T>
struct type_{
	template<typename U>
	constexpr bool
	operator ==(U&&) const{
		return std::is_same_v<T, std::decay_t<U>>;
	}
};

template<typename T>
constexpr type_<T> type{};


TEST_CASE("zubera::tuple", "[zubera][enumerable][tuple]"){
	using namespace std::literals::string_literals;
	using namespace test;
	using zubera::tuple;

// 	constexpr auto equal_to_variant = [](auto var, auto value) constexpr {
// 		bool result = false;
// 		std::visit([&](auto it){
// 			result = (it == value);
// 		}, var);
// 		return result;
// 	};

	auto is_int = test::overloaded{
		[](int){ return true; },
		[](auto...){ return false; }
	};

	SECTION("all_of"){
		static_assert(tuple(1, 2, 3).all_of(is_int), "");
		static_assert(!tuple(1, 2, 3.0f).all_of(is_int), "");
	}

	SECTION("select"){
// 		auto t = tuple(1, 'c', 3.14).concat(tuple(1, 'c'));
		CHECK((zubera::tuple{1, 2}.select(is_over(3)).is_empty()));
// 		CHECK(t.equal_to(tuple(1, 'c', 3.14, 1, 'c'), equal_to_variant));
// 		CHECK(type<zubera::vector<std::any>> == zubera::tuple{}.select(is_over(3)));
	}

	SECTION("find_index"){
		constexpr auto result = tuple{3, 1, 6, 4}.find_index(is_even);
		static_assert(*result == 2, "");
	}

	SECTION("map"){
		CHECK((zubera::tuple{1, 2, 3}.map([](auto) constexpr{
			return "homu"s;
		}) == zubera::vector{"homu"s, "homu"s, "homu"s}));
	}
}
