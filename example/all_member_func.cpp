#include "../lib/zubera.hpp"
#include <string>

template<typename T>
auto
print(T t){
	std::cout << t;
}

template<typename T>
auto
print(std::optional<T> op){
	if( op ){
		std::cout << *op;
	}
	else{
		std::cout << "None";
	}
}


#define OUTPUT(expr) \
	std::cout << #expr << " is "; print(expr); std::cout << "\n";

template<typename F>
void
call(std::string name, F f){
	std::cout << "=== " << name << " ===" << std::endl;
	f();
	std::cout << "\n";
}

int
main(){
	using namespace std::literals;

	auto is_even = [](auto it){ return it % 2 == 0; };
	auto is_odd  = [](auto it){ return it % 2 != 0; };
	auto under   = [](auto x){
		return [x](auto n){ return n <= x; };
	};
	auto twice = [](auto it){
		return it + it;
	};
	auto to_string = [](auto it){
		return std::to_string(it);
	};
	auto equal = [](auto n){
		return [n](auto it){ return n == it; };
	};
	
	auto v = [](auto... args){
		return zubera::vector{ args... };
	};

	call("all_of", [&]{
		OUTPUT(v(2, 4, 6, 8).all_of(is_even));
		// output: 1
		OUTPUT(v(2, 4, 6, 8).all_of(under(6)));
		// output: 0
	});

	call("any_of", [&]{
		OUTPUT(v(1, 2, 3).any_of(is_even));
		// output: 1
		OUTPUT(v(1, 2, 3).any_of(is_odd));
		// output: 1
		OUTPUT(v(1, 3, 5).any_of(is_even));
		// output: 0
	});

	call("collect/map", [&]{
		OUTPUT(v(1, 2, 3).collect(twice));
		// output: [2, 4, 6]
		OUTPUT(v(1, 2, 3).collect(to_string).collect(twice));
		// output: [11, 22, 33]
	});

	call("count/count_if", [&]{
		OUTPUT(v(1, 1, 2, 1, 2, 3).count());
		// output: 6
		OUTPUT(v(1, 1, 2, 1, 2, 3).count(1));
		// output: 3
		OUTPUT(v(1, 1, 2, 1, 2, 3).count(2));
		// output: 2
		OUTPUT(v(1, 1, 2, 1, 2, 3).count_if(is_odd));
		// output: 4
	});

	call("cycle", [&]{
		OUTPUT(v(1, 2).cycle(2));
		// output: [1, 2, 1, 2]
	});

	call("detect/find", [&]{
		OUTPUT(v(5, 3, 2, 1, 4).detect(is_even));
		// output: 2
		OUTPUT(v(5, 3, 2, 1, 4).detect(equal(0)));
		// output: None
	});

	call("drop", [&]{
		OUTPUT(v(5, 3, 2, 1, 4).drop(2));
		// output: [2, 1, 4]
	});

	call("drop_while", [&]{
		OUTPUT(v(5, 3, 7, 8, 2, 1, 4).drop_while(is_odd));
		// output: [8, 2, 1, 4]
	});

	call("each_cons", [&]{
		std::cout << "v(1, 2, 3, 4).each_cons" << std::endl;
		v(1, 2, 3, 4).each_cons(2, [](zubera::vector<int> const& it){
			std::cout << it << ", ";
		});
		// output: [1, 2], [2, 3], [3, 4],
		std::cout << "\n";
	});

	call("each_slice", [&]{
		std::cout << "v(1, 2, 3, 4, 5).each_slice" << std::endl;
		v(1, 2, 3, 4, 5).each_slice(2, [](zubera::vector<int> const& it){
			std::cout << it << ", ";
		});
		// output: [1, 2], [3, 4], [5],
		std::cout << "\n";
	});

	call("each_with_index", [&]{
		std::cout << "v(\"homu\"s, \"mami\"s, \"mado\"s).each_with_index" << std::endl;
		v("homu"s, "mami"s, "mado"s).each_with_index([](auto it, auto i){
			std::cout << "(" << i << " : " << it << "), ";
		});
		// output: (0 : homu), (1 : mami), (2 : mado),
		std::cout << "\n";
	});

	call("each_with_object", [&]{
		std::cout << "v(\"homu\"s, \"mami\"s, \"mado\"s).each_with_object" << std::endl;
		v("homu"s, "mami"s, "mado"s).each_with_object("value: ", [](auto it, auto obj){
			std::cout << obj << it << ", ";
		});
		// output: value: homu, value: mami, value: mado,
		std::cout << "\n";
	});

	call("find_all/select", [&]{
		OUTPUT(v(3, 6, 2, 5, 1, 4).find_all(is_even));
		// output: [6, 2, 4]
	});

	call("find_index", [&]{
		OUTPUT(v("homu"s, "mami"s, "mado"s, "mami"s, "mado"s).find_index(equal("mado")));
		// output: 2
		OUTPUT(v("homu"s, "mami"s, "mado"s, "mami"s, "mado"s).find_index(equal("saya")));
		// output: None
	});

	call("first", [&]{
		OUTPUT(v("homu"s, "mami"s, "mado"s).first());
		// output: homu
		OUTPUT(v("homu"s, "mami"s, "mado"s).first(2));
		// output: [homu, mami]

		OUTPUT(zubera::vector<int>{}.first())
		// output: None
		OUTPUT(zubera::vector<int>{}.first(2))
		// output: []
	});

	call("include/member", [&]{
		OUTPUT(v("homu"s, "mami"s, "mado"s).include("mami"));
		// output: 1
		OUTPUT(v("homu"s, "mami"s, "mado"s).include("saya"));
		// output: 0
	});

	call("inject/reduce", [&]{
		OUTPUT(v(1, 2, 3, 4, 5).inject([](auto sum, auto it){ return sum + it; }));
		// output: 15
		OUTPUT(v(1, 2, 3, 4, 5).inject(10, [](auto sum, auto it){ return sum + it; }));
		// output: 25
	});

	call("max", [&]{
		OUTPUT(v(3, 2, 5, 1, 4).max());
		// output: 5
		OUTPUT(v(3, 2, 5, 1, 4).max(2));
		// output: [5, 4]
		OUTPUT(zubera::vector<int>{}.max());
		// output: None
		OUTPUT(zubera::vector<int>{}.max(2));
		// output: []
	});

	call("max_by", [&]{
		auto size = [](auto it){ return it.size(); };
		OUTPUT(v("giraffe"s, "mouse"s, "hippopotamus"s, "cat"s).max_by(size));
		// output: hippopotamus
		OUTPUT(v("giraffe"s, "mouse"s, "hippopotamus"s, "cat"s).max_by(2, size));
		// output: [hippopotamus, giraffe]
		OUTPUT(zubera::vector<std::string>{}.max_by(size));
		// output: None
		OUTPUT(zubera::vector<std::string>{}.max_by(2, size));
		// output: []
	});

	call("min", [&]{
		OUTPUT(v(3, 2, 5, 1, 4).min());
		// output: 1
		OUTPUT(v(3, 2, 5, 1, 4).min(2));
		// output: [1, 2]
		OUTPUT(zubera::vector<int>{}.min());
		// output: None
		OUTPUT(zubera::vector<int>{}.min(2));
		// output: []
	});

	call("min_by", [&]{
		auto size = [](auto it){ return it.size(); };
		OUTPUT(v("giraffe"s, "mouse"s, "hippopotamus"s, "cat"s).min_by(size));
		// output: cat
		OUTPUT(v("giraffe"s, "mouse"s, "hippopotamus"s, "cat"s).min_by(2, size));
		// output: [cat, mouse]
		OUTPUT(zubera::vector<std::string>{}.min_by(size));
		// output: None
		OUTPUT(zubera::vector<std::string>{}.min_by(2, size));
		// output: []
	});

	call("minmax", [&]{
		OUTPUT(v(3, 2, 5, 1, 4).minmax().map([](std::optional<int> it){ return bool(it); }));
		// output: [1, 1]
		OUTPUT(v(3, 2, 5, 1, 4).minmax().map([](std::optional<int> it){ return *it; }));
		// output: [1, 5]
		OUTPUT(zubera::vector<int>{}.minmax().map([](std::optional<int> it){ return bool(it); }));
		// output: [0, 0]
	});

	call("minmax_by", [&]{
		auto size = [](auto it){ return it.size(); };
		OUTPUT(v("giraffe"s, "mouse"s, "hippopotamus"s, "cat"s).minmax_by(size).map([](std::optional<std::string> it){ return *it; }));
		// output: [cat, hippopotamus]
	});

	call("none_of", [&]{
		OUTPUT(v(3, 1, 5, 7).none_of(is_even));
		// output: 1
		OUTPUT(v(3, 1, 5, 7).none_of(equal(3)));
		// output: 0
	});

	call("one_of", [&]{
		OUTPUT(v(3, 1, 5, 7, 4).one_of(is_even));
		// output: 1
		OUTPUT(v(3, 1, 5, 7, 4).one_of(is_odd));
		// output: 0
	});

	call("reverse_each", [&]{
		std::cout << "v(1, 2, 3, 4, 5).reverse_each" << std::endl;
		v(1, 2, 3, 4, 5).reverse_each([](auto it){
			std::cout << it << ", ";
		});
		// output: 5, 4, 3, 2, 1,
		std::cout << "\n";
	});

	call("sort", [&]{
		OUTPUT(v(3, 6, 2, 5, 1, 4).sort());
		// output: [1, 2, 3, 4, 5, 6]
		OUTPUT(v(3, 6, 2, 5, 1, 4).sort([](auto a, auto b){ return a < b ? 1 : -1; }));
		// output: [6, 5, 4, 3, 2, 1]
	});

	call("sort_by", [&]{
		OUTPUT(v("giraffe"s, "mouse"s, "hippopotamus"s, "cat"s).sort_by([](auto it){ return it.size(); }));
		// output: [cat, mouse, giraffe, hippopotamus]
	});

	call("take", [&]{
		OUTPUT(v(3, 6, 2, 5, 1, 4).take(3));
		// output: [3, 6, 2]
	});

	call("take_while", [&]{
		OUTPUT(v(2, 6, 3, 5, 1, 4).take_while(is_even));
		// output: [2, 6]
	});

	call("zip", [&]{
		std::cout << "v(1, 2, 3, 4).zip(v(14, 15, 14))" << std::endl;
		zubera::vector numbers{1, 2, 3, 4};
		zubera::vector ages{14, 15, 14};

		zubera::vector<
			zubera::tuple<
				std::optional<int>,
				std::optional<int>
			>
		> datas = numbers.zip(ages);

		for(auto&& [number, age] : datas){
			std::cout << "[";
			print(number); std::cout << ", ";
			print(age);    std::cout << "]\n";
		}
		/* output:
		[1, 14]
		[2, 15]
		[3, 14]
		[4, None]
		*/
	});


	return 0;
}
