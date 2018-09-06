#ifndef __NBTPrimitive__hpp_2018_08_26_21_36
#define __NBTPrimitive__hpp_2018_08_26_21_36
#include <nbt/NBTBase.hpp>
namespace nbt{
    class NBTPrimitive:public NBTTagBase{
    public:
        virtual int32_t getInt()const noexcept(true)=0;
        LIBLCFUNC virtual int64_t getLong()const noexcept(true);
        LIBLCFUNC int16_t getShort()const noexcept(true);
        LIBLCFUNC int8_t getByte()const noexcept(true);
        LIBLCFUNC bool getBoolean()const noexcept(true);
        LIBLCFUNC virtual float getFloat()const noexcept(true);
        LIBLCFUNC virtual double getDouble()const noexcept(true);
        LIBLCFUNC operator int()const noexcept(true);
        LIBLCFUNC operator int64_t()const noexcept(true);
        LIBLCFUNC operator int16_t()const noexcept(true);
        LIBLCFUNC operator int8_t()const noexcept(true);
        LIBLCFUNC explicit operator bool()const noexcept(true);
        LIBLCFUNC operator float()const noexcept(true);
        LIBLCFUNC operator double()const noexcept(true);
    };
    namespace detail{
        template<typename T,uint8_t tag_type> class NBTTagPrimitive:public NBTPrimitive{
        private:
            T value;
        public:
            NBTTagPrimitive(T val):value{val}{}
            template<typename U,uint8_t t> NBTTagPrimitive(NBTTagPrimitive<U,t> val) noexcept(true):value(val.val){}
            NBTTagPrimitive()=default;
            int32_t getInt()const noexcept(true){
                return int32_t(value);
            }
            int64_t getLong()const noexcept(true){
                return int64_t(value);
            }
            float getFloat()const noexcept(true){
                return float(value);
            }
            double getDouble()const noexcept(true){
                return double(value);
            }
            int32_t getTagType()const noexcept(true){
                return tag_type;
            }
            int32_t hashCode()const{
                return hashcode(value)*31+tag_type;
            }
            void read(DataInputStream& i){
                i >> value;
            }
            void write(DataOutputStream& o)const{
                o << value;
            }

        };
    }
    using NBTTagByte = detail::NBTTagPrimitive<int8_t,TAG_BYTE>;
    using NBTTagShort = detail::NBTTagPrimitive<int16_t,TAG_SHORT>;
    using NBTTagInt = detail::NBTTagPrimitive<int32_t,TAG_INT>;
    using NBTTagLong = detail::NBTTagPrimitive<int64_t,TAG_LONG>;
    using NBTTagFloat = detail::NBTTagPrimitive<float,TAG_FLOAT>;
    using NBTTagDouble = detail::NBTTagPrimitive<double,TAG_DOUBLE>;
}


#endif
