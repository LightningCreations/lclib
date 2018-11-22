/*
 * Callable.hpp
 *
 *  Created on: Sep 6, 2018
 *      Author: connor
 */

#ifndef _INCLUDE_CALLABLE_HPP__2018_09_06_9_27_41
#define _INCLUDE_CALLABLE_HPP__2018_09_06_9_27_41
#include <utility>
#include <type_traits>
#include <mutex>
#include <lclib/Config.hpp>

/**
 * A type satisfying FunctionObject, which models the identity function,
 *  that is, calling the function forwards its argument.
 */
class identity{
public:
	constexpr identity()=default;
	template<typename T> constexpr T&& operator()(T&& t)const noexcept(true){
		return std::forward(t);
	}
};


/**
 * Returns a forwarding Callable, that is, a ForwardingFunctionObject, that when called
 * with valid arguments for the source callable, yields the same result as INVOKE(c,std::forward(args)...)
 * Arguments are captured via perfect forwarding so lvalues are passed by reference.
 * Note that because the source may be any callable, and the result is a FunctionObject,
 * this function makes it possible to wrap Callables that do not provide the function call operator into a single object.
 * This is less constrained then wrapping it into a std::function as std::function constrains the call signature.
 * Given any args, which are perfectly forwarded as though by a template parameter pack,
 * Let INVOKE be some invoke expression which accepts a callable object, and parameters for the invocation
 * (which includes std::invoke, std::bind (which does not invoke the function, but wraps parameters), std::apply (where args are stored in a single tuple), and std::visit (where all the arguments are passed in a variant))
 * Also let c be some lvalue callable (which may or may not be const-qualified), and f be the result of forwardCall(c)
 * If INVOKE(c,std::forward(args)...) is well formed, then INVOKE(f,std::forward(args)...) is also well formed (otherwise it is ill-formed),
 * If INVOKE(c,std::forward(args)...) is non-throwing, then INVOKE(f,std::forward(args)...) is non-throwing (otherwise potentially throwing),
 * If INVOKE(c,std::forward(args)...) is a constant expression, then INVOKE(f,std::forward(args)...) is a constant expression.
 * In addition, the result of INVOKE(c,std::forward(args)...) is semantically equivalent (though may not be the same as), the result of INVOKE(f,std::forward(args)...),
 * the same applies to the observable side-effects of INVOKE(c,std::forward(args)...)
 * A Callable Satisfies ForwardingFunctionObject if, by design, all of the above conditions are satisfied
 */
template<typename Callable> constexpr auto forwardCall(Callable& c){
	return  [&c](auto&&... args)mutable noexcept(noexcept(std::invoke(c,std::forward(args)...)))->std::invoke_result_t<Callable,decltype(std::forward(args))...>{
		return std::invoke(c,std::forward(args)...);
	};
}

/**
 * Returns a forwarding Callable, which forwards function calls to the underlying object,
 * in a thread-safe way, by locking the specified mutex.
 * If the mutex is locked (and not owned by the current thread) when this function is called, the behavior is undefined
 * The result is a FunctionalObject which forwards invocations to the underlying object, after locking the supplied mutex.
 * \Mutex must satisfy BasicMutex
 * \Callable must statisfy Invocable
 * Note: If the Callable is a Pointer to member object, access and mutation of the underlying object is not thread-safe
 * Only the actually invocation of the Invocable is guaranteed to be thread-safe
 */
template<typename Mutex,typename Callable> constexpr auto forwardThreadSafe(Mutex& m,Callable& c){
	return [&m,&c](auto&&... args)mutable->std::invoke_result_t<Callable,decltype(std::forward(args))...>{
		std::lock_guard<Mutex> lock(m);
		return std::invoke(c,std::forward(args)...);
	};
}

#endif /* INCLUDE_CALLABLE_HPP_ */
