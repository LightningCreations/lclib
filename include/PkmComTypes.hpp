#ifndef __PkmComTypes_hpp_2018_08_22_22_24
#define __PkmComTypes_hpp_2018_08_22_22_24
#include <string>
#include <json/json.h>
#include <cstdint>
#include <Version.hpp>
#include <UUID.hpp>
#include <JTime.hpp>
#include <type_traits>
#include <vector>
#include <TypeTraits.hpp>

using std::void_t;

namespace pkmcom{

    template<typename T> struct pkmcom_traits{
    };
    typedef int32_t pkmcom_hash;
    typedef uint32_t pkmcom_size;
    typedef bool pkmcom_bool;
    typedef uint8_t pkmcom_byte;
    typedef uint16_t pkmcom_ushort;
    typedef uint32_t pkmcom_uint;
    typedef int8_t pkmcom_sbyte;
    typedef int16_t pkmcom_short;
    typedef int32_t pkmcom_int;
    typedef int64_t pkmcom_long;
    typedef std::string pkmcom_string;
    typedef Json::Value pkmcom_json;
    typedef UUID pkmcom_uuid;
    typedef Version pkmcom_version;
    typedef Instant pkmcom_instant;
    typedef Duration pkmcom_duration;
    typedef float pkmcom_float;
    typedef double pkmcom_double;

    class PacketBuffer{
    private:
        vector<char> buffer;
        int pos=0;
    public:
        void writeByte(uint8_t);
        uint8_t readByte();
        template<typename T> void_t<decltype(pkmcom_traits<T>::hashcode(std::declval<T>())),std::enable_if_t<!std::is_trivially_copyable_v<T>>> write(const T& t){
            pkmcom_traits<T>::write(t,*this);
        }
        template<typename T> void_t<decltype(pkmcom_traits<T>::hashcode(std::declval<T>())),std::enable_if_t<std::is_trivially_copyable_v<T>>> write(T t){
            pkmcom_traits<T>::write(t,*this);
        }
        template<typename T> void_t<decltype(pkmcom_traits<T>::hashcode(std::declval<T>()))> read(T& t){
            pkmcom_traits<T>::read(t,*this);
        }
        const char* getBuffer()const;
        pkmcom_size getSize()const;
    };

    template<typename E,typename=std::enable_if_t<std::is_enum_v<E>>> struct pkmcom_traits<E>{
    public:
        using underlying_type = std::underlying_type_t<E>;
        using underlying_traits = pkm_traits<underlying_type>;
        constexpr pkmcom_hash hashcode(E val){
            return underlying_traits.hashcode(static_cast<underlying_type>(val));
        }
        constexpr pkmcom_size size(E val){
            return underlying_traits.size(static_cast<underlying_type>(val));
        }
    };

    template<> struct pkmcom_traits<pkmcom_bool>{
    public:
        constexpr pkmcom_hash hashcode(pkmcom_bool b){
            return b?1337:1331;
        }
        constexpr pkmcom_size size(pkmcom_bool b){
            return 1;
        }
        void write(pkmcom_bool b,PacketBuffer& buff){
            buff.writeByte(b?1:0);
        }
        void read(pkmcom_bool& b,PacketBuffer& buff){
            b = buff.readByte()!=0;
        }
    };

}
#endif