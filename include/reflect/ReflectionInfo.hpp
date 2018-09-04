/*
 * ReflectionInfo.hpp
 *
 *  Created on: Sep 1, 2018
 *      Author: connor
 */

#ifndef INCLUDE_REFLECT_REFLECTIONINFO_HPP_2018_09_01_22_56
#define INCLUDE_REFLECT_REFLECTIONINFO_HPP_2018_09_01_22_56
#include <type_traits>
//#define _REFLECT_DISABLE
#ifndef _REFLECT_DISABLE
#define export_field(field) template struct reflect::field_reflection_info<decltype((field)),field>;
#define export_function(function) template struct reflect::function_reflection_info<decltype(function),function>;
#define export_type(type) template struct reflect::type_reflection_info<type>;
#define export_enumerator(enumerator) template struct reflect::enumerator_reflection_info<decltype(enumerator),enumerator>;
#define export_constructor(cl,...) template struct reflect::constructor_reflection_info<cl,##__VA_ARGS__>;
#else
#define export_field(field)
#define export_function(function)
#define export_type(type)
#define export_enumerator(enumerator)
#define export_constructor(cl,...)
#endif
namespace reflect{
	template<typename T,T& field> struct field_reflection_info{
		//TODO add some sort of export to the binary in an implementation defined way
		//To export the symbol's reflection information
		//On SNES-OS it uses the intrinsic __export_reflection_info(field)
		//UB if the target is an auto, or a thread_local variable
	};

	template<typename T,T& method> struct function_reflection_info{
		//TODO add some sort of export to the binary in an implementation defined way
		//UB if the target is not a function
	};
	template<typename T> struct type_reflection_info{
		//TODO add some sort of export to the binary for reflection information
	};
	template<typename E,E val> struct enumerator_reflection_info{
		//TODO add some sort of export of reflection information
		//UB if the target is not an enum type
	};
	template<class target,typename... Args> struct constructor_reflection_info{
		//TODO export reflection info
		//UB if the class does not have a constructor with the give args types
		//(Note that if its defined but defined as defaulted its not UB)
	};

	struct reflected_field{
	public:
		reflected_field(const char*);
		const char* getName()const;
		const char* getDecltypeName()const;
	};
	struct reflected_method{
	public:
		reflected_method(const char*);
		const char* getName()const;
		const char* getUndecoratedName()const;
		const char* getDecltype()const;
		const char** getParameterTypes()const;
		const char* getReturnType()const;
		const char* getNameAndSignature()const;
		const char* getLinkageMode()const;
		const char* getExceptionSpecification()const;
	};

};


#endif /* INCLUDE_REFLECT_REFLECTIONINFO_HPP_ */
