#ifndef __NBTArray_hpp_2018_08_26_21_37
#define __NBTArray_hpp_2018_08_26_21_37
#include <nbt/NBTBase.hpp>
#include <algorithm>
#include <iterator>
#include <stdexcept>
namespace nbt{
    namespace detail{
    template<typename T,uint8_t tag_type> class NBTArray:public NBTTagBase{
        private:
            T* arr;
            std::size_t _size;
        public:
            typedef T value_type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef T* iterator;
            typedef const T* const_iterator
            typedef std::reverse_iterator<iterator> reverse_iterator;
            typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
            typedef std::size_t size_type;
            
            NBTArray():arr(nullptr),_size(0){}
            NBTArray(std::size_t size):arr(new T[size]),_size(size){}
            template<std::size_t N> NBTArray(const T(&arr)[N]):arr(new T[N]),_size(N){
                std::copy(std::begin(arr),std::end(arr),this->arr);
            }
            template<std::size_t N> NBTArray(const std::array<T,N> arr&):arr(new T[N]),_size(N){
                std::copy(std::begin(arr),std::end(arr),this->arr);
            }
            template<typename U,std::size_t N,typename=std::enable_if_t<std::is_convertible_v<U,T>>> NBTArray(const U(&arr)[N]):arr(new T[N]),_size(N){
                std::copy(std::begin(arr),std::end(arr),this->arr);
            }
            template<typename U,std::size_t N,typename=std::enable_if_t<std::is_convertible_v<U,T>>> NBTArray(const std::array<U,N> arr&):arr(new T[N]),_size(N){
                std::copy(std::begin(arr),std::end(arr),this->arr);
            }
            template<class Iterator,typename=std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iterator>::value_type,T>>> 
            NBTArray(Iterator begin,Iterator end):arr(new T[std::distance(begin,end)]),_size(std::distance(begin,end)){
                std::copy(begin,end,arr);
            }
            NBTArray(const NBTArray& arr):arr(new T[arr._size]),_size(arr._size){
                std::copy(std::begin(arr),std::end(arr),this->arr);
            }
            
            NBTArray(NBTArray&& arr)noexcept(true):arr(std::exchange(arr.arr,nullptr)),size(arr._size){}
            NBTArray(const NBTArray&&)=delete;
            NBTArray& operator=(const NBTArray& _arr){
                if(this==&arr)
                    return *this;
                if(this->arr!=nullptr)
                    delete[] arr;
                arr = new T[_arr._size];
                _size = _arr._size;
                std::copy(std::begin(_arr),std::end(_arr),arr);
                return *this;
            }
            NBTArray& operator=(NBTArray&& _arr)noexcept(true){
                if(this->arr!=nullptr)
                    delete[] arr;
                arr = std::exchange(_arr.arr,nullptr);
                _size = _arr._size;
                return *this;
            }
            NBTArray& operator=(const NBTArray&&)=delete;

            ~NBTArray(){
                if(arr!=nullptr)
                    delete[] arr;
            }
            void swap(NBTArray& _arr)noexcept(true){
                std::swap(arr,_arr.arr);
                std::swap(_size,_arr._size);
            }
            iterator begin()noexcept{
                return arr;
            }
            iterator end()noexcept{
                return arr+_size;
            }
            const_iterator begin()const noexcept{
                return arr;
            }
            const_iterator end()const noexcept{
                return arr+_size;
            }
            const_iterator cbegin()const noexcept{
                return arr;
            }
            const_iterator cend()const noexcept{
                return arr+_size;
            }
            reverse_iterator rbegin()noexcept{
                return std::make_reverse_iterator(end());
            }
            reverse_iterator rend()noexcept{
                return std::make_reverse_iterator(begin());
            }
            reverse_iterator rbegin()const noexcept{
                return std::make_reverse_iterator(end());
            }
            reverse_iterator rend()const noexcept{
                return std::make_reverse_iterator(begin());
            }
            reverse_iterator crbegin()const noexcept{
                return std::make_reverse_iterator(end());
            }
            reverse_iterator crend()const noexcept{
                return std::make_reverse_iterator(begin());
            }
            const_pointer data()const noexcept{
                return arr;
            }
            pointer data()noexcept{
                return arr;
            }
            pointer release()noexcept{
                return std::exchange(arr,nullptr);
            }
            reference operator[](size_type s)noexcept{
                return arr[s];
            }
            const_reference operator[](size_type s)const noexcept{
                return arr[s];
            }
            reference at(size_type s){
                return s>=_size?arr[s]:throw std::out_of_range("size input is out of range");
            }
            const_reference at(size_type s)const{
                return s>=_size?arr[s]:throw std::out_of_range("size input is out of range");
            }
            size_type size()const{
                return _size;
            }
            void read(DataInputStream& d){
                if(arr!=nullptr)
                    delete[] arr;
                d >> _size;
                arr = new T[_size];
                for(T& t:*this)
                    d >> t;
            }
            void write(DataOutputStream& d)const{
                d << _size;
                for(T t:*this)
                    d << t;
            }
            int32_t hashCode()const{
                int32_t hash = 0;
                for(T t:*this){
                    hash *= 31;
                    hash += hashcode(t);
                }
                return hash*31+NBTTagBase::hashCode();
            }
            int32_t getTagType()const{
                return tag_type;
            }
        };
    }
    using NBTTagByteArray = detail::NBTArray<int8_t,TAG_BYTE_ARRAY>;
    using NBTTagIntArray = detail::NBTArray<int32_t,TAG_INT_ARRAY>;
    using NBTTagLongArray = detail::NBTArray<int64_t,TAG_LONG_ARRAY>;
}
#endif