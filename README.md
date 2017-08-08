[![Build Status](https://travis-ci.org/osyo-manga/cpp-zubera.svg?branch=master)](https://travis-ci.org/osyo-manga/cpp-zubera)


# zubera is Ruby like enumerable C++ library

## Implemented

- [x] all_of : すべて○○かを調べる。
- [x] any_of : どれか○○かを調べる。
- [ ] chunk : ブロックの戻り値ごとに要素の並びをまとめる。
- [x] collect : ブロックが返す値を集める。
- [ ] collect_concat : 
- [x] count/count_if : 指定の要素の数を数える。
- [x] cycle : 要素を何度も繰り返し取り出す。
- [x] detect : 条件に会う要素を返す。
- [x] drop : 先頭から要素をいくつか除いて配列を作成。
- [x] drop_while : ブロックが真を返す要素を先頭から除いて配列を作成。
- [x] each_cons : 要素を指定の数ずつ繰り返し取り出す。
- [ ] each_entry : 
- [x] each_slice : 要素を指定の数ずつ繰り返し取り出す。
- [x] each_with_index : 要素を番号付きで繰り返し取り出す。
- [x] each_with_object : ブロックを繰り返してオブジェクトを操作。
- [x] entries : 配列に変換。
- [x] find : 条件に会う要素を返す。
- [x] find_all : 条件に合う要素を集める。
- [x] find_index : 要素の位置を調べる。
- [x] first : 先頭の要素を取り出す。
- [ ] flat_map : 
- [ ] grep : 「引数 === 要素」である要素を集める。
- [ ] group_by : 要素をいくつかのグループに分ける。
- [x] include : 要素が含まれるか調べる。
- [x] inject : ブロックを使って繰り返し計算する。
- [x] map : ブロックが返す値を集める。
- [x] max : 最大値を返す。
- [x] max_by : ブロックの戻り値が最大値となる要素を返す。
- [x] member : 要素が含まれるか調べる。
- [x] min : 最小値を返す。
- [x] min_by : ブロックの戻り値が最小値となる要素を返す。
- [x] minmax : 最小値と最大値を返す。
- [x] minmax_by : ブロックの戻り値が最小値、最大値となる要素を返す。
- [x] none_of : すべて○○でないかを調べる。
- [x] one_of : 1つだけ○○かを調べる。
- [x] partition : 要素を2つの配列に分ける。
- [x] reduce : ブロックを使って繰り返し計算する。
- [x] reject : 条件に合わない要素を集める。
- [x] reverse_each : 要素を末尾から繰り返し取り出す。
- [x] select : 条件に合う要素を集める。
- [ ] slice_before : 要素の並びを分割。
- [x] sort : 要素をソートする。
- [x] sort_by : ブロックの戻り値でソートする。
- [x] take : 先頭から要素をいくつか取り出す。
- [x] take_while : ブロックが真を返す要素を先頭から取り出す。
- [x] to_a : 配列に変換。
- [x] zip : 複数のオブジェクトの要素の組み合わせを作る。

## Example

#### `all_of`

```cpp
#include <zubera.hpp>

int
main(){
	zubera::vector v{2, 4, 6, 8};
	
	bool result = v.all_of([](auto it){
		return it % 2 == 0;
	});
	std::cout << result << std::endl;
	// output: 1

	result = v.all_of([](auto it){
		return it <= 6;
	});
	std::cout << result << std::endl;
	// output: 0

	return 0;
}
```

#### `any_of`

```cpp
#include <zubera.hpp>

int
main(){
	zubera::vector v{1, 2, 3};
	
	bool result = v.any_of([](auto it){
		return it % 2 == 0;
	});
	std::cout << result << std::endl;
	// output: 1

	result = v.any_of([](auto it){
		return it % 2 != 0;
	});
	std::cout << result << std::endl;
	// output: 1

	result = v.all_of([](auto it){
		return it > 4;
	});
	std::cout << result << std::endl;
	// output: 0

	return 0;
}
```

#### `collect/map`

```cpp
#include <zubera.hpp>
#include <string>

int
main(){
	zubera::vector v{1, 2, 3};
	
	zubera::vector<int> twices = v.collect([](auto it){
		return it + it;
	});
	std::cout << twices << std::endl;
	// output: [2, 4, 6]
	
	zubera::vector<std::string> strs = v.collect([](auto it){
		return std::to_string(it) + std::to_string(it);
	});
	std::cout << strs << std::endl;
	// output: [11, 22, 33]

	return 0;
}
```

#### `count/count_if`

```cpp
#include <zubera.hpp>

int
main(){
	zubera::vector v{1, 1, 2, 3, 1, 2, 1, 4};

	std::cout << v.count() << std::endl;
	// output: 8

	std::cout << v.count(1) << std::endl;
	// output: 4

	std::cout << v.count_if([](auto it){ return it % 2 == 0; }) << std::endl;
	// output: 3

	return 0;
}
```

#### `cycle`

```cpp
#include <zubera.hpp>

int
main(){
	zubera::vector v{1, 2};

	v.cycle(2, [](auto it){
		std::cout << it << " ";
	});
	// output: 1 2 1 2
	std::cout << std::endl;

	// cycle return zubera::enumerator
	auto result = v.cycle(3);
	std::cout << result << std::endl;
	// output: [1, 2, 1, 2, 1, 2]

	return 0;
}
```

#### `detect/find`

```cpp
#include <zubera.hpp>

int
main(){
	zubera::vector v{5, 3, 2, 1, 4};

	std::optional<int> result = v.detect([](auto it){ return it % 2 == 0; });
	std::cout << bool(result) << std::endl;
	// output: 1
	std::cout << *result << std::endl;
	// output: 2

	result = v.detect([](auto it){ return it > 5; });
	std::cout << bool(result) << std::endl;
	// output: 0

	return 0;
}
```


#### drop

```cpp
#include <zubera.hpp>

int
main(){
	zubera::vector v{5, 3, 2, 1, 4};

	zubera::vector<int> result = v.drop(2);
	std::cout << result << std::endl;
	// output: [2, 1, 4]

	return 0;
}
```


#### `drop_while`

```cpp
#include "../../../lib/zubera.hpp"
// #include <zubera.hpp>

int
main(){
	zubera::vector v{5, 3, 7, 8, 2, 1, 4};

	zubera::vector<int> result = v.drop_while([](auto it){ return it % 2 != 0; });
	std::cout << result << std::endl;
	// output: [8, 2, 1, 4]

	return 0;
}
```


#### `each_cons`

```cpp
#include <zubera.hpp>

int
main(){
	zubera::vector v{1, 2, 3, 4};

	v.each_cons(2, [](zubera::vector<int> const& it){
		std::cout << it << ", ";
	});
	// output: [1, 2], [2, 3], [3, 4],

	return 0;
}
```


#### `each_slice`

```cpp
#include <zubera.hpp>

int
main(){
	zubera::vector v{1, 2, 3, 4, 5};

	v.each_slice(2, [](zubera::vector<int> const& it){
		std::cout << it << ", ";
	});
	// output: [1, 2], [3, 4], [5],

	return 0;
}
```


#### `each_with_index`

```cpp
#include <zubera.hpp>
#include <string>

int
main(){
	using namespace std::literals;
	zubera::vector v{"homu"s, "mami"s, "mado"s};

	v.each_with_index([](auto it, auto i){
		std::cout << "(" << i << " : " << it << "), ";
	});
	// output: (0 : homu), (1 : mami), (2 : mado),

	return 0;
}
```


#### `each_with_object`

```cpp
#include <zubera.hpp>
#include <string>

int
main(){
	using namespace std::literals;
	zubera::vector v{"homu"s, "mami"s, "mado"s};

	v.each_with_object("value: ", [](auto it, auto obj){
		std::cout << obj << it << ", ";
	});
	// output: value: homu, value: mami, value: mado,

	return 0;
}
```


#### `find_index`

```cpp
#include <zubera.hpp>
#include <string>

int
main(){
	using namespace std::literals;
	zubera::vector v{"homu"s, "mami"s, "mado"s, "mami"s, "mado"s};

	std::optional<int> index = v.find_index([](auto it){ return it == "mado"; });
	std::cout << bool(index) << std::endl;
	// output: 1
	std::cout << *index << std::endl;
	// output: 2

	index = v.find_index([](auto it){ return it == "saya"; });
	std::cout << bool(index) << std::endl;
	// output: 0

	return 0;
}
```


#### `first`

```cpp
#include <zubera.hpp>
#include <string>

int
main(){
	using namespace std::literals;
	zubera::vector v{"homu"s, "mami"s, "mado"};

	std::optional<std::string> head = v.first();
	std::cout << bool(head) << std::endl;
	// output: 1
	std::cout << *head << std::endl;
	// output: homu

	head = zubera::vector<std::string>{}.first();
	std::cout << bool(head) << std::endl;
	// output: 0

	zubera::vector<std::string> heads = v.first(2);
	std::cout << heads << std::endl;
	// output: [homu, mami]

	heads = zubera::vector<std::string>{}.first(2);
	std::cout << result << std::endl;
	// output: []

	return 0;
}
```


#### `include/member`

```cpp
#include <zubera.hpp>
#include <string>

int
main(){
	using namespace std::literals;
	zubera::vector v{"homu"s, "mami"s, "mado"};

	bool has_homu = v.include("homu");
	std::cout << has_homu << std::endl;
	// output: 1

	bool has_saya = v.include("saya");
	std::cout << has_saya << std::endl;
	// output: 0

	return 0;
}
```


#### `inject/reduce`

```cpp
#include <zubera.hpp>

int
main(){
	zubera::vector v{1, 2, 3, 4, 5};

	int sum = v.inject([](auto sum, auto it){
		return sum + it;
	});
	std::cout << sum << std::endl;
	// output: 15

	// with init
	sum = v.inject(10, [](auto sum, auto it){
		return sum + it;
	});
	std::cout << sum << std::endl;
	// output: 25

	return 0;
}
```


#### `max`

```cpp
#include <zubera.hpp>

int
main(){
	zubera::vector v{3, 2, 5, 1, 4};

	std::optional<int> top = v.max();
	std::cout << bool(top) << std::endl;
	// output: 1
	std::cout << *top << std::endl;
	// output: 5

	top = zubera::vector<int>{}.max();
	std::cout << bool(top) << std::endl;
	// output: 0

	zubera::vector<int> tops = v.max(2);
	std::cout << tops << std::endl;
	// output: [5, 4]

	return 0;
}
```









