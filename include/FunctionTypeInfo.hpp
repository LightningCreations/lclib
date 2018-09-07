/*
 * FunctionTypeInfo.hpp
 *
 *  Created on: Sep 7, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_FUNCTIONTYPEINFO_HPP__2018_09_07_15_53
#define __INCLUDE_FUNCTIONTYPEINFO_HPP__2018_09_07_15_53
#include <TypeInfo.hpp>
namespace types{
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...),Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......),Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^LevelModifier::CONST*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^LevelModifier::CONST*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)volatile,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<Ret>{}()*Level::FUNCTION^LevelModifier::VOLATILE*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)volatile,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<Ret>{}()*Level::FUNCTION^LevelModifier::VOLATILE*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const volatile,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^LevelModifier::CV*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const volatile,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^LevelModifier::CV*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^Level::REFERENCE^(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^Level::REFERENCE^(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^Level::REFERENCE^LevelModifier::CONST*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(Level::REFERENCE^LevelModifier::CONST)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)volatile&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<Ret>{}()*Level::FUNCTION^(Level::REFERENCE^LevelModifier::VOLATILE)*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)volatile&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<Ret>{}()*Level::FUNCTION^(Level::REFERENCE^LevelModifier::VOLATILE)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const volatile&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(Level::REFERENCE^LevelModifier::CV)*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const volatile&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*(Level::REFERENCE^LevelModifier::CV)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)&&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^Level::XVALUE^(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)&&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^Level::XVALUE^(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const&&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(Level::XVALUE^LevelModifier::CONST)*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const&&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(Level::XVALUE^LevelModifier::CONST)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)volatile&&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<Ret>{}()*Level::FUNCTION^(Level::XVALUE^LevelModifier::VOLATILE)*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)volatile&&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<Ret>{}()*Level::FUNCTION^(Level::XVALUE^LevelModifier::VOLATILE)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const volatile&&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(Level::XVALUE^LevelModifier::CV)*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
		}
	};
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const volatile&&,Ret,Args...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return (TypeHash<Ret>{}()^Level::FUNCTION)*(Level::XVALUE^LevelModifier::CV)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
		}
	};

	//noexcept starts from here
	template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^LevelModifier::CONST*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^LevelModifier::CONST*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)volatile noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return TypeHash<Ret>{}()*Level::FUNCTION^LevelModifier::VOLATILE*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)volatile noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return TypeHash<Ret>{}()*Level::FUNCTION^LevelModifier::VOLATILE*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const volatile noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^LevelModifier::CV*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const volatile noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^LevelModifier::CV*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^Level::REFERENCE^(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^Level::REFERENCE^(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^Level::REFERENCE^LevelModifier::CONST*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(Level::REFERENCE^LevelModifier::CONST)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)volatile&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return TypeHash<Ret>{}()*Level::FUNCTION^(Level::REFERENCE^LevelModifier::VOLATILE)*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)volatile&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return TypeHash<Ret>{}()*Level::FUNCTION^(Level::REFERENCE^LevelModifier::VOLATILE)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const volatile&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(Level::REFERENCE^LevelModifier::CV)*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const volatile&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*(Level::REFERENCE^LevelModifier::CV)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)&&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^Level::XVALUE^(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)&&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^Level::XVALUE^(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const&&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(Level::XVALUE^LevelModifier::CONST)*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const&&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(Level::XVALUE^LevelModifier::CONST)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)volatile&&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return TypeHash<Ret>{}()*Level::FUNCTION^(Level::XVALUE^LevelModifier::VOLATILE)*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)volatile&&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return TypeHash<Ret>{}()*Level::FUNCTION^(Level::XVALUE^LevelModifier::VOLATILE)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args...)const volatile&&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*TypeCode::LEVEL^(Level::XVALUE^LevelModifier::CV)*(TypeHash<Args>(){} + ...);//Ignore This, valid code (C++17 Fold Expression)
			}
		};
		template<typename Ret,typename... Args> struct TypeHash<enable_specialization_t<Ret(Args......)const volatile&&noexcept,Ret,Args...>>{
		public:
			constexpr TypeHash()=default;
			constexpr TypeHash(const TypeHash&)=default;
			constexpr TypeHash(TypeHash&&)=default;
			TypeHash(const TypeHash&&)=delete;
			constexpr TypeHash& operator=(const TypeHash&)=default;
			constexpr TypeHash& operator=(TypeHash&&)=default;
			TypeHash& operator=(const TypeHash&&)=delete;
			constexpr TypeCode operator()()const{
				return (TypeHash<Ret>{}()^Level::FUNCTION)*(Level::XVALUE^LevelModifier::CV)*(TypeHash<Args>(){} + ...)^TypeCode::VARARGS;//Ignore This, valid code (C++17 Fold Expression)
			}
		};
}



#endif /* __INCLUDE_FUNCTIONTYPEINFO_HPP__2018_09_07_15_53 */
