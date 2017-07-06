#ifndef ZUBERA_VECTOR_H
#define ZUBERA_VECTOR_H

#include <vector>
#include "./enumerable.hpp"

namespace zubera{

template<typename T>
struct vector : std::vector<T>, enumerable<vector<T>, T, vector>{
	using enumerable_value_type = T;
	using std::vector<T>::vector;
	using self_t = vector<T>;

	template<typename F>
	auto
	each(F f) const{
		for(auto n : *this){
			f(n);
		}
		return *this;
	}

	auto
	push(T const& new_value) const{
		std::vector<T> v{this->begin(), this->end()};
		v.push_back(new_value);
		return self_t{ v.begin(), v.end() };
	}
};
template<typename T, typename...Args>
vector(T&&, Args...) -> vector<std::decay_t<T>>;

template<typename T, typename U>
auto
operator ==(vector<T> const&v, U&& u)
->decltype(static_cast<std::vector<T>>(v) == u){
	return static_cast<std::vector<T>>(v) == u;
}

}  // namespace zubera

#endif /* ZUBERA_VECTOR */
