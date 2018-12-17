<h1>SHA Library</h1>

Included from `<lclib\security\SHA.hpp>`<br/>

The SHA Library is a part of the security library, which provides MessageDigestAlgorithms implementing The Secure Hash Algorithm.
Presently Implementations for SHA1, SHA-224, SHA-256, SHA-384, and SHA-512 are available.
Implementations of SHA-512/224 and SHA-512/256 are planned. Implementations of SHA3 are not currently planned, but might be added in the Future. SHA0 will not be implemented by this Library.

<h2>Synopsis</h2>

```cpp
namespace security{
	template<size_t Target,typename byte=std::byte> class SHA;
}
```


<h2>Classes</h2>

<h3>SHA</h3>

Default template class for the SHA. Specialized on Target for various SHA implementations. When specialized, SHA satisfies MessageDigestAlgorithm.
The byte parameter specifies which "byte" type is used by the algorithm.
A program which instantiates SHA with a Target that is not specialized, or with a type which does not satisfy Byte is ill formed.

<h4>Member Types</h4>

```cpp
using byte_type = byte; //(1)
using input_type = const byte*; //(2)
using output_type = byte*; //(3)
```
(1): Alias for byte.
(2): Input buffer type.
(3): Output buffer type.

<h4>Static Fields</h4>

```cpp
constexpr static const std::size_t digestSize;
```

Specified By MessageDigestAlgorithm. 
 




