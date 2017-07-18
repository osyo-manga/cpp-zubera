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
- [ ] max_by : ブロックの戻り値が最大値となる要素を返す。
- [ ] member : 要素が含まれるか調べる。
- [ ] min : 最小値を返す。
- [ ] min_by : ブロックの戻り値が最小値となる要素を返す。
- [ ] minmax : 最小値と最大値を返す。
- [ ] minmax_by : ブロックの戻り値が最小値、最大値となる要素を返す。
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
- [ ] zip : 複数のオブジェクトの要素の組み合わせを作る。



