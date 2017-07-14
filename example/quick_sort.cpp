#include "../lib/zubera.hpp"
#include <iostream>
#include <string>

template<typename T>
constexpr auto
sort(T&& t){
	if( t.is_empty() ){
		return t;
	}
	auto first = *t.first();
	auto [left, right] = t.drop(1).partition([&](auto it) constexpr{ return it < first; });
	return sort(left) + zubera::vector{first} + sort(right);
}


int
main(){
	using namespace std::literals::string_literals;

	auto v = zubera::vector{ 3, 1, 5, 4, 9, 7, 8, 2, 4, 1, -3, 2 };

	std::cout << sort(v) << std::endl;

	return 0;
}
