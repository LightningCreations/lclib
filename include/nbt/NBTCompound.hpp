#ifndef __NBTCompound_hpp_2018_08_26_21_41
#define __NBTCompound_hpp_2018_08_26_21_41
#include <nbt/NBTBase.hpp>
#include <UUID.hpp>
#include <Version.hpp>
namespace nbt{
    class NBTTagEnd:public NBTTagBase{
    public:
        void write(DataOutputStream&)const;
        void read(DataInputStream&);
        int32_t getTagType()const;
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
        NBTTagCompound(const NBTTagCompound&)=default;
        NBTTagCompound(NBTTagCompound&&)=default;
        NBTTagCompound(const NBTTagCompound&&)=delete;
        NBTTagCompound& operator=(const NBTTagCompound&)=default;
        NBTTagCompound& operator=(NBTTagCompound&&)=default;
        NBTTagCompound& operator=(const NBTTagCompound&&)=delete;
        mapped_type& operator[](const std::string&);
        const mapped_type& operator[](const std::string&)const;
        bool hasTag(const std::string&)const;
        bool hasTag(const std::string&,uint8_t)const;
        iterator begin();
        const_iterator begin()const;
        const_iterator cbegin()const;
        iterator end();
        const_iterator end()const;
        const_iterator cend()const;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin()const;
        const_reverse_iterator crbegin()const;
        reverse_iterator rend();
        const_reverse_iterator rend()const;
        const_reverse_iterator crend()const;
        bool getBoolean(const std::string&)const;
        int8_t getByte(const std::string&)const;
        int16_t getShort(const std::string&)const;
        int32_t getInt(const std::string&)const;
        int64_t getLong(const std::string&)const;
        float getFloat(const std::string&)const;
        double getDouble(const std::string&)const;
        Version getVersion(const std::string&)const;
        UUID getUUID(const std::string&)const;
        const std::string& getString(const std::string&)const;
        void setBoolean(const std::string&,bool);
        void setByte(const std::string&,int8_t);
        void setShort(const std::string&,int16_t);
        void setInt(const std::string&,int32_t);
        void setLong(const std::string&,int64_t);
        void setFloat(const std::string&,float);
        void setDouble(const std::string&,double);
        void setVersion(const std::string&,Version);
        void setUUID(const std::string&,UUID);
        void setString(const std::string&,const std::string&);
        void setString(const std::string&,std::string&&);
        int32_t getTagType()const;
        int32_t hashCode()const;
        void read(DataInputStream&);
        void write(DataOutputStream&)const;
    };
}

#endif