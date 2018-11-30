/*
 * Concepts.hpp
 *
 *  Created on: Sep 17, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_SECURITY_CONCEPTS_HPP__2018_09_17_17_02_39
#define __INCLUDE_SECURITY_CONCEPTS_HPP__2018_09_17_17_02_39
#include <lclib/Config.hpp>
#include <cstddef>
#include <lclib/TypeTraits.hpp>
#include <lclib/ShadowRandom.hpp>
#ifdef LIBLCCXX_HAS_CONCEPTS
#include <lclib/Concepts.hpp>
#endif
namespace security{
	constexpr const std::size_t dynamicSize(-1);
	/**
	 * Template class for Key Concept
	 * Given that K satisfies Key, and an object k of type K and c of type const K,
	 * K::data_type is a valid member type const T* where is_byte_v<T> is true.
	 * L::algorithm is a c-style string which represents the name of the algorithm
	 * c.data() yields a data_type, which points to an array of bytes. If K also satisfies DestroyableKey, a null pointer must be returned if the c has been destroyed.
	 * c.size() is the size of the key, in bytes. (May or may not change if c has been destroyed)
	 * In addition: If K satisfies DestroyableKey, k.destroy() must, in a secure way, destroy the memory associated with k,
	 * then cause further calls to k.data() to return a null pointer.
	 * Further, If K satsifies PrivateKey, K must also satisfy DestroyableKey,
	 * and c.getPublicKey() must return an instance of some type which Satisfies Key.
	 * The return value must be consistent (unless c is destroyed, If so, it must throw an unspecified subclass of SecurityException),
	 * And must return the exactly associated PublicKey.
	 * If K satisfies DestroyableKey, the memory associated with k must be securely destroyed when k is destructed,
	 * that is, k.~K() must have the same effect on the associated memory as k.destroy()
	 */
	template<typename byte> class Key{
	public:
		static_assert(is_byte_v<byte>,"Key must be instantiated with a byte type");
		static constexpr const char algorithm[0]{};
		static constexpr std::size_t keySize{0};
		using byte_type = byte;
		using data_type = const byte_type*;
		//Key Concept
		data_type data()const;
		size_t size()const;
		Key()=default;
		Key(const Key&)=default;
		Key(Key&&)=default;
		Key(const Key&&)=delete;
	};
	template<typename byte> class DestroyableKey:public Key<byte>{
	public:
		//DestroyableKey Concept
		void destroy();
		DestroyableKey()=default;
		DestroyableKey(const DestroyableKey&)=default/*or = delete*/;
		DestroyableKey(DestroyableKey&&)=default/*or = delete*/;
		DestroyableKey(const DestroyableKey&&)=delete;
		~DestroyableKey();
	};
	template<typename byte> class PrivateKey:public DestroyableKey<byte>{
	public:
		using public_key = Key<byte>;
		public_key getPublicKey()const;
	};
	/**
	 * Defines the Padding Concept,
	 * This concept is defined to pad uneven blocks to a constrained block size.
	 */
	template<typename byte> class Padding{
	public:
		static_assert(is_byte_v<byte>,"Padding must be instantiated with a byte type");
		using byte_type = byte;
		using data_type = byte_type*;

		/**
		 * Returns the size of the padded data with the given size of the unpadded data
		 */
		static std::size_t getPaddedSize(std::size_t);
		/**
		 * Adds padding to the given block.
		 * It is the responsibility of the caller to ensure that the input data is at least
		 * getPaddedSize(size)
		 */
		static void pad(data_type,std::size_t);
		/**
		 * Obtains the length of the unpadded data in the given padded input data.
		 */
		static std::size_t unpad(data_type,std::size_t);
	};

	/**
	 * Defines the SymmetricCipherAlgorithm concept.
	 */
	template<typename byte> class SymmetricCipherAlgorithm{
	public:
		static_assert(is_byte_v<byte>,"SymmetricCipherAlgorithm must be instantiated with a byte type");
		using byte_type = byte;
		using input_type = const byte_type*;
		using output_type = byte_type*;
		using padding = Padding<byte>;
		using key = Key<byte>;
		constexpr static const std::size_t blockSize{0};
		void init(const key& k,bool enc,input_type iv);
		void update(input_type in,std::size_t sz,output_type out);
		void doFinal(output_type out);
	};
	/**
	 * Defines the AsymettricCipherAlgorithm concept
	 */
	template<typename byte> class AsymmetricCipherAlgorithm{
	public:
		static_assert(is_byte_v<byte>,"AsymmetricCipherAlgorithm must be instantiated with a byte type");
		using byte_type = byte;
		using input_type = const byte_type*;
		using output_type = byte_type*;
		using padding = Padding<byte>;
		using private_key = PrivateKey<byte>;
		using public_key = Key<byte>;
		constexpr static const std::size_t blockSize{0};
		void init(const private_key& pk,bool enc,input_type iv);
		void init(const public_key& pk,bool enc,input_type iv);
		void update(input_type in,std::size_t sz,output_type out);
		void doFinal(output_type out);
	};

	/**
	 * Defines the KeyGenerator Concept.
	 * Provided to generate Random keys in a secure manor
	 */
	template<typename byte> class KeyGenerator{
	public:
		using byte_type = byte;
		using key = Key<byte>;
		using source_type = ShadowRandom;
		KeyGenerator();
		KeyGenerator(source_type&&);
		key operator()();
	};
	/**
	 * Defines the KeyDerivationAlgorithm Concept.
	 * Used to derive Keys from arbitrary input, or salted passwords
	 */
	template<typename byte> class KeyDerivationAlgorithm{
	public:
		using byte_type = byte;
		using input_type = const byte*;
		using password_type = const char*;
		using salt_type = const byte*;
		using key = Key<byte>;
		key deriveKey(input_type in,std::size_t sz);
		key deriveKey(password_type pwd,salt_type salt,std::size_t sz);
	};
	/**
	 * Defines the KeyPairGenerator Concept.
	 * Provided to generate Random keys in a secure manor
	 */
	template<typename byte> class KeyPairGenerator{
	public:
		using byte_type = byte;
		using private_key = PrivateKey<byte>;
		using public_key = typename private_key::public_key;
		using source_type = ShadowRandom;
		KeyPairGenerator();
		KeyPairGenerator(source_type&&);
		std::pair<public_key,private_key> operator()();
	};
	/**
	 * Defines the MessageDigestAlgorithm concept
	 */
	template<typename byte> class MessageDigestAlgorithm{
	public:
		using byte_type = byte;
		using input_type = const byte_type*;
		using output_type = byte_type*;
		constexpr static const std::size_t digestSize{0};
		void init();
		void update(input_type,std::size_t);
		void doFinal(output_type);
	};
#ifdef LIBLCCXX_HAS_CONCEPTS
	namespace concepts{
		/**
		 * Add Actual concept declarations if C++20 Concepts are supported.
		 */
		template<typename T> concept Key=Byte<typename T::byte_type>&&requires(const T& t){
			{t.data()} -> typename T::data_type;
			{t.size()} -> std::size_t;
		}

		template<typename T> concept DestroyableKey=Key<T>&&requires(T& t){
			{t.destroy()}
		}

		template<typename T> concept PrivateKey=DestroyableKey<T>&&requires(const T& t){
			{t.publicKey()} -> typename T::public_key;
		}
		template<typename T> concept Padding=Byte<typename T::byte_type>&&requires(std::size_t sz,typename T::data_type dt){
			{T::getPaddedSize(sz)} -> std::size_t;
			{T::pad(dt,sz)};
			{T::unpad(dt,sz)} -> std::size_t;
		}

		template<typename T> concept Transformer=Byte<typename T::byte_type>&&requires(T& t,typename T::input_type in,typename T::output_type out){
			{T::blockSize}->std::size_t;
			{t.transform(in,out)};
			{t.update(in)};
			{t.reverseTransform(in,sz,out);};
		}

		template<typename T> concept SymmetricCipherAlgorithm=Byte<typename T::byte_type>&&Key<typename T::key>&&
				Padding<typename T::padding>&&
				Transformer<typename T::transformer>&&
				requires(T& t,const typename T::key& k,typename T::input_type in,std::size_t sz,typename T::output_type out,bool b){
			{t.init(k,b,in)};
			{t.update(in,sz,out)};
			{t.doFinal(out)};
			{T::blockSize} -> std::size_t;
		}
		template<typename T> concept AsymmetricCipherAlgorithm=Byte<typename T::byte_type>&&
				PrivateKey<typename T::private_key>&&Key<typename T::public_key>&&
				Padding<typename T::padding>&&
				Transformer<typename T::transformer>&&
				requires(T& t,const typename T::private_key& pv,const typename T::public_key& pu,typename T::input_type in,std::size_t sz,typename T::output_type,bool b){
			{t.init(pv,b,in)};
			{t.init(pb,b,in)};
			{t.update(in,sz,out)};
			{t.doFinal(out)};
			{T::blockSize} -> std::size_t;
		}
		template<typename T> concept CipherAlgorithm=AsymmetricCipherAlgorithm<T>||SymmetricCipherAlgorithm<T>;

		template<typename T> concept KeyGenerator=Byte<typename T::byte_type>
			&&Key<typename T::key>&&requires(typename T::source_type&& x,T& t){
			{T{}};
			{T{std::move(x)}};
			{t.newKey()} -> typename T::key;
		}
		template<typename T> concept KeyDerivationAlgorithm=
				Byte<typename T::byte_type>&&Key<typename T::key>
			&&requires(T& t,typename T::password_type pwd,typename T::salt_type salt,typename T::input_type raw){
			{t.deriveKey(raw)}->typename T::key;
			{t.deriveKey(pwd,salt)}->typename T::key;
		}
		template<typename T> concept KeyPairGenerator=Byte<typename T::byte_type>
			&&PrivateKey<typename T::private_key>&&Key<typename T::public_key>
			&&requires(T& t,typename T::source_type&& x){
			{T{}};
			{T{x}};
			{t.newKeyPair()} -> std::pair<typename T::public_key,typename T::private_key>;
		}
		template<typename T> concept MessageDigestAlgorithm=Byte<typename T::byte_type>
		&&requires(T& t,typename T::input_type in,std::size_t sz,typename T::output_type out){
			{T::digestSize} -> std::size_t;
			{t.init()};
			{t.update(in,sz)};
			{t.doFinal(out)};
		}
	}
#endif
	namespace traits{
		template<typename T,typename=void> struct is_key:std::false_type{};
		template<typename T> struct is_key<T,std::void_t<
			std::enable_if_t<is_cstring_v<decltype(T::algorithm)>>,
			std::enable_if_t<is_byte_v<typename T::byte_type>>,
			std::enable_if_t<std::is_same_v<typename T::byte_type,remove_cvptr_t<typename T::data_type>>>,
			decltype(std::declval<const T>().data()),
			decltype(std::declval<const T>().size())>>:std::true_type{};
		template<typename T> constexpr const bool is_key_v = is_key<T>::value;
		template<typename T,typename=void> struct is_destroyable_key:std::false_type{};
		template<typename T> struct is_destroyable_key<T,std::void_t<
			std::enable_if_t<is_key_v<T>>,
			decltype(std::declval<T>().destroy())>>:std::true_type{};
		template<typename T> constexpr const bool is_destroyable_key_v = is_destroyable_key<T>::value;
		template<typename T,typename=void> struct is_private_key:std::false_type{};
		template<typename T> struct is_private_key<T,std::void_t<
			std::enable_if_t<is_destroyable_key_v<T>>,
			std::enable_if_t<is_key_v<typename T::public_key>>,
			std::enable_if_t<std::is_same_v<typename T::public_key,decltype(std::declval<const T>().getPublicKey())>>>>
				:std::true_type{};
		template<typename T> constexpr const bool is_private_key_v = is_private_key<T>::value;

		template<typename T,typename=void> struct is_padding:std::false_type{};
		template<typename T> struct is_padding<T,std::void_t<
			std::enable_if_t<is_byte_v<typename T::byte_type>>,
			decltype(T::getPaddedSize(std::declval<std::size_t>())),
			decltype(T::pad(std::declval<typename T::data_type>(),std::declval<std::size_t>())),
			decltype(T::unpad(std::declval<typename T::data_type>(),std::declval<std::size_t>()))
			>>:std::true_type{};
		template<typename T> constexpr const bool is_padding_v = is_padding<T>::value;

		template<typename T,typename=void> struct is_symmetric_cipher_algorithm:std::false_type{};
		template<typename T> struct is_symmetric_cipher_algorithm<T,std::void_t<
			std::enable_if_t<is_byte_v<typename T::byte_type>>,
			std::enable_if_t<is_padding_v<typename T::padding>>,
			std::enable_if_t<is_key_v<typename T::key>>,
			std::enable_if_t<std::is_same_v<decltype(T::blockSize),std::size_t>>,
			decltype(std::declval<T&>().init(std::declval<typename T::key>(),std::declval<bool>(),std::declval<typename T::input_type>())),
			decltype(std::declval<T&>().update(std::declval<typename T::input_type>(),std::declval<std::size_t>(),std::declval<typename T::output_type>())),
			decltype(std::declval<T&>().doFinal(std::declval<typename T::output_type>()))
		>>:std::true_type{};
		template<typename T> constexpr const bool is_symmetric_cipher_algorithm_v = is_symmetric_cipher_algorithm<T>::value;

		template<typename T,typename=void> struct is_asymmetric_cipher_algorithm:std::false_type{};
		template<typename T> struct is_asymmetric_cipher_algorithm<T,std::void_t<
			std::enable_if_t<is_byte_v<typename T::byte_type>>,
			std::enable_if_t<is_padding_v<typename T::padding>>,
			std::enable_if_t<is_private_key_v<typename T::private_key>>,
			std::enable_if_t<is_key_v<typename T::public_key>>,
			decltype(std::declval<T&>().init(std::declval<typename T::private_key>(),std::declval<bool>(),std::declval<typename T::input_type>())),
			decltype(std::declval<T&>().init(std::declval<typename T::public_key>(),std::declval<bool>(),std::declval<typename T::input_type>())),
			decltype(std::declval<T&>().update(std::declval<typename T::input_type>(),std::declval<std::size_t>(),std::declval<typename T::output_type>())),
			decltype(std::declval<T&>().doFinal(std::declval<typename T::output_type>()))
		>>:std::true_type{};
		template<typename T> constexpr const bool is_asymmetric_cipher_algorithm_v = is_asymmetric_cipher_algorithm<T>::value;

		template<typename T> struct is_cipher_algorithm:std::disjunction<is_symmetric_cipher_algorithm<T>,is_asymmetric_cipher_algorithm<T>>{};
		template<typename T> constexpr const bool is_cipher_algorithm_v = is_cipher_algorithm<T>::value;

		template<typename T,typename=void> struct is_key_generator:std::false_type{};
		template<typename T> struct is_key_generator<T,std::void_t<
			std::enable_if_t<is_key_v<typename T::key>>,
			std::enable_if_t<is_byte_v<typename T::byte_type>>,
			std::enable_if_t<std::is_default_constructible_v<T>>,
			std::enable_if_t<std::is_constructible_v<T,typename T::source_type&&>>,
			std::enable_if_t<std::is_invocable_r_v<T,typename T::key>>
		>>:std::true_type{};
		template<typename T> constexpr const bool is_key_generator_v = is_key_generator<T>::value;

		template<typename T,typename=void> struct is_key_pair_generator:std::false_type{};
		template<typename T> struct is_key_pair_generator<T,std::void_t<
			std::enable_if_t<is_private_key_v<typename T::private_key>>,
			std::enable_if_t<is_key_v<typename T::public_key>>,
			std::enable_if_t<is_byte_v<typename T::byte_type>>,
			std::enable_if_t<std::is_default_constructible_v<T>>,
			std::enable_if_t<std::is_constructible_v<T,typename T::source_type&&>>,
			std::enable_if_t<std::is_invocable_r_v<T,std::pair<typename T::public_key,typename T::private_key>>>
		>>:std::true_type{};
		template<typename T> constexpr const bool is_key_pair_generator_v = is_key_pair_generator<T>::value;

		template<typename T,typename=void> struct is_key_derivation_algorithm:std::false_type{};
		template<typename T> struct is_key_derivation_algorithm<T,std::void_t<
			std::enable_if_t<is_byte_v<typename T::byte_type>>,
			std::enable_if_t<is_cstring_v<typename T::password_type>>,
			std::enable_if_t<is_key_v<typename T::key>>,
			std::enable_if_t<std::is_same_v<typename T::key,decltype(std::declval<T>().deriveKey(std::declval<typename T::input_type>(),std::declval<std::size_t>()))>>,
			std::enable_if_t<std::is_same_v<typename T::key,decltype(std::declval<T>().deriveKey(std::declval<typename T::password_type>(),std::declval<typename T::salt_type>(),std::declval<std::size_t>()))>>
		>>:std::true_type{};
		template<typename T> constexpr const bool is_key_derivation_algorithm_v = is_key_derivation_algorithm<T>::value;

		template<typename T,typename=void> struct is_message_digest_algorithm:std::false_type{};
		template<typename T> struct is_message_digest_algorithm<T,std::void_t<
			std::enable_if_t<is_byte_v<typename T::byte_type>>,
			decltype(T::digestSize),
			decltype(std::declval<T&>().init()),
			decltype(std::declval<T&>().update(std::declval<typename T::input_type>(),std::declval<std::size_t>())),
			decltype(std::declval<T&>().doFinal(std::declval<typename T::output_type>()))
		>>:std::true_type{};
		template<typename T> constexpr const bool is_message_digest_algorithm_v = is_message_digest_algorithm<T>::value;
	}
}




#endif /* __INCLUDE_SECURITY_CONCEPTS_HPP__2018_09_17_17_02_39 */
