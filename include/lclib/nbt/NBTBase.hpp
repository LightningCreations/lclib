#ifndef __NBTTags_hpp_2018_08_26_00_42
#define __NBTTags_hpp_2018_08_26_00_42
#include <Hash.hpp>
#include <IOWrapper.hpp>
#include <string>
#include <cstdint>
#include <Wrappers.hpp>
namespace nbt{
    class NBTTagBase:public Hashable{
    public:
        virtual ~NBTTagBase()=default;
        virtual void write(DataOutputStream&)const=0;
        virtual void read(DataInputStream&)=0;
        virtual int32_t getTagType()const noexcept(true)=0;
        LIBLCFUNC int32_t hashCode()const;
    };
    

    class NBTTagString:public NBTTagBase{
    private:
        std::string value;
    public:
        LIBLCFUNC NBTTagString(const char*);
        LIBLCFUNC NBTTagString(const std::string&);
        LIBLCFUNC NBTTagString(std::string&&);
        NBTTagString()=default;
        LIBLCFUNC operator const std::string&()const noexcept(true);
        LIBLCFUNC operator std::string&&()&& noexcept(true);
        LIBLCFUNC const std::string& getValue()const& noexcept(true);
        LIBLCFUNC std::string&& getValue()&& noexcept(true);
        LIBLCFUNC void write(DataOutputStream&)const;
        LIBLCFUNC void read(DataInputStream&);
        LIBLCFUNC int32_t hashCode()const;
        LIBLCFUNC int32_t getTagType()const noexcept(true);
    };
    
    constexpr const uint8_t TAG_END{0};
    constexpr const uint8_t TAG_BYTE{1};
    constexpr const uint8_t TAG_SHORT{2};
    constexpr const uint8_t TAG_INT{3};
    constexpr const uint8_t TAG_LONG{4};
    constexpr const uint8_t TAG_FLOAT{5};
    constexpr const uint8_t TAG_DOUBLE{6};
    constexpr const uint8_t TAG_BYTE_ARRAY{7};
    constexpr const uint8_t TAG_STRING{8};
    constexpr const uint8_t TAG_LIST{9};
    constexpr const uint8_t TAG_COMPOUND{10};
    constexpr const uint8_t TAG_INT_ARRAY{11};
    constexpr const uint8_t TAG_LONG_ARRAY{12};
    constexpr const uint8_t TAG_ANY_NEUMERIC{99};
    using NbtWrapper = PolymorphicWrapper<NBTTagBase>;

    LIBLCFUNC NbtWrapper readListTag(DataInputStream&,uint8_t);
    LIBLCFUNC NbtWrapper readCompoundTag(DataInputStream&,std::string&);
}
#endif
