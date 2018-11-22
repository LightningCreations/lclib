#ifndef __TupleIterators_hpp_2018_08_24_15_33
#define __TupleIterators_hpp_2018_08_24_15_33
#include <tuple>
#include <variant>
#include <utility>
#include <type_traits>
#include <lclib/TypeTraits.hpp>
#include <iterator>
/**
 * Helper class for iterating over the elements of a tuple or tuple-like-type
 * Returns an InputIterator of variants containing const references to the elements of the tuple
 * The behavior is undefined if the iterator's lifetime is longer then the tuple.
 * tuple_iterator can be instantiated for any type which defines std::tuple_size, std::tuple_element, and std::get
 * except for std::variant.
 */
template<typename T,typename=void> struct tuple_iterator{
private:
    tuple_iterator()=delete;
    tuple_iterator(const tuple_iterator&)=delete;
    tuple_iterator(tuple_iterator&&)=delete;
    tuple_iterator& operator=(const tuple_iterator&)=delete;
    tuple_iterator& operator=(tuple_iterator&&)=delete;
};
/**
 * Specialization of tuple_iterator for std::tuples
 */
template<typename T> struct tuple_iterator<T,std::void_t<std::tuple_size<remove_cvref_t<T>>>>{
public:
	using tuple_type = remove_cvref_t<T>;
private:
	template<size_t... Is> static auto get_value_type(std::index_sequence<Is...>) ->
		std::variant<std::reference_wrapper<std::remove_reference_t<
			std::add_const_t<std::tuple_element_t<Is,tuple_type>>>>...>;

    const static constexpr std::size_t max_pos = std::tuple_size_v<T>;
public:
    using value_type = decltype(get_value_type(std::make_index_sequence<max_pos>{}));
    using difference_type = std::ptrdiff_t;
    using pointer = tuple_iterator;
    using iterator_category = std::input_iterator_tag;
private:
    const tuple_type& target;
    std::size_t pos;
    friend struct reference;
public:
    struct reference{
    private:
        const tuple_type& target;
        std::size_t pos;
        template<std::size_t I,std::size_t... Is> value_type getValueHelper(std::index_sequence<I,Is...>){
            if(I==pos)
                return std::ref(std::get<I>(target));
            else
                return getValueHelper(std::index_sequence<Is...>{});
        }
        template<std::size_t I> value_type getValueHelper(std::index_sequence<I>){
            if(I==pos)
                return std::ref(std::get<I>(target));
            else
                throw "Placeholder error: reached end of tuple";
        }
    public:
        reference(const T& t,std::size_t pos):target(t),pos(t.pos){}

        operator value_type(){
            return getValueHelper(std::make_index_sequence<max_pos>{});
        }
        template<typename U,std::size_t... Ids,typename=std::enable_if_t<std::disjunction_v<
        		std::is_same<U,std::tuple_element_t<Ids,T>>...>>> const U& get(std::index_sequence<Ids...>)const{
        	return std::get<U>(getValueHelper(std::make_index_sequence<max_pos>{}));
        }
        template<typename U,typename=decltype(get(std::make_index_sequence<max_pos>{}))> operator const U&()const{
        	return get(std::make_index_sequence<max_pos>{});
        }
    };
    tuple_iterator(const T& target,std::size_t pos=0):target(target),pos(pos){}
    tuple_iterator(T&&,std::size_t=0)=delete;
    tuple_iterator& operator++(){
        pos++;
        return *this;
    }
    reference operator*()const{
        return reference(target,pos);
    }
};
template<typename... Ts> struct tuple_iterator<std::variant<Ts...>,void>{
private:
	tuple_iterator()=delete;
	tuple_iterator(const tuple_iterator&)=delete;
	tuple_iterator(tuple_iterator&&)=delete;
	tuple_iterator& operator=(const tuple_iterator&)=delete;
	tuple_iterator& operator=(tuple_iterator&&)=delete;
};

template<typename T> tuple_iterator(const T&) -> tuple_iterator<T>;
template<typename T> tuple_iterator(const T&,std::size_t) -> tuple_iterator<T>; 

template<typename T,typename=std::enable_if_t<std::is_constructible_v<tuple_iterator<T>,const T&>>>
	struct tuple_iterable{
	private:
		const T& t;
	public:
		tuple_iterable(const T& t):t(t){}
		tuple_iterable(T&&)=delete;
		tuple_iterator<T> begin()const{
			return tuple_iterator(t);
		}
		tuple_iterator<T> end()const{
			return tuple_iterator(t,std::tuple_size_v<T>);
		}
	};

template<typename T> tuple_iterable(const T&) -> tuple_iterable<T>;



#endif
