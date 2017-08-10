#include "../lib/zubera.hpp"
#include <iostream>
#include <string>
#include <vector>


template<typename T>
void
print(T&& t){
	for(auto&& it : t){
		std::cout << it << " ";
	}
	std::cout << std::endl;
}


// X is enumerable
// extend zubera::enumerable<Derived, ValueType>
struct X : zubera::enumerable<X, int>{

	// enumerable needs each function;
	template<typename F>
	void
	each(F&& f) const{
		for(auto&& it : data){
			f(it);
		}
	}

	std::vector<int> data = {1, 2, 3, 4, 5};
};



int
main(){
	X x;

	// data to strings.
	// zubera::vector is enumerable std::vector.
	zubera::vector<std::string> strs = x.map([](auto it){
		return "value:" + std::to_string(it);
	});

	// each loop
	strs.each([](auto str){
		std::cout << str << std::endl;
	});
	// output: [value:1, value:2, value:3, value:4, value:5]


	// zubera::vector to std::vector
	std::vector<std::string> v_strs = strs;
	for(auto&& str : v_strs){
		std::cout << str << std::endl;
	}


	// filtering by data.
	zubera::vector<int> evens = x.select([](auto it){
		return it % 2 == 0;
	});
	print(evens);
	// output: 2 4


	// method chain.
	zubera::vector<std::string> twice_value_string = x.select([](auto it){
		return it % 2 == 0;
	}).map([](auto it){
		return it + it;
	}).map([](auto it){
		return "value:" + std::to_string(it);
	});
	print(twice_value_string);
	// output: value:4 value:8

	return 0;
}
