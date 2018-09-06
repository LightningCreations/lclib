#ifndef __NBTList_hpp_2018_08_26_21_40
#define __NBTList_hpp_2018_08_26_21_40
#include <nbt/NBTBase.hpp>
#include <vector>
#include <initializer_list>
#include <type_traits>
namespace nbt{
    class NBTTagList:public NBTTagBase{
    private:
        std::vector<NbtWrapper> underlying;
        uint8_t listTagType;
    public:
        typedef NbtWrapper value_type;
        typedef std::vector<value_type> vector_type;
        typedef vector_type::pointer pointer;
        typedef vector_type::const_pointer const_pointer;
        typedef vector_type::reference reference;
        typedef vector_type::const_reference const_reference;
        typedef vector_type::iterator iterator;
        typedef vector_type::const_iterator const_iterator;
        typedef vector_type::reverse_iterator reverse_iterator;
        typedef vector_type::const_reverse_iterator const_reverse_iterator;
        typedef vector_type::size_type size_type;
        NBTTagList();
        template<typename T,typename=std::enable_if_t<std::is_base_of_v<NBTTagBase,T>>>
            NBTTagList(const std::initializer_list<T>& t):listTagType(TAG_END){
                for(const T& val:t)
                    add(val);
            }
        template<typename T,typename=std::enable_if_t<std::is_base_of_v<NBTTagBase,T>>>
            bool add(const T& t){
                if(t.getTagType()==TAG_END||(listTagType!=TAG_END&&t.getTagType()!=listTagType))
                    return false;
                if(listTagType==TAG_END)
                    listTagType = t.getTagType();
                underlying.emplace_back(PolymorphicWrapper(t));
            }
        LIBLCFUNC pointer data();
        LIBLCFUNC const_pointer data()const;
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
        LIBLCFUNC reference operator[](size_type i);
        LIBLCFUNC const_reference operator[](size_type i)const;
        LIBLCFUNC reference at(size_type i);
        LIBLCFUNC const_reference at(size_type i)const;
        LIBLCFUNC size_type size()const;
        LIBLCFUNC void clear();
        LIBLCFUNC uint8_t getListTagType()const noexcept(true);
        LIBLCFUNC int32_t getTagType()const noexcept(true);
        LIBLCFUNC int32_t hashCode()const;
        LIBLCFUNC void write(DataOutputStream&)const;
        LIBLCFUNC void read(DataInputStream&);
    };
}


#endif
