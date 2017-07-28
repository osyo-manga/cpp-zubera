#include "../lib/zubera.hpp"


int
main(){
	auto range1_5 = zubera::make_enumerator<int>([](auto y){
		y << 1;
		y << 2;
		y << 3;
		y << 4;
		y << 5;
	});

	range1_5.each([](auto it){
		std::cout << it << ",";
	});
	// output: 1,2,3,4,5,
	std::cout << std::endl;

	std::cout << range1_5.map([](auto it){ return it + it; }) << std::endl;
	// output: [2, 4, 6, 8, 10]

	std::cout << range1_5.select([](auto it){ return it % 2 == 0; }) << std::endl;
	// output: [2, 4]
	
	return 0;
}
