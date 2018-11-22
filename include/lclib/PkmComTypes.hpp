#ifndef __PkmComTypes_hpp_2018_08_22_22_24
#define __PkmComTypes_hpp_2018_08_22_22_24
#include <json/json.h>
#include <string>
#include <cstdint>
#include <lclib/Version.hpp>
#include <lclib/UUID.hpp>
#include <lclib/JTime.hpp>
#include <type_traits>
#include <vector>
#include <lclib/TypeTraits.hpp>

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

    LIBLCAPI class PacketBuffer{
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

    template<typename E> struct pkmcom_traits<std::enable_if_t<std::is_enum_v<E>,E>>{
    public:
        using underlying_type = std::underlying_type_t<E>;
        using underlying_traits = pkmcom_traits<underlying_type>;
        constexpr static pkmcom_hash hashcode(E val) noexcept(noexcept(underlying_traits::hashcode(std::declval<underlying_type>()))){
            return pkmcom_traits<underlying_type>::hashcode(static_cast<underlying_type>(val));
        }
        constexpr static pkmcom_size size(E val) noexcept(noexcept(underlying_traits::size(std::declval<underlying_type>()))){
            return pkmcom_traits<underlying_type>::size(static_cast<underlying_type>(val));
        }
        static void write(E val,PacketBuffer& buff){
        	pkmcom_traits<underlying_type>::write(static_cast<underlying_type>(val),buff);
        }
        static void read(E& v,PacketBuffer& buff){
        	pkmcom_traits<underlying_type>::read(reinterpret_cast<underlying_type&>(v),buff);
        }
    };

    template<> struct pkmcom_traits<pkmcom_bool>{
    public:
        constexpr static pkmcom_hash hashcode(pkmcom_bool b)noexcept(true){
            return b?1337:1331;
        }
        constexpr static pkmcom_size size(pkmcom_bool b)noexcept(true){
            return 1;
        }
        static void write(pkmcom_bool b,PacketBuffer& buff){
            buff.writeByte(b?1:0);
        }
        static void read(pkmcom_bool& b,PacketBuffer& buff){
            b = buff.readByte()!=0;
        }
    };
    template<> struct pkmcom_traits<pkmcom_byte>{
    public:
        constexpr static pkmcom_hash hashcode(pkmcom_byte b)noexcept(true){
            return pkmcom_hash(b)&0xff;
        }
        constexpr static pkmcom_byte size(pkmcom_byte b)noexcept(true){
            return 1;
        }
        static void write(pkmcom_byte b,PacketBuffer& buff){
            buff.writeByte(b);
        }
        static void read(pkmcom_byte& b,PacketBuffer& buff){
            b = buff.readByte();
        }
    };
    template<> struct pkmcom_traits<pkmcom_sbyte>{
    public:
        constexpr static pkmcom_hash hashcode(pkmcom_sbyte s)noexcept(true){
            return pkmcom_hash(s)|(s&0x80!=0?0xffffff00:0);
        }
        constexpr static pkmcom_hash size(pkmcom_sbyte s)noexcept(true){
            return 1;
        }
    };

    template<typename T,typename=void> struct is_packet:std::false_type{};
    template<typename T>
    struct is_packet<T,std::void_t<decltype(std::declval<const T>().size()),
	decltype(std::declval<const T>().hashCode()),
			decltype(std::declval<const T>().write(std::declval<PacketBuffer&>()))
		,decltype(std::declval<T&>().read(std::declval<PacketBuffer&>()))>>:std::true_type{};

   template<typename T> struct is_packet<const T>:is_packet<T>{};
   template<typename T> struct is_packet<volatile T>:is_packet<T>{};
   template<typename T> struct is_packet<const volatile T>:is_packet<T>{};
   template<typename T> constexpr const bool is_packet_v = is_packet<T>::value;

    template<typename T,typename=void> struct is_pkmcom_type:std::false_type{};
    template<typename T> struct is_pkmcom_type<T,
	std::void_t<decltype(pkmcom_traits<T>::size(std::declval<const T>())),
    			decltype(pkmcom_traits<T>::hashcode(std::declval<const T>()))
				,decltype(pkmcom_traits<T>::write(std::declval<const T>(),std::declval<PacketBuffer&>())),
				decltype(pkmcom_traits<T>::read(std::declval<T&>(),std::declval<PacketBuffer&>()))>>:std::true_type{};
    template<typename T> constexpr const bool is_pkmcom_type_v = is_pkmcom_type<T>::value;

    template<typename T> struct pkmcom_traits<std::enable_if_t<is_packet_v<T>,T>>{
    public:
    	using packet_type = std::remove_cv_t<T>;
    	constexpr static pkmcom_hash hashcode(const packet_type& t)noexcept(noexcept(std::declval<const packet_type>().hashCode())){
    		return t.hashCode();
    	}
    	constexpr static pkmcom_size size(const packet_type& t)noexcept(noexcept(std::declval<const packet_type>().size())){
    		return t.size();
    	}
    	void write(const packet_type& t,PacketBuffer& b){
    		t.write(b);
    	}
    	void read(packet_type& t,PacketBuffer& b){
    		t.read(b);
    	}
    };
}
#endif
