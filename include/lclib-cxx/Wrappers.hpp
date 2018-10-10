#ifndef __Wrappers_hpp_2018_06_19_13_46
#define __Wrappers_hpp_2018_06_19_13_46
#include <utility>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <lclib-cxx/TypeTraits.hpp>


/**
 * PolymorphicWrapper is a Type Erased Pointer-like type that can allocate,
 * store, and obtain references to a type or any derived type.
 * PolymorphicWrapper Models an owned Pointer Type,
 * the pointer contained in a PolymorphicWrapper is deleted when the Object is deleted.
 * PolymorphicWrappers are move only.
 * In addition, instantiating PolymorphicWrapper with a possibly cv-qualified
 * is the same as instantiating it with the unqualified type.
 * PolymorphicWrapper cannot be instantiated with a non-class type. A program which does so is ill-formed.
 * In addition, the type parameter of PolymorphicWrapper must a have a virtual destructor.
 * The behavior is undefined if PolymorphicWrapper instantiated with a type that does NOT have a virtual destructor
 * is used, except if the instance is default-constructed and never assigned.
 * PolymorphicWrapper is a better type-erasure option then std::any,
 *  as it constrains the types of objects it can own to T and its subtypes.
 * PolymorphicWrapper is also better than std::variant,
 *  as PolymorphicWrapper can be used even when the set of potential value types is unknown.
 */
template<typename T> class PolymorphicWrapper{
public:
	static_assert(std::is_class_v<T>,"PolymorphicWrapper requires a class-type");

	using value_type = std::remove_cv_t<T>;
	using pointer = value_type*;
	using reference = value_type&;
	using const_pointer = const value_type*;
	using const_reference = const value_type&;
	using rvalue_reference = value_type&&;
	template<typename U> constexpr static bool is_derived = std::is_base_of_v<value_type,U>;
	template<typename U,typename Q=std::remove_cv_t<U>> using derived_type = std::enable_if_t<is_derived<Q>,Q>;
	template<typename U> using derived_pointer = std::add_pointer_t<derived_type<U>>;
	template<typename U> using derived_reference = std::add_lvalue_reference_t<derived_type<U>>;
	template<typename U> using const_derived_pointer = std::add_pointer_t<std::add_const_t<derived_type<U>>>;
	template<typename U> using const_derived_reference = std::add_lvalue_reference_t<std::add_const_t<derived_type<U>>>;
	template<typename U> using derived_rvalue_reference = std::add_rvalue_reference_t<derived_type<U>>;
	template<typename U> using derived_polymorphic_wrapper = require_types<PolymorphicWrapper&,derived_type<U>>;

	using copy_base = std::enable_if_t<std::is_copy_constructible_v<T>,bool>;
	using copy_assign_base = std::enable_if_t<std::is_copy_constructible_v<T>,PolymorphicWrapper&>;
	using move_base = std::enable_if_t<std::is_move_constructible_v<T>,bool>;
	using move_assign_base = std::enable_if_t<std::is_move_constructible_v<T>,PolymorphicWrapper&>;
	using move_from_const_base = const value_type&&;
	template<typename U> using copy_derived = std::enable_if_t<std::is_copy_constructible_v<derived_type<U>>>;
	template<typename U> using disable_copy_derived = std::enable_if_t<!std::is_default_constructible_v<derived_type<U>>>;
	template<typename U> using move_derived = std::enable_if_t<std::is_move_constructible_v<derived_type<U>>>;
	template<typename U> using disable_move_derived = std::enable_if_t<!std::is_move_constructible_v<derived_type<U>>>;
	template<typename U> using move_from_const_derived = require_types_t<bool,const derived_type<U>&&>;
	template<typename U> using instanceof_type = require_types_t<bool,derived_type<U>>;
	template<typename U> using default_in_place = std::enable_if_t<std::is_default_constructible_v<derived_type<U>>>;
	template<typename U,typename... Args> using construct_in_place = std::enable_if_t<std::is_constructible_v<derived_type<U>,Args...>>;
private:
    pointer val;
    PolymorphicWrapper(const PolymorphicWrapper&)=delete;
    PolymorphicWrapper& operator=(const PolymorphicWrapper&)=delete;
public:
    /**
     * Default Constructs a PolymorphicWrapper.
     * The PolymorphicWrapper owns no object of any type.
     * \Exceptions: Non-throwing
     */
    PolymorphicWrapper()noexcept(true):val(nullptr){}
    /**
     * Moves Constructs a PolymorphicWrapper.
     * The new PolymorphicWrapper takes ownership of the moved-from PolymorphicWrapper.
     * \Exceptions: Non-throwing
     */
    PolymorphicWrapper(PolymorphicWrapper&& r)noexcept(true):val(std::exchange(r.val,nullptr)){}
    ~PolymorphicWrapper(){
		delete val;
    }
    /**
     * Constructs a new PolymorphicWrapper.
     * The owned object is allocated and constructed using the copy constructor of T, with T as its dynamic type
     * This constructor does not participate in overload resolution unless std::is_copy_constructible_v<T> is true.
     * \Exceptions: Throws any exceptions thrown by T's copy-constructor (noexcept if the selected constructor is non-throwing)
     */
    PolymorphicWrapper(const value_type& t,copy_base=false)noexcept(std::is_nothrow_copy_constructible_v<T>):val(new value_type(t)){}
    /**
     * Constructs a new PolymorphicWrapper.
     * The owned object is allocated and constructed using the move constructor, with T as its dynamic type
     * This constructor does not participate in overload resolution unless std::is_move_constructible_v<T> is true.
     * \Exceptions: Throws any exceptions thrown by T's move-constructor (noexcept if the selected constructor is non-throwing)
     */
    PolymorphicWrapper(value_type&& t,move_base=false)noexcept(std::is_nothrow_move_constructible_v<value_type>):val(new value_type(t)){}

    /**
     * Delete move from const
     */
    PolymorphicWrapper(move_from_const_base)=delete;
    /**
     * Constructs a new PolymorphicWrapper.
     * The owned object is allocated and constructed using the copy constructor of U, with U as its dynamic type
     * This constructor does not participate in overload resolution unless std::is_base_of_v<T,U>
     *  and std::is_copy_constructible_v<U> are both true.
     * \Exceptions: Throws any exceptions thrown by U's Copy Constructor (noexcept if the selected constructor is non-throwing)
     */
    template<typename U,typename=copy_derived<U>> PolymorphicWrapper(const U& u)noexcept(std::is_nothrow_copy_constructible_v<U>):val(new derived_type<U>(u)){}
    /**
     * If A Derived type disables copy construction, prevent it from selecting Copy-base overload
     */
    template<typename U,typename=disable_copy_derived<U>> PolymorphicWrapper(const U&)=delete;
    /**
     * Constructs a new PolymorphicWrapper.
     * The owned object is allocated and constructed using the move constructor of U, with U as its dynamic type
     * This constructor does not participate in overload resolution unless std::is_base_of_v<T,U>
     *  and std::is_move_constructible_v<U> are both true.
     * \Exceptions: Throws any exception thrown by U's Move Constructor (noexcept if the selected constructor is non-throwing)
     */
    template<typename U,typename=move_derived<U>> PolymorphicWrapper(U&& u)noexcept(std::is_nothrow_move_constructible_v<U>):val(new derived_type<U>(u)){}

    /**
     * If the derived class has disabled move construction,
     * prevent use of Move-derived.
     */
    template<typename U,typename=disable_move_derived<U>> PolymorphicWrapper(U&& u)=delete;

    /**
     * Disable Move-from const for derived types.
     */
    template<typename U,typename=derived_type<U>> PolymorphicWrapper(const U&&)=delete;
    /**
     * Move Constructs a PolymorphicWrapper from a wrapper to a derived class.
     * The new PolymorphicWrapper takes ownership of the moved-from wrapper's owned object.
     * This constructor does not participate in overload resolution unless std::is_base_of_v<T,U> is true
     * \Exceptions: This constructor does not throw any exceptions
     */
    template<typename U,typename=derived_polymorphic_wrapper<U>> PolymorphicWrapper(PolymorphicWrapper<U>&& pw)noexcept(true):val(std::exchange(pw.val,nullptr)){}

    /**
     * Constructs a new PolymorphicWrapper.
     * The owned object is allocated and constructed using the default constructor of U, with U as its dynamic type (U may be T)
     * This constructor does not participate in overload resolution unless std::is_base_of_v<T,U> and
     * 	std::is_default_constructible_v<U> are both true.
     * \Exceptions: Throws any exception thrown by U's default constructor (noexcept if the selected constructor is non-throwing)
     */
    template<typename U,typename=default_in_place<U>> PolymorphicWrapper(std::in_place_type_t<U>)noexcept(std::is_nothrow_default_constructible_v<U>):val(new U){}

    /**
     * Constructs a new PolymorphicWrapper.
     * The owned object is allocated and constructed using the default constructor of U, with U as its dynamic type (U may be T)
     * This constructor does not participate in overload resolution unless std::is_base_of_v<T,U> and
     *  std::is_constructible_v<U,Args...> are both true.
     * \Exceptions: Throws any exception thrown by the selected constructor (noexcept if that constructor is non-throwing)
     */
    template<typename U,typename... Args,typename=construct_in_place<U,Args...>> PolymorphicWrapper(std::in_place_type_t<U>,Args&&... args)noexcept(std::is_nothrow_constructible_v<U,Args...>)
    	:val(new U(std::forward(args)...)){}

    /**
     * Explicitly casts to a derived class.
     * This method does not participate in overload resolution unless std::is_base_of_v<T,U> is true.
     * \Exceptions: Throws std::bad_cast if the dynamic type of the owned object is not U or a base class of U.
     */
    template<typename U> explicit operator derived_reference<U>()&{
            return dynamic_cast<derived_reference<U>>(*val);
    }
    /**
     * Explicitly casts to a derived class (const overload).
     * This method does not participate in overload resolution unless std::is_base_of_v<T,U> is true.
     * \Exceptions: Throws std::bad_cast if the dynamic type of the owned object is not U or a subclass of U.
     */
    template<typename U> explicit operator const_derived_reference<U>()const{
            return dynamic_cast<const_derived_reference<U>>(*val);
    }
    /**
     * Explicitly cast to a derived class (rvalue overload).
     * This method does not participate in overload resolution unless std::is_base_of_v<T,U> is true.
     * \Exceptions: Throws std::bad_cast if the dynamic type of the owned object is not U or a subclass of U.
     */
    template<typename U> explicit operator derived_rvalue_reference<U>()&&{
            return std::move(dynamic_cast<derived_rvalue_reference<U>>(*val));
    }
    /**
     * Implicitly converts to T. (same as **this).
     * \Exceptions: Non-throwing
     */
    operator reference()& noexcept(true){
        return *val;
    }
    /**
     * Implicitly converts to T. (same as **this, const overload).
     * \Exceptions: Non-throwing
     */
    operator const_reference()const noexcept(true){
        return *val;
    }
    /**
     * Implicit converts to T. (same as **this, rvalue overload)
     * \Exceptions: Non-throwing
     */
    operator rvalue_reference()&& noexcept(true){
        return std::move(*val);
    }

    /**
     * Obtains the dynamic type of the owned object, or the type information of T if there is no dynamic type.
     * \Exceptions: Non-throwing
     */
    const std::type_info& type()const noexcept(true){
        return val!=nullptr?typeid(*val):typeid(T);
    }
    /**
     * Checks if The dynamic type of the owned object is U or a subclass of U.
     * This method does not participate in overload resolution unless std::is_base_of_v<T,U> is true.
     * \Exceptions: Non-throwing
     */
	template<typename U> instanceof_type<U> instanceof()const noexcept(true){
		return dynamic_cast<derived_pointer<U>>(val)!=nullptr;
	}
	/**
	 * Preforms a Checked cast to U. Effectively the same as static_cast<U&>(*this). (which also results in a checked cast).
	 * This method does not participate in overload resolution unless std::is_base_of_v<T,U> is true.
	 * \Exceptions: Throws std::bad_cast if The dynamic type of the owned value is not U or a subclass of U.
	 */
    template<typename U>
        derived_reference<U> checkedcast()&{
            return dynamic_cast<derived_reference<U>>(*val);
        }
    /**
     * Preforms a Checked cast to U. Effectively the same as static_cast<const U&>(*this) (const overload).
     * This method does not participate in overload resolution unless std::is_base_of_v<T,U> is true.
     * \Exceptions: Throws std::bad_cast if the dynamic type of the owned value is not U or a subclass of U.
     */
    template<typename U>
        const_derived_reference checkedcast()const{
            return dynamic_cast<const_derived_reference<U>>(*val);
        }
    /**
     * Preforms a Check cast to U. Assuming *this is an xvalue, effectively the same as static_cast<U&&>(*this) (rvalue overload).
     * This method does not participate in overload resolution unless std::is_base_of_v<T,U> is true.
     * \Exceptions: Throws std::bad_cast if the dynamic type of the owned value is not U or a subclass.
     */
    template<typename U,typename=std::enable_if_t<std::is_base_of_v<T,U>>>
        derived_rvalue_reference<U> checkedcast()&&{
            return std::move(dynamic_cast<derived_reference<U>>(*val));
        }
    pointer operator->()noexcept(true){
        return val;
    }
    const_pointer operator->()const noexcept(true){
        return val;
    }
    /**
     * Obtains the owned value. (Same as implicit cast to T&)
     * \Exceptions: Non-throwing
     */
    reference operator*()&noexcept(true){
        return *val;
    }
    /**
     * Obtains the owned value. (Same as implicit cast to const T&)
     */
    const_reference operator*()const noexcept(true){
        return *val;
    }
    /**
     * Obtains the owned value. (Same as implicit cast to T&&)
     * \Exceptions: Non-throwing
     */
    rvalue_reference operator*()&&noexcept(true){
        return std::move(*val);
    }

    /**
     * Deletes the owned value and moves r to this.
     * The current object gains ownership of r's owned value.
     * \Exceptions: Non-throwing
     */
    PolymorphicWrapper& operator=(PolymorphicWrapper&& r)noexcept(true){
        delete val;
        val = std::exchange(r.val,nullptr);
        return *this;
    }
    /**
     * Deletes the owned value and moves r to this.
     * The current object gains ownership of r's owned value.
     * This method does not participate in overload resolution unless std::is_base_of_v<T,U> is true.
     * \Exceptions: Non-throwing
     */
    template<typename U>
    derived_polymorphic_wrapper<U>& operator=(PolymorphicWrapper<U>&& r)noexcept(true){
        delete val;
        val = std::exchange(r.val,nullptr);
        return *this;
    }
};


template<typename T> PolymorphicWrapper(T&&) ->PolymorphicWrapper<T>;
template<typename T> PolymorphicWrapper(const T&) -> PolymorphicWrapper<T>;
template<typename T> PolymorphicWrapper(std::in_place_type_t<T>) -> PolymorphicWrapper<T>;
template<typename T,typename... Args> PolymorphicWrapper(std::in_place_type_t<T>,Args&&...) -> PolymorphicWrapper<T>;




#endif
