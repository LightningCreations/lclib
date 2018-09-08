/*
 * Reflection.cpp
 *
 *  Created on: Sep 8, 2018
 *      Author: connor
 */
#include <reflect/ReflectionInfo.hpp>
#include <unordered_map>

namespace std{
	template<> struct hash<types::TypeCode>{
	public:
		constexpr hash()=default;
	};
}
#ifndef __NOREFLECTION
std::unordered_map<types::TypeCode,const reflect::reflection_info_base*> reflectionTable;

namespace reflect{
	void registerReflectionKey(const reflect::reflection_info_base* b,types::TypeCode key,ReflectionKeyType){
		reflectionTable[key] =b;
	}
	const reflect::reflection_info_base* getReflectionKey(types::TypeCode key){
		return reflectionTable[key];
	}
	void registerTypeAlias(types::TypeCode key,const reflect::reflection_info_base* b){
		reflectionTable[key] =b;
	}
}

struct void_type_reflection:reflect::reflection_info_base{
	types::TypeCode hashKey;
	const char* name;
	types::TypeCode baseClass;
	void_type_reflection():name("void"),hashKey(types::TypeHash<void>{}()),
			baseClass{0}{
				fieldOffsets[0] = offsetof(void_type_reflection,name);
				fieldOffsets[1] = offsetof(void_type_reflection,hashKey);
				fieldOffsets[2] = offsetof(void_type_reflection,baseClass);
				reflect::registerReflectionKey(this,hashKey,reflect::TYPE);
				reflect::registerTypeAlias(types::nameHash("void"),this);
			}
};

using reflect::type_reflection_info;

const void_type_reflection __reflect__tvoid{};
const type_reflection_info<char> __reflect__tchar{"char"};
const type_reflection_info<unsigned char> __reflect__tunsigned_char{"unsigned char"};
const type_reflection_info<short> __reflect__tshort{"short"};
const type_reflection_info<unsigned short> __reflect__tunsigned_short{"unsigned short"};
const type_reflection_info<int> __reflect__tint{"int"};
const type_reflection_info<unsigned int> __reflect__tunsigned_int{"unsigned int"};
const type_reflection_info<long> __reflect__tlong{"long"};
const type_reflection_info<unsigned long> __reflect__tunsigned_long{"unsigned long"};
const type_reflection_info<long long> __reflect__tlong_long{"long long"};
const type_reflection_info<unsigned long long> __reflect__tunsigned_long_long{"unsigned long long"};
const type_reflection_info<wchar_t> __reflect__twchar_t{"wchar_t"};
const type_reflection_info<char16_t> __reflect__tchar16_t{"char16_t"};
const type_reflection_info<char32_t> __reflect__tchar32_t{"char32_t"};
const type_reflection_info<decltype(((void*)0)-((void*)0))> __reflect__tptrdiff_t{"ptrdiff_t"};
const type_reflection_info<decltype(nullptr)> __reflect__tnullptr_t{"nullptr_t"};
const type_reflection_info<void*> __reflect__tvoidptr{"void*"};
#else
//Satisfy ODR but don't export the hashtable when Reflection is disabled
namespace reflect{
	void registerReflectionKey(const reflect::reflection_info_base* b,types::TypeCode key,ReflectionKeyType){
	}
	const reflect::reflection_info_base* getReflectionKey(types::TypeCode key){
		return nullptr;
	}
	void registerTypeAlias(types::TypeCode key,const reflect::reflection_info_base* b){
	}
}
#endif

