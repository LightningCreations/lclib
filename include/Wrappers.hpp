#ifndef __Wrappers_hpp_2018_06_19_13_46
#define __Wrappers_hpp_2018_06_19_13_46
#include <utility>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <TypeInfo.hpp>

template<typename T> class PolymorphicWrapper{
private:
    T* val;
    PolymorphicWrapper(const PolymorphicWrapper&)=delete;
    PolymorphicWrapper& operator=(const PolymorphicWrapper&)=delete;
public:
    PolymorphicWrapper()noexcept(true):val(nullptr){}
    PolymorphicWrapper(PolymorphicWrapper&& r)noexcept(true):val(std::exchange(r.val,nullptr)){}
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>> 
        PolymorphicWrapper(PolymorphicWrapper<U>&& r)noexcept(true):val(std::exchange(r.val,nullptr)){}
    ~PolymorphicWrapper(){
        if(val!=nullptr)
            delete val;
    }
    PolymorphicWrapper(const T& t)noexcept(std::is_nothrow_copy_constructible_v<T>):val(new T(t)){}
    PolymorphicWrapper(T&& t)noexcept(std::is_nothrow_move_constructible_v<T>):val(new T(t)){}
    PolymorphicWrapper(const T&&)=delete;
    template<typename U,
    typename=typename std::enable_if<std::conjunction<std::is_base_of<T,U>,
    std::is_default_constructible<U>>::value>::type> 
        PolymorphicWrapper(std::in_place_type_t<U>)noexcept(std::is_nothrow_default_constructible_v<T>):val(new U){}
    template<typename U,typename... Args,
        typename=std::enable_if_t<std::is_base_of_v<T,U>>> 
        PolymorphicWrapper(std::in_place_type_t<U>,Args&&... args)noexcept(std::is_nothrow_constructible_v<T,Args&&...>):
        val(new U(std::forward<Args>(args)...)){}
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>> 
        PolymorphicWrapper(const U& u)noexcept(std::is_nothrow_copy_constructible_v<U>):val(new U(u)){}
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        PolymorphicWrapper(U&& u)noexcept(std::is_nothrow_move_constructible_v<U>):val(new U(u)){}
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        PolymorphicWrapper(const U&&)=delete;

    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        explicit operator U&()&{
            return dynamic_cast<U&>(*val);
    }
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        explicit operator const U&()const{
            return dynamic_cast<const U&>(*val);
    }
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        explicit operator U&&()&&{
            return std::move(dynamic_cast<U&>(*val));
    }
    operator T&()& noexcept(true){
        return *val;
    }
    operator const T&()const noexcept(true){
        return *val;
    }
    operator T&&()&& noexcept(true){
        return std::move(*val);
    }
    const std::type_info& type()const noexcept(true){
        return val==nullptr?typeid(*val):typeid(nullptr_t);
    }
    template<typename U,typename= std::enable_if_t<std::is_base_of_v<T,U>>>
        bool instanceof()const noexcept(true){
            return dynamic_cast<U*>(val)!=nullptr;
        }
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        U& checkedcast()&{
            return dynamic_cast<U&>(*val);
        }
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        const U& checkedcast()const{
            return dynamic_cast<const U&>(*val);
        }
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        U&& checkedcast()&&{
            return std::move(dynamic_cast<U&>(*val));
        }
    
    T* operator->()noexcept(true){
        return val;
    }
    const T* operator->()const noexcept(true){
        return val;
    }
    T& operator*()&noexcept(true){
        return *val;
    }
    const T& operator*()const noexcept(true){
        return *val;
    }
    T&& operator*()&&noexcept(true){
        return std::move(*val);
    }
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        PolymorphicWrapper<T>& operator=(const U& u)noexcept(std::is_nothrow_copy_constructible_v<U>){
            if(val!=nullptr)
                delete val;
            val = new U(u);
            return *this;
        }
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        PolymorphicWrapper<T>& operator=(U&& u)noexcept(std::is_nothrow_move_constructible_v<U>){
            if(val!=nullptr)
                delete val;
            val = new U(u);
            return *this;
    }
    PolymorphicWrapper& operator=(PolymorphicWrapper&& r)noexcept(true){
        if(val!=nullptr)
            delete val;
        val = std::exchange(r.val,nullptr);
        return *this;
    }
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
    PolymorphicWrapper& operator=(PolymorphicWrapper<U>&& r)noexcept(true){
        if(val!=nullptr)
            delete val;
        val = std::exchange(r.val,nullptr);
        return *this;
    }
    
};


template<typename T> PolymorphicWrapper(T&&) ->PolymorphicWrapper<T>;
template<typename T> PolymorphicWrapper(const T&) -> PolymorphicWrapper<T>;
template<typename T> PolymorphicWrapper(std::in_place_type_t<T>) -> PolymorphicWrapper<T>;
template<typename T,typename... Args> PolymorphicWrapper(std::in_place_type_t<T>,Args&&...) -> PolymorphicWrapper<T>;


namespace types{
	template<typename T> struct TypeHash<enable_specialization_t<PolymorphicWrapper<T>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return nameHash("Vec2");
		}
	};
}



#endif
