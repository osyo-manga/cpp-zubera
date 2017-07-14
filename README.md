[![Build Status](https://travis-ci.org/osyo-manga/cpp-zubera.svg_ofbranch=master)](https://travis-ci.org/osyo-manga/cpp-zubera)

# zubera is Ruby like enumerable C++ library

## Implemented
(result: std::optional)
- all_of : すべて○○かを調べる。
- any_of : どれか○○かを調べる。
- collect : ブロックが返す値を集める。
- count : 指定の要素の数を数える。
- cycle : 要素を何度も繰り返し取り出す。
- detect : 条件に会う要素を返す。(result: std::optional)
- drop : 先頭から要素をいくつか除いて配列を作成。
- drop_while : ブロックが真を返す要素を先頭から除いて配列を作成。
- each_cons : 要素を指定の数ずつ繰り返し取り出す。
- each_slice : 要素を指定の数ずつ繰り返し取り出す。
- each_with_index : 要素を番号付きで繰り返し取り出す。
- each_with_object : ブロックを繰り返してオブジェクトを操作。
- entries : 配列に変換。
- find : 条件に会う要素を返す。(result: std::optional)
- find_all : 条件に合う要素を集める。
- find_index : 要素の位置を調べる。(result: std::optional)
- first : 先頭の要素を取り出す。(result: std::optional)
- inject : ブロックを使って繰り返し計算する。
- map : ブロックが返す値を集める。
- max : 最大値を返す。(result: std::optional)
- none_of : すべて○○でないかを調べる。
- one_of : 1つだけ○○かを調べる。
- partition : 要素を2つの配列に分ける。
- reduce : ブロックを使って繰り返し計算する。
- reject : 条件に合わない要素を集める。
- reverse_each : 要素を末尾から繰り返し取り出す。
- select : 条件に合う要素を集める。
- sort : 要素をソートする。
- take : 先頭から要素をいくつか取り出す。
- take_while : ブロックが真を返す要素を先頭から取り出す。
- to_a : 配列に変換。


