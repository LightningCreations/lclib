#include <nbt/NBTPrimitive.hpp>

using namespace nbt;

int64_t NBTPrimitive::getLong()const{
    return int64_t(getInt());
}
int16_t NBTPrimitive::getShort()const{
    return int16_t(getInt());
}
int8_t NBTPrimitive::getByte()const{
    return int8_t(getInt());
}
bool NBTPrimitive::getBoolean()const{
    return getInt()!=0;
}
float NBTPrimitive::getFloat()const{
    return float(getInt());
}
double NBTPrimitive::getDouble()const{
    return double(getInt());
}
NBTPrimitive::operator int()const{
    return getInt();
}
NBTPrimitive::operator int64_t()const{
    return getLong();
}
NBTPrimitive::operator int16_t()const{
    return getShort();
}
NBTPrimitive::operator int8_t()const{
    return getByte();
}
NBTPrimitive::operator bool()const{
    return getBoolean();
}
NBTPrimitive::operator float()const{
    return getFloat();
}
NBTPrimitive::operator double()const{
    return getDouble();
}