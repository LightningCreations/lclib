
#include <lclib-cxx/nbt/NBTBase.hpp>
#include <lclib-cxx/nbt/NBTPrimitive.hpp>
#include <lclib-cxx/nbt/NBTArray.hpp>
#include <lclib-cxx/nbt/NBTCompound.hpp>
#include <lclib-cxx/nbt/NBTList.hpp>
#include <lclib-cxx/IOWrapper.hpp>
#include <lclib-cxx/Wrappers.hpp>

namespace nbt{
    NbtWrapper readListTag(DataInputStream& i,uint8_t tagType){
        switch(tagType){
        case TAG_END:
            return NBTTagEnd();
        break;
        case TAG_BYTE:
            {
            NBTTagByte b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_SHORT:
            {
            NBTTagShort b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_INT:
            {
            NBTTagInt b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_LONG:
            {
            NBTTagLong b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_FLOAT:
            {
            NBTTagFloat b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_DOUBLE:
            {
            NBTTagDouble b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_BYTE_ARRAY:
            {
            NBTTagByteArray b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_STRING:
            {
            NBTTagString b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_LIST:
            {
            NBTTagList b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_COMPOUND:
            {
            NBTTagCompound b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_INT_ARRAY:
            {
            NBTTagIntArray b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_LONG_ARRAY:
            {
            NBTTagLongArray b;
            b.read(i);
            return std::move(b);
            }
        break;
        }
        return NbtWrapper();//@unreachable@
=======
#include <nbt/NBTBase.hpp>
#include <nbt/NBTPrimitive.hpp>
#include <nbt/NBTArray.hpp>
#include <nbt/NBTCompound.hpp>
#include <nbt/NBTList.hpp>

namespace nbt{
    NbtWrapper readListTag(DataInputStream& i,uint8_t tagType){
        switch(tagType){
        case TAG_END:
            return NBTTagEnd();
        break;
        case TAG_BYTE:
            {
            NBTTagByte b;
            b.read(i);
            return b;
            }
        break;
        case TAG_SHORT:
            {
            NBTTagShort b;
            b.read(i);
            return b;
            }
        break;
        case TAG_INT:
            {
            NBTTagInt b;
            b.read(i);
            return b;
            }
        break;
        case TAG_LONG:
            {
            NBTTagLong b;
            b.read(i);
            return b;
            }
        break;
        case TAG_FLOAT:
            {
            NBTTagFloat b;
            b.read(i);
            return b;
            }
        break;
        case TAG_DOUBLE:
            {
            NBTTagDouble b;
            b.read(i);
            return b;
            }
        break;
        case TAG_BYTE_ARRAY:
            {
            NBTTagByteArray b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_STRING:
            {
            NBTTagString b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_LIST:
            {
            NBTTagList b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_COMPOUND:
            {
            NBTTagCompound b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_INT_ARRAY:
            {
            NBTTagIntArray b;
            b.read(i);
            return std::move(b);
            }
        break;
        case TAG_LONG_ARRAY:
            {
            NBTTagLongArray b;
            b.read(i);
            return std::move(b);
            }
        break;
        }
    }

    NbtWrapper readCompoundTag(DataInputStream& i,std::string& nameOut){
        uint8_t tagType;
        i >> tagType >> nameOut;
        return readListTag(i,tagType);
    }
}
