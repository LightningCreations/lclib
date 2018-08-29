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
        pointer data();
        const_pointer data()const;
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
        reference operator[](size_type i);
        const_reference operator[](size_type i)const;
        reference at(size_type i);
        const_reference at(size_type i)const;
        size_type size()const;
        void clear();
        uint8_t getListTagType()const;
        int32_t getTagType()const;
        int32_t hashCode()const;
        void write(DataOutputStream&)const;
        void read(DataInputStream&);
    };
}


#endif