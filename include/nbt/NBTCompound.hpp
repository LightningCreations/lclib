#ifndef __NBTCompound_hpp_2018_08_26_21_41
#define __NBTCompound_hpp_2018_08_26_21_41
#include <nbt/NBTBase.hpp>
#include <nbt/NBTPrimitive.hpp>
#include <UUID.hpp>
#include <Version.hpp>
namespace nbt{
    class NBTTagEnd:public NBTTagBase{
    public:
    	LIBLCFUNC void write(DataOutputStream&)const;
    	LIBLCFUNC void read(DataInputStream&);
    	LIBLCFUNC int32_t getTagType()const;
    };
    class NBTTagCompound:public NBTTagBase{
    private:
        std::map<std::string,NbtWrapper> underlying;
        const NBTPrimitive& getPrimitive(const std::string&)const;
    public:
        typedef std::map<std::string,NbtWrapper> map_type;
        typedef map_type::value_type value_type;
        typedef map_type::reference reference;
        typedef map_type::const_reference const_reference;
        typedef map_type::pointer pointer;
        typedef map_type::const_pointer const_pointer;
        typedef map_type::iterator iterator;
        typedef map_type::const_iterator const_iterator;
        typedef map_type::reverse_iterator reverse_iterator;
        typedef map_type::const_reverse_iterator const_reverse_iterator;
        typedef map_type::key_type key_type;
        typedef map_type::mapped_type mapped_type;
        
        NBTTagCompound()=default;
        NBTTagCompound(const NBTTagCompound&)=delete;
        NBTTagCompound(NBTTagCompound&&)=default;
        NBTTagCompound& operator=(const NBTTagCompound&)=delete;
        NBTTagCompound& operator=(NBTTagCompound&&)=default;
        LIBLCFUNC mapped_type& operator[](const std::string&);
        LIBLCFUNC const mapped_type& operator[](const std::string&)const;
        LIBLCFUNC bool hasTag(const std::string&)const;
        LIBLCFUNC bool hasTag(const std::string&,uint8_t)const;
        LIBLCFUNC iterator begin();
        LIBLCFUNC const_iterator begin()const;
        LIBLCFUNC const_iterator cbegin()const;
        LIBLCFUNC iterator end();
        LIBLCFUNC const_iterator end()const;
        LIBLCFUNC const_iterator cend()const;
        LIBLCFUNC reverse_iterator rbegin();
        LIBLCFUNC const_reverse_iterator rbegin()const;
        LIBLCFUNC const_reverse_iterator crbegin()const;
        LIBLCFUNC reverse_iterator rend();
        LIBLCFUNC const_reverse_iterator rend()const;
        LIBLCFUNC const_reverse_iterator crend()const;
        LIBLCFUNC bool getBoolean(const std::string&)const;
        LIBLCFUNC int8_t getByte(const std::string&)const;
        LIBLCFUNC int16_t getShort(const std::string&)const;
        LIBLCFUNC int32_t getInt(const std::string&)const;
        LIBLCFUNC int64_t getLong(const std::string&)const;
        LIBLCFUNC float getFloat(const std::string&)const;
        LIBLCFUNC double getDouble(const std::string&)const;
        LIBLCFUNC Version getVersion(const std::string&)const;
        LIBLCFUNC UUID getUUID(const std::string&)const;
        LIBLCFUNC const std::string& getString(const std::string&)const;
        LIBLCFUNC void setBoolean(const std::string&,bool);
        LIBLCFUNC void setByte(const std::string&,int8_t);
        LIBLCFUNC void setShort(const std::string&,int16_t);
        LIBLCFUNC void setInt(const std::string&,int32_t);
        LIBLCFUNC void setLong(const std::string&,int64_t);
        LIBLCFUNC void setFloat(const std::string&,float);
        LIBLCFUNC void setDouble(const std::string&,double);
        LIBLCFUNC void setVersion(const std::string&,Version);
        LIBLCFUNC void setUUID(const std::string&,UUID);
        LIBLCFUNC void setString(const std::string&,const std::string&);
        LIBLCFUNC void setString(const std::string&,std::string&&);
        LIBLCFUNC int32_t getTagType()const noexcept(true);
        LIBLCFUNC int32_t hashCode()const;
        LIBLCFUNC void read(DataInputStream&);
        LIBLCFUNC void write(DataOutputStream&)const;
    };
}

#endif
