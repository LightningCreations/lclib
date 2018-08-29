#include <nbt/NBTList.hpp>
#include <nbt/NBTPrimitive.hpp>
#include <nbt/NBTCompound.hpp>
using namespace nbt;

NBTTagList::pointer NBTTagList::data(){
    return underlying.data();
}
NBTTagList::const_pointer NBTTagList::data()const{
    return underlying.data();
}
NBTTagList::iterator NBTTagList::begin(){
    return underlying.begin();
}
NBTTagList::const_iterator NBTTagList::begin()const{
    return underlying.begin();
}
NBTTagList::const_iterator NBTTagList::cbegin()const{
    return underlying.cbegin();
}
NBTTagList::iterator NBTTagList::end(){
    return underlying.end();
}
NBTTagList::const_iterator NBTTagList::end()const{
    return underlying.end();
}
NBTTagList::const_iterator NBTTagList::cend()const{
    return underlying.cend();
}
NBTTagList::reverse_iterator NBTTagList::rbegin(){
    return underlying.rbegin();
}
NBTTagList::const_reverse_iterator NBTTagList::rbegin()const{
    return underlying.rbegin();
}
NBTTagList::const_reverse_iterator NBTTagList::crbegin()const{
    return underlying.crbegin();
}
NBTTagList::reverse_iterator NBTTagList::rend(){
    return underlying.rend();
}
NBTTagList::const_reverse_iterator NBTTagList::rend()const{
    return underlying.rend();
}
NBTTagList::const_reverse_iterator NBTTagList::crend()const{
    return underlying.crend();
}
NBTTagList::reference NBTTagList::operator[](size_type i){
    return underlying[i];
}
NBTTagList::const_reference NBTTagList::operator[](size_type i)const{
    return underlying[i];
}
NBTTagList::reference NBTTagList::at(size_type i){
    return underlying.at(i);
}
NBTTagList::const_reference NBTTagList::at(size_type i)const{
    return underlying.at(i);
}
NBTTagList::size_type NBTTagList::size()const{
    return underlying.size();
}
void NBTTagList::clear(){
    underlying.clear();
    listTagType = TAG_END;
}
uint8_t NBTTagList::getListTagType()const{
    return listTagType;
}
int32_t NBTTagList::getTagType()const{
    return TAG_LIST;
}
int32_t NBTTagList::hashCode()const{
    return hashcode(underlying)*31+this->NBTTagBase::hashCode();
}
void NBTTagList::write(DataOutputStream& o)const{
    o << listTagType << size();
    for(const NBTTagBase& t:*this)
        t.write(o);
}
void NBTTagList::read(DataInputStream& i){
    uint32_t size;
    i >> listTagType >> size;
    for(uint32_t j=0;j<size;j++)
        underlying.push_back(readListTag(i,listTagType));
}
