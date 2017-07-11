#ifndef ZUBERA_ENUMERATOR_H
#define ZUBERA_ENUMERATOR_H

#include <utility>

namespace zubera{

template<
	typename T,
	typename Eacher
>
struct enumerator;

template<typename T, typename Eacher>
constexpr enumerator<T, std::decay_t<Eacher>>
make_enumerator(Eacher&& eacher);

template<
	typename T,
	typename Eacher
>
struct enumerator : zubera::enumerable<enumerator<T, Eacher>, T>{
	template<typename F>
	struct yielder{
		template<typename... Args>
		constexpr auto
		operator <<(Args&&... args) const{
			f(std::forward<Args>(args)...);
			return *this;
		}

		template<typename... Args>
		constexpr auto
		yield(Args&&... args) const{
			return f(std::forward<Args>(args)...);
		}

		template<typename... Args>
		constexpr auto
		operator()(Args&&... args) const{
			return yield(std::forward<Args>(args)...);
		}

		F f;
	};

	template<typename F>
	static constexpr yielder<std::decay_t<F>>
	make_yielder(F&& f){
		return { std::forward<F>(f) };
	}

	constexpr enumerator(Eacher eacher) : eacher(eacher){}

	template<typename F>
	constexpr auto
	each(F&& f) const{
		return eacher(make_yielder(std::forward<F>(f)));
	}

	template<typename Init, typename F>
	constexpr auto
	with_index(Init init, F&& f) const{
		return eacher(make_yielder([&](auto it) constexpr{
			return f(it, init++);
		}));
	}

	template<typename F>
	constexpr auto
	with_index(F&& f) const{
		return with_index(0, std::forward<F>(f));
	}

	template<typename Obj, typename F>
	constexpr auto
	with_object(Obj&& obj, F&& f) const{
		eacher(make_yielder([&](auto it) constexpr{
			return f(it, obj);
		}));
		return obj;
	}
private:
	Eacher eacher;
};

template<typename T, typename Eacher>
constexpr enumerator<T, std::decay_t<Eacher>>
make_enumerator(Eacher&& eacher){
	return { eacher };
}

template<typename T, typename Obj, typename F>
constexpr auto
make_enumerator(Obj&& obj, F&& f){
	return make_enumerator<T>([=](auto y) constexpr{ return f(obj, y); });
}

template<typename T>
constexpr auto
irange(T&& first, T&& last){
	return make_enumerator<T>([=](auto y) constexpr{
		for(auto i = first ; i < last ; ++i){
			y(i);
		}
	});
}

}  // namespace zubera

#endif /* ZUBERA_ENUMERATOR */
