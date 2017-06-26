#ifndef HELPER_H
#define HELPER_H

#include <utility>

template<typename T>
struct type_{
	template<typename U>
	constexpr bool
	operator ==(U&&) const{
		return std::is_same<T, std::decay_t<U>>{};
	}
};

template<typename T>
constexpr type_<T> t{};


#endif /* HELPER */
