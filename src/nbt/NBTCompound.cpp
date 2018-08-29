#include <nbt/NBTBase.hpp>
#include <nbt/NBTArray.hpp>
#include <nbt/NBTPrimitive.hpp>
#include <nbt/NBTList.hpp>
#include <nbt/NBTCompound.hpp>

using namespace nbt;
using namespace std::string_literals;
const std::string empty{};
void NBTTagEnd::write(DataOutputStream&)const{}
void NBTTagEnd::read(DataInputStream&){}
int32_t NBTTagEnd::getTagType()const{
    return TAG_END;
}

const NBTPrimitive& NBTTagCompound::getPrimitive(const std::string& s)const{
    return underlying.at(s).checkedcast<NBTPrimitive>();
}

NBTTagCompound::mapped_type& NBTTagCompound::operator[](const std::string& s){
    return underlying[s];
}
const NBTTagCompound::mapped_type& NBTTagCompound::operator[](const std::string& s)const{
    return underlying.at(s);
}
bool NBTTagCompound::hasTag(const std::string& s)const{
    return underlying.count(s)!=0;
}
bool NBTTagCompound::hasTag(const std::string& s,uint8_t tagType)const{
    if(!hasTag(s))
        return false;
    else if(tagType==TAG_ANY_NEUMERIC)
        return underlying.at(s).instanceof<NBTPrimitive>();
    else
        return underlying.at(s)->getTagType()==tagType;
}
NBTTagCompound::iterator NBTTagCompound::begin(){
    return underlying.begin();
}
NBTTagCompound::const_iterator NBTTagCompound::begin()const{
    return underlying.begin();
}
NBTTagCompound::const_iterator NBTTagCompound::cbegin()const{
    return underlying.cbegin();
}
NBTTagCompound::iterator NBTTagCompound::end(){
    return underlying.end();
}
NBTTagCompound::const_iterator NBTTagCompound::end()const{
    return underlying.end();
}
NBTTagCompound::const_iterator NBTTagCompound::cend()const{
    return underlying.cend();
}
NBTTagCompound::reverse_iterator NBTTagCompound::rbegin(){
    return underlying.rbegin();
}
NBTTagCompound::const_reverse_iterator NBTTagCompound::rbegin()const{
    return underlying.rbegin();
}
NBTTagCompound::const_reverse_iterator NBTTagCompound::crbegin()const{
    return underlying.crbegin();
}
NBTTagCompound::reverse_iterator NBTTagCompound::rend(){
    return underlying.rend();
}
NBTTagCompound::const_reverse_iterator NBTTagCompound::rend()const{
    return underlying.rend();
}
NBTTagCompound::const_reverse_iterator NBTTagCompound::crend()const{
    return underlying.crend();
}
bool NBTTagCompound::getBoolean(const std::string& s)const{
    if(hasTag(s,TAG_ANY_NEUMERIC))
        return getPrimitive(s).getBoolean();
    return false;
}
int8_t NBTTagCompound::getByte(const std::string& s)const{
    if(hasTag(s,TAG_ANY_NEUMERIC))
        return getPrimitive(s).getByte();
    return 0;
}
int16_t NBTTagCompound::getShort(const std::string& s)const{
    if(hasTag(s,TAG_ANY_NEUMERIC))
        return getPrimitive(s);
    return 0;
}
int32_t NBTTagCompound::getInt(const std::string& s)const{
    if(hasTag(s,TAG_ANY_NEUMERIC))
        return getPrimitive(s);
    return 0;
}
int64_t NBTTagCompound::getLong(const std::string& s)const{
    if(hasTag(s,TAG_ANY_NEUMERIC))
        return getPrimitive(s);
    return 0;
}
float NBTTagCompound::getFloat(const std::string& s)const{
    if(hasTag(s,TAG_ANY_NEUMERIC))
        return getPrimitive(s);
    return 0;
}
double NBTTagCompound::getDouble(const std::string& s)const{
    if(hasTag(s,TAG_ANY_NEUMERIC))
        return getPrimitive(s);
    return 0;
}
Version NBTTagCompound::getVersion(const std::string& s)const{
    if(hasTag(s,TAG_ANY_NEUMERIC))
        return Version(getPrimitive(s));
    return 0;
}
UUID NBTTagCompound::getUUID(const std::string& s)const{
    std::string most = s+"Most"s;
    std::string least = s+"Least"s;
    return UUID(getLong(most),getLong(least));
}
const std::string& NBTTagCompound::getString(const std::string& s)const{
    if(hasTag(s,TAG_STRING))
        return underlying.at(s).checkedcast<NBTTagString>();
    return empty;
}
void NBTTagCompound::setBoolean(const std::string& s,bool b){
    underlying[s]=NBTTagByte(b?1:0);
}
void NBTTagCompound::setByte(const std::string& s,int8_t b){
    underlying[s]=NBTTagByte(b);
}
void NBTTagCompound::setShort(const std::string& s,int16_t i){
    underlying[s]=NBTTagShort(i);
}
void NBTTagCompound::setInt(const std::string& s,int32_t i){
    underlying[s]=NBTTagInt(i);
}
void NBTTagCompound::setLong(const std::string& s,int64_t l){
    underlying[s]=NBTTagLong(l);
}
void NBTTagCompound::setFloat(const std::string& s,float f){
    underlying[s]=NBTTagFloat(f);
}
void NBTTagCompound::setDouble(const std::string& s,double d){
    underlying[s]=NBTTagDouble(d);
}
void NBTTagCompound::setVersion(const std::string& s,Version v){
    underlying[s]=NBTTagShort(v.getEncoded());
}
void NBTTagCompound::setUUID(const std::string& s,UUID u){
    std::string most = s+"Most"s;
    std::string least = s+"Least"s;
    setLong(most,u.getHigh());
    setLong(least,u.getLow());
}
void NBTTagCompound::setString(const std::string& s,const std::string& str){
    underlying[s]=NBTTagString(str);
}
void NBTTagCompound::setString(const std::string& s,std::string&& str){
    underlying[s]=NBTTagString(str);
}
int32_t NBTTagCompound::getTagType()const{
    return TAG_COMPOUND;
}
int32_t NBTTagCompound::hashCode()const{
    return hashcode(underlying)*31+NBTTagBase::hashCode();
}
void NBTTagCompound::read(DataInputStream& i){
    underlying.clear();
    std::string name;
    NbtWrapper w;
    while(!(w=readCompoundTag(i,name)).instanceof<NBTTagEnd>())
        underlying[name]=std::move(w);
}
void NBTTagCompound::write(DataOutputStream& o)const{
    for(const_reference p:underlying){
        o << uint8_t(p.second->getTagType()) << p.first;
        p.second->write(o);
    }
    o << TAG_END << empty;
}
