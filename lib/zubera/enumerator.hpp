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
		return eacher(make_yielder(f));
	}

	template<typename F>
	constexpr auto
	with_index(F f) const{
		auto i = 0;
		return eacher(make_yielder([&](auto it) constexpr{
			return f(it, i++);
		}));
	}

private:
	Eacher eacher;
};

template<typename T, typename Eacher>
constexpr enumerator<T, std::decay_t<Eacher>>
make_enumerator(Eacher&& eacher){
	return { eacher };
}

}  // namespace zubera

#endif /* ZUBERA_ENUMERATOR */
