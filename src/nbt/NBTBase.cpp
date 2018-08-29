#include <nbt/NBTBase.hpp>
#include <cstdint>

using namespace nbt;

int32_t NBTTagBase::hashCode()const{
    return getTagType();
}

NBTTagString::NBTTagString(const char* c):value(c){}
NBTTagString::NBTTagString(const std::string& s):value(s){}
NBTTagString::NBTTagString(std::string&& s):value(s){}

NBTTagString::operator const std::string&()const{
    return value;
}
NBTTagString::operator std::string&&()&&{
    return std::move(value);
}

void NBTTagString::write(DataOutputStream& d)const{
    d << value;
}
void NBTTagString::read(DataInputStream& d){
    d >> value;
}

int32_t NBTTagString::hashCode()const{
    return hashcode(value)*31+NBTTagBase::hashCode();
}

int32_t NBTTagString::getTagType()const{
    return TAG_STRING;
}
