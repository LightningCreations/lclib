#ifndef __TupleIterators_hpp_2018_08_24_15_33
#define __TupleIterators_hpp_2018_08_24_15_33
#include <tuple>
#include <variant>
#include <utility>
#include <type_triats>
template<typename T> struct tuple_iterator{
private:
    tuple_iterator()=delete;
    tuple_iterator(const tuple_iterator&)=delete;
    tuple_iterator(tuple_iterator&&)=delete;
    tuple_iterator& operator=(const tuple_iterator&)=delete;
    tuple_iterator& operator=(tuple_iterator&&)=delete;
};

template<typename... Ts> struct tuple_iterator<std::tuple<Ts...>>{
private:
    using value_type = std::variant<std::add_lvalue_reference_t<std::add_const_t<Ts>>...>;
    const static constexpr std::size_t max_pos = sizeof...(Ts);
    const std::tuple<Ts...>& target;
    std::size_t pos;
    friend struct reference;
    struct reference{
    private:
        const std::tuple<Ts...>& target;
        std::size_t pos;
        template<std::size_t I,std::size_t... Is> value_type getValueHelper(std::index_sequence<I,Is...>){
            if(I==pos)
                return std::get<I>(target.target);
            else
                return getValueHelper(std::index_sequence<Is...>{});
        }
        template<std::size_t I> value_type getValueHelper(std::index_sequence<I>){
            if(I==pos)
                return std::get<I>(target.target);
            else
                throw "Placeholder error: reached end of tuple";
        }
    public:
        reference(const std::tuple<Ts...>& t,std::size_t pos):target(t),pos(t.pos){}
        operator value_type(){
            return getValueHelper(std::make_index_sequence<max_pos>{});
        }
        template<typename T,typename=std::enable_if_t<(std::is_same_v<T,Ts>||...)>> operator T()const{
            return std::get<T>(getValueHelper(std::make_index_sequence<max_pos>{});
        }
    };
    tuple_iterator(const std::tuple<Ts...>& target,std::size_t pos=0):target(target),pos(pos){}
    tuple_iterator(std::tuple<Ts...>&&,std::size_t=0)=delete;
    tuple_iterator& operator++(){
        pos++;
        return *this;
    }
    reference operator*()const{
        return reference(target,pos);
    }
};
template<typename T> struct tuple_iterator<const T>:tuple_iterator<T>{};

template<typename T> tuple_iterator(const T&) -> tuple_iterator<T>;
template<typename T> tuple_iterator(const T&,std::size_t) -> tuple_iterator<T>; 

template<typename T,typename=decltype(tuple_iterator<T>(std::declval<const T&>()))> struct tuple_iterable{
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