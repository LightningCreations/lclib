#ifndef __Operations_hpp_2018_08_07_10_37
#define __Operations_hpp_2018_08_07_10_37

#include <utility>

class NotCopyable{
private:
    NotCopyable(const NotCopyable&)=delete;
    NotCopyable& operator=(const NotCopyable&)=delete;
};

class NotMoveable{
private:
    NotMoveable(NotMoveable&&)=delete;
    NotMoveable& operator=(NotMoveable&&)=delete;
};

class NotAssignable{
private:
    NotAssignable& operator=(const NotAssignable&)=delete;
    NotAssignable& operator=(NotAssignable&&)=delete;
};

class Disable:NotMoveable,NotCopyable{
private:
    Disable()=delete;
};

template<class C,typename T> struct PMRAccessor{
private:
	using class_type = C;
	using target_type = T;
	using member_type = T C::*;
	member_type member;
public:
	PMRAccessor(member_type member) noexcept(true):member(member){}
	target_type& operator()(class_type& c)const noexcept(true){
		return c.*member;
	}
	const target_type& operator()(const class_type& c)const noexcept(true){
		return c.*member;
	}
	target_type&& operator()(class_type&& c)const noexcept(true){
		return c.*member;
	}
};

template<class C,typename T> PMRAccessor(T C::*) -> PMRAccessor<C,T>;

template<class C,typename T> decltype(auto) toAccessorFunction(T C::* target) noexcept(true) ->PMRAccessor<C,T>{
	return target;
}

template<class C,typename Ret,typename... Args> auto toCallable(Ret (C::* pmr)(Args...))noexcept(true){
	return [pmr](C& c,Args&&... args){
		return c.*pmr(std::forward<Args>(args)...);
	};
}
template<class C,typename Ret,typename... Args> auto toCallable(Ret (C::* pmr)(Args...)const)noexcept(true){
	return [pmr](const C& c,Args&&... args){
		return c.*pmr(std::forward<Args>(args)...);
	};
}
template<class C,typename Ret,typename... Args> auto toCallable(Ret (C::* pmr)(Args...)noexcept(true))noexcept(true){
	return [pmr](C& c,Args&&... args)noexcept(true){
		return c.*pmr(std::forward<Args>(args)...);
	};
}

template<class C,typename Ret,typename... Args> auto toCallable(Ret (C::* pmr)(Args...)const noexcept(true))noexcept(true){
	return [pmr](C& c,Args&&... args)noexcept(true){
		return c.*pmr(std::forward<Args>(args)...);
	};
}

namespace _detail{
	template<class C,typename Ret,typename... Args> struct NonConstVarargsCaller{
	private:
		Ret (C::* pmr)(Args......);
	public:
		NonConstVarargsCaller(Ret (C::* pmr)(Args......)) noexcept:pmr(pmr){}
		template<typename... VAArgs> Ret operator()(C& c, Args&&... args,VAArgs&&... vaargs){
			return (c.*pmr)(std::forward(args)...,std::forward(vaargs)...);
		}
	};
	template<class C,typename Ret,typename... Args> struct ConstVarargsCaller{
	private:
		Ret (C::* pmr)(Args......)const;
	public:
		ConstVarargsCaller(Ret (C::* pmr)(Args......)const)noexcept(true):pmr(pmr){}
		template<typename... VAArgs> Ret operator()(const C& c,Args&&... args,VAArgs&&... vaargs){
			return (c.*pmr)(std::forward(args)...,std::forward(vaargs)...);
		}
	};

	template<class C,typename Ret,typename... Args> struct NonConstNoexceptVarargsCaller{
		private:
			Ret (C::* pmr)(Args......)noexcept;
		public:
			NonConstNoexceptVarargsCaller(Ret (C::* pmr)(Args......)noexcept) noexcept:pmr(pmr){}
			template<typename... VAArgs> Ret operator()(C& c, Args&&... args,VAArgs&&... vaargs)noexcept{
				return (c.*pmr)(std::forward(args)...,std::forward(vaargs)...);
			}
		};
		template<class C,typename Ret,typename... Args> struct ConstNoexceptVarargsCaller{
		private:
			Ret (C::* pmr)(Args......)const noexcept(true);
		public:
			ConstVarargsCaller(Ret (C::* pmr)(Args......)const noexcept(true))noexcept(true):pmr(pmr){}
			template<typename... VAArgs> Ret operator()(const C& c,Args&&... args,VAArgs&&... vaargs)noexcept{
				return (c.*pmr)(std::forward(args)...,std::forward(vaargs)...);
			}
		};
}

template<class C,typename Ret,typename... Args> auto toCallable(Ret (C::* pmr)(Args......))noexcept(true)->_detail::NonConstVarargsCaller<C,Args...>{
	return pmr;
}
template<class C,typename Ret,typename... Args> auto toCallable(Ret (C::* pmr)(Args......)const)noexcept(true)->_detail::ConstVarargsCaller<C,Args...>{
	return pmr;
}
template<class C,typename Ret,typename... Args> auto toCallable(Ret (C::* pmr)(Args......) noexcept)noexcept(true)->_detail::NonConstNoexceptVarargsCaller<C,Args...>{
	return pmr;
}
template<class C,typename Ret,typename... Args> auto toCallable(Ret (C::* pmr)(Args......)const noexcept)noexcept(true)->_detail::ConstNoexceptVarargsCaller<C,Args...>{
	return pmr;
}


#endif
