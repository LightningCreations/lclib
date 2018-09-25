<h1>Security Library Named Requirements</h1>
This is part of the security library defining Concepts and named requirements of Security Types<br/>
Included by &lt;security/Concepts.hpp&gt;<br/>
In `namespace security`
<h2>Named Requirements</h2>
<h3>Key</h3>
Key is named requirement in the LCLib C++ Security Library. It is the structure representation of a Cryptographic Key.
<h4>Requirements:</h4>
Given
<ul>
<li>T, a type which statisfies Key</li>
<li>c: an object of type const T</li>
<li>x: an xvalue or prvalue of type T</li>
<li>cx: an xvalue of type const T</li>
</ul>
<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics or Value</th>
		<th>Notes</th>
	</tr>
	<tr>
		<td>typename T::byte_type</td>
		<td>a typename which satisfies ByteType</td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td>typename T::data_type</td>
		<td>a typename which is a pointer or array to possibly cv-qualified byte_type</td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td>T::algorithm</td>
		<td>a type which satisfies C-String</td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td>T{}</td>
		<td>A T</td>
		<td>Default construct T</td>
		<td></td>
	</tr>
	<tr>
		<td>T{c}</td>
		<td>A T or Deleted</td>
		<td>Copy Constructs a T. </td>
		<td></td>
	</tr>
	<tr>
		<td>T{x}</td>
		<td>A T</td>
		<td>Move constructs a T.</td>
		<td></td>
	</tr>
	<tr>
		<td>T{cx}</td>
		<td>Deleted</td>
		<td>Move-from-const is disallowed for Keys</td>
		<td></td>
	</tr>
	<tr>
		<td>c.data()</td>
		<td>data_type</td>
		<td>Returns the data associated with the key. Format is implementation defined. .</td>
		<td>If T satisfies DestroyableKey, c must not have been destroyed</td>
	</tr>
	<tr>
		<td>c.size()</td>
		<td>std::size_t</td>
		<td>The size of the data associated with the key</td>
		<td></td>
	</tr>
</table>
<h4>Notes</h4>
If a type T satisfies Key, the type const T also satisifies Key. This does not hold for the subconcepts of Key.
<h3>DestroyableKey</h3>
DestroyableKey is a requirement which extends the Key concept. DestroyableKeys contain sensitive information which may need to be destroyed at will, and must be destroyed when the object is destroyed. In general, Private Keys and Secret Keys should satisfy destroyable key.
<h4>Requirements</h4>
Given:
<ul>
<li>A type T which satisfies DestroyableKey</li>
<li>k a value of type T</li>
<li>c a value of type const T</li>
<li>x an xvalue or prvalue of type T</li>
</ul>
T must satisfy Key (except that Copy and move constructors may be deleted)
<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics or Value</th>
		<th>Notes</th>
	</tr>
	<tr>
		<td>T{c}</td>
		<td>A T or deleted</td>
		<td>If not deleted, equivalent to copy constructor defined with the Key Concept</td>
		<td>May be deleted if Keys cannot be shared. Undefined behavior if c is destroyed</td>
	</tr>
		<td>T{x}</td>
		<td>A T or deleted</td>
		<td>If not deleted, resources owned by x are transfered to this. If Resource Move is semantically equal to resource copy, x must be destroyed.</td>
		<td>May be deleted if Keys cannot be shared (Not required to be deleted even if Copy Constructor is deleted). Undefined behavior if x is destroyed</td>
	</tr>
	<tr>
		<td>k.~T()</td>
		<td>Destructor</td>
		<td>If k has not been moved from or destroyed, resources owned by k must be destroyed.</td>
		<td></td>
	</tr>
	<tr>
		<td>k.destroy()</td>
		<td>void</td>
		<td>Destroys k. All resources owned by k must be destroyed in a secure way. This may involve overwriting the storage of the resources.</td>
	</tr>
</table>

<h3>PrivateKey</h3>
PrivateKey further extends the DestroyableKey concept, and exists for asymmetric encryption algorithms. PrivateKeys also provide an ability to (potentially) compute the public key.
<h4>Requirements</h4>
Given:
<ul>
<li>A type T which satisfies PrivateKey</li>
<li>c, and object of type const T</li>
</ul>
T must also Satisfy DestroyableKey
<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics or Value</th>
		<th>Notes</th>
	</tr>
	<tr>
		<td>typename T::public_key</td>
		<td>a Typename</td>
		<td>The associated public key type</td>
		<td>Must satisfy Key. UB if public_key::byte_type is not T::byte_type</td>
	</tr>
	<tr>
		<td>c.getPublicKey()</td>
		<td>public_key</td>
		<td>The public key associated with this private key.</td>
		<td></td>
	</tr>
</table>

<h3>Padding</h3>
Padding concept is provided to allow Cipher Algorithms to pad incomplete blocks. Padding is a static concept, objects of types which satisfy Padding do not need to be created.
<h4>Requirements</h4>
Given:
<ul>
<li>T, a type which satisfies Padding</li>
<li>sz, an object of type std::size_t</li>
<li>d, an object of type T::data_type</li>
</ul>
<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics or Value</th>
		<th>Notes</th>
	</tr>
	<tr>
		<td>typename T::byte_type</td>
		<td>A type alias</td>
		<td>The byte type of the Padding</td>
		<td>Must satisfy Byte</td>
	</tr>
	<tr>
		<td>typename T::data_type</td>
		<td>A type alias</td>
		<td>A pointer to an array of byte_type</td>
		<td></td>
	</tr>
	<tr>
		<td>T::getPaddedSize(sz)</td>
		<td>std::size_t</td>
		<td>The total size of the block list which has starting size sz, after padding</td>
		<td></td>
	</tr>
	<tr>
		<td>T::pad(d,sz)</td>
		<td>void</td>
		<td>Adds padding to the end of d. sz is the size of the unpadded block.</td>
		<td>The object pointed to by d must be at least getPaddedSize(sz) bytes.</td>
	</tr>
	<tr>
		<td>T::unpad(d,sz)</td>
		<td>std::size_t</td>
		<td>returns the index of the first byte of the unpadded data block.</td>
		<td></td>
	</tr>
</table>

<h3>SymmetricCipherAlgorithm</h3>
Cipher Algorithm for Symmetric Ciphers (Like AES). 

<h4>Requirements</h4>
Given:
<ul>
<li>T, a type which satisfies SymmetricCipherAlgorithm</li>
<li>t, a value of type T</li>
<li>k, an lvalue of type const T::key, or an rvalue of type T::key</li>
<li>b, a value of type bool</li>
<li>in, a value of type T::input_type</li>
<li>out, a value of type T::output_type</li>
<li>sz, a value of type std::size_t, which is a multiple of T::blockSize</li>
</ul>

<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics or Value</th>
		<th>Notes</th>
	</tr>
	<tr>
		<td>typename T::byte_type</td>
		<td>type alias</td>
		<td>Byte type used by operations</td>
		<td>Must satisfy Byte</td>
	</tr>
	<tr>
		<td>typename T::input_type</td>
		<td>type alias</td>
		<td>Pointer to a constant input buffer of byte_type</td>
		<td></td>
	</tr>
	<tr>
		<td>typename T::output_type</td>
		<td>type alias</td>
		<td>Pointer to a mutable output buffer of byte_type</td>
		<td></td>
	</tr>
	<tr>
		<td>typename T::key</td>
		<td>type alias</td>
		<td>Key type used for init</td>
		<td>Must Satisfy Key. UB if key::byte_type is not byte_type</td>
	</tr>
	<tr>
		<td>typename T::padding</td>
		<td>type alias</td>
		<td>Padding type used for CipherIOStreams</td>
		<td>Must satisfy Padding. UB if padding::byte_type is not byte_type</td>
	</tr>
	<tr>
		<td>T::blockSize</td>
		<td>size_t</td>
		<td>The size of the block used or -1.</td>
		<td>Must be a constant expression</td>
	</tr>
	<tr>
		<td>t.init(k,b,in)</td>
		<td>void</td>
		<td>Initializes the Cipher to process input, using k as the key, and in as the iv(if used). b is true if the Cipher is used to Encrypt, false if used to Decrypt.</td>
		<td></td>
	</tr>
	<tr>
		<td>t.update(in,sz,out)</td>
		<td>void</td>
		<td>Processes bytes form in, and sends them to out</td>
		<td>Effects are unspecified (and may be undefined) if blockSize is not -1, and sz is not a multiple of blockSize. Behavior is undefined if init has not been called</td>
	</tr>
	<tr>
		<td>t.doFinal(out)</td>
		<td>void</td>
		<td>Finalizes the cipher. Any leftover bytes are copied to out.</td>
		<td>Behavior is undefined if init has not been called</td>
	</tr>
</table>

<h3>AsymmetricCipherAlgorithm</h3>
Concept for Cipher Algorithms that use a different key for encryption/decryption (such as RSA).

<h4>Requirements</h4>
Given:

<ul>
<li>T, a type which satisfies AsymmetricCipherAlgorithm</li>
<li>t, a value of type T</li>
<li>pk, an lvalue of type const T::public_key, or an rvalue of type T::public_key</li>
<li>pvk, an lvalue of type const T::private_key, or an rvalue of type T::private_key</li>
<li>b, a value of type bool</li>
<li>in, a value of type T::input_type</li>
<li>out, a value of type T::output_type</li>
<li>sz, a value of type std::size_t, which is a multiple of T::blockSize</li>
</ul>


<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics or Value</th>
		<th>Notes</th>
	</tr>
	<tr>
		<td>typename T::byte_type</td>
		<td>type alias</td>
		<td>Byte type used by operations</td>
		<td>Must satisfy Byte</td>
	</tr>
	<tr>
		<td>typename T::input_type</td>
		<td>type alias</td>
		<td>Pointer to a constant input buffer of byte_type</td>
		<td></td>
	</tr>
	<tr>
		<td>typename T::output_type</td>
		<td>type alias</td>
		<td>Pointer to a mutable output buffer of byte_type</td>
		<td></td>
	</tr>
	<tr>
		<td>typename T::private_key</td>
		<td>type alias</td>
		<td>Key type used for init</td>
		<td>Must Satisfy PrivateKey. UB if key::byte_type is not byte_type</td>
	</tr>
	<tr>
		<td>typename T::public_key</td>
		<td>type alias</td>
		<td>Key type used for init</td>
		<td>Must Satisfy Key. UB if key::byte_type is not byte_type. UB if private_key::public_key is not the same type</td>
	</tr>
	<tr>
		<td>typename T::padding</td>
		<td>type alias</td>
		<td>Padding type used for CipherIOStreams</td>
		<td>Must satisfy Padding. UB if padding::byte_type is not byte_type</td>
	</tr>
	<tr>
		<td>T::blockSize</td>
		<td>size_t</td>
		<td>The size of the block used or -1.</td>
		<td>Must be a constant expression</td>
	</tr>
	<tr>
		<td>t.init(pk,b,in)</td>
		<td>void</td>
		<td>Initializes the Cipher to process input, using pk as the key, and in as the iv(if used). b is true if the Cipher is used to Encrypt, false if used to Decrypt.</td>
		<td>Effects of initializing an AssymetricCipherAlgorithm in decrypt mode with a public key is Implementation-Defined</td>
	</tr>
	<tr>
		<td>t.init(pvk,b,in)</td>
		<td>void</td>
		<td>Initializes the Cipher to process input, using pvk as the key, and in as the iv(if used). b is true if the Cipher is used to Encrypt, false if used to Decrypt.</td>
		<td>Effects of initializing an AssymetricCipherAlgorithm in encry mode with a private key is Implementation-Defined.</td>
	</tr>
	<tr>
		<td>t.update(in,sz,out)</td>
		<td>void</td>
		<td>Processes bytes form in, and sends them to out</td>
		<td>Effects are unspecified (and may be undefined) if blockSize is not -1, and sz is not a multiple of blockSize. Behavior is undefined if init has not been called</td>
	</tr>
	<tr>
		<td>t.doFinal(out)</td>
		<td>void</td>
		<td>Finalizes the cipher. Any leftover bytes are copied to out.</td>
		<td>Behavior is undefined if init has not been called</td>
	</tr>
</table>

<h3>CipherAlgorithm</h3>
CipherAlgorithm is a generalized concept, which has specializations in SymmetricCipherAlgorithm and AsymmetricCipherAlgorithm.
CipherAlgorithm does not have any explicit requirements, but All types which satisfy SymmetricCipherAlgorithm or AsymmetricCipherAlgorithm, and only such types, satisfy CipherAlgorithm.

<h3>KeyGenerator</h3>
KeyGenerator is a provided to generate random Keys for SymmetricCipherAlgorithms. KeyGenerator is a special NullaryFunctionalObject which returns a key when called.

<h4>Requirements</h4>
Given:
<ul>
<li>T, a type which satisfies KeyGenerator</li>
<li>t, a value of type t</li>
<li>r, and rvalue of type T::source_type</li>
</ul>
<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics or Value</th>
		<th>Notes</th>
	</tr>
	<tr>
		<td>typename T::byte_type</td>
		<td>type alias</td>
		<td>The byte type</td>
		<td>Must satisfy Byte</td>
	</tr>
	<tr>
		<td>typename T::key</td>
		<td>type alias</td>
		<td>The Key type</td>
		<td>Must satisfy Key. UB if key::byte_type is not the same as byte_type</td>
	</tr>
	<tr>
		<td>typename T::source_type</td>
		<td>type alias</td>
		<td>The source of random bytes to be used to generate keys</td>
		<td>
			Must satisfy DefaultConstructible and MoveConstructible. Must also either: 
			<ul>
				<li> be a subclass of Random (defined in &lt;Random.hpp&gt;)</li>
				<li>Satisfy UniformRandomBitGenerator</li>
			</ul>
		</td>
	</tr>
	<tr>
		<td>T{}</td>
		<td>T</td>
		<td>Initialize the internal random number generator with an unpredictable state</td>
		<td></td>
	</tr>
	<tr>
		<td>T{r}</td>
		<td>T</td>
		<td>Initializes the internal random number generator by moving the state of r</td>
		<td>If source_type is Non-Deterministic (such as std::random_device), this is equivalent to T{}</td>
	</tr>
	<tr>
		<td>t()</td>
		<td>T::key_type</td>
		<td>Generates a random Key from the internal random number generator</td>
		<td></td>
	</tr>
</table>
		
<h3>KeyPairGenerator</h3>
KeyPairGenerator is a provided to generate random Keys pairs for AsymmetricCipherAlgorithms. KeyGenerator is a special NullaryFunctionalObject which returns a key when called.

<h4>Requirements</h4>
Given:
<ul>
<li>T, a type which satisfies KeyGenerator</li>
<li>t, a value of type t</li>
<li>r, and rvalue of type T::source_type</li>
</ul>
<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics or Value</th>
		<th>Notes</th>
	</tr>
	<tr>
		<td>typename T::byte_type</td>
		<td>type alias</td>
		<td>The byte type</td>
		<td>Must satisfy Byte</td>
	</tr>
	<tr>
		<td>typename T::private_key</td>
		<td>type alias</td>
		<td>The PrivateKey type</td>
		<td>Must satisfy PrivateKey. UB if key::byte_type is not the same as byte_type</td>
	</tr>
	<tr>
		<td>typename T::public_key</td>
		<td>type alias</td>
		<td>The PublicKey type</td>
		<td>Must satisfy Key. UB if this is not the same type as private_key::public_key</td>
	</tr>
	<tr>
		<td>typename T::source_type</td>
		<td>type alias</td>
		<td>The source of random bytes to be used to generate keys</td>
		<td>
			Must satisfy DefaultConstructible and MoveConstructible. Must also either: 
			<ul>
				<li> be a subclass of Random (defined in &lt;Random.hpp&gt;)</li>
				<li>Satisfy UniformRandomBitGenerator</li>
			</ul>
		</td>
	</tr>
	<tr>
		<td>T{}</td>
		<td>T</td>
		<td>Initialize the internal random number generator with an unpredictable state</td>
		<td></td>
	</tr>
	<tr>
		<td>T{r}</td>
		<td>T</td>
		<td>Initializes the internal random number generator by moving the state of r</td>
		<td>If source_type is Non-Deterministic (such as std::random_device), this is equivalent to T{}</td>
	</tr>
	<tr>
		<td>t()</td>
		<td>std::pair<T::public_key,T::private_key></td>
		<td>Generates a random Key Pair from the internal random number generator</td>
		<td>The public key returned must be equivalent to value resulting from calling getPublicKey on the returned private key.</td>
	</tr>
</table>

<h3>KeyDerivationAlgorithm</h3>
KeyDerivationAlgorithm is an algorithm for turning arbitrary byte sequences, and salted passwords into keys. 
<h4>Requirements</h4>
Given:
<ul>
<li>T, a type which satisfies KeyDerivationAlgorithm</li>
<li>t, a value of type T</li>
<li>bytes, a value of type T::input_type</li>
<li>pwd, a value of type T::password_type</li>
<li>salt, a value of type T::salt_type</li>
<li>sz, a value of type std::size_t</li>
</ul>
<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics or Value</th>
		<th>Notes</th>
	</tr>
	<tr>
		<td>typename T::byte_type</td>
		<td>Type Alias</td>
		<td>The byte type</td>
		<td>Must satisfy Byte</td>
	</tr>
	<tr>
		<td>typename T::input_type</td>
		<td>Type Alias</td>
		<td>The constant buffer of byte_type</td>
		<td></td>
	</tr>
	<tr>
		<td>typename T::password_type</td>
		<td>Type Alias</td>
		<td>The type of passwords for use in deriving from salted passwords</td>
		<td>Must satisfy CString</td>
	</tr>
	<tr>
		<td>typename T::salt_type</td>
		<td>Type Alias</td>
		<td>The Type of the salt for deriving from salted passwords</td>
		<td></td>
	</tr>
	<tr>
		<td>t.deriveKey(in,sz)</td>
		<td>key</td>
		<td>A key which's content depends on the buffer, and only the buffer</td>
		<td></td>
	</tr>
	<tr>
		<td>t.deriveKey(pwd,salt,sz)</td>
		<td>key</td>
		<td>A key which's content depends on the password and the salt (and only those values)</td>
		<td></td>
	</tr>
</table>

<h3>MessageDigestAlgorithm</h3>
Algorithm for the MessageDigest structure. MessageDigestAlgorithms hash or "digest" arbitary byte buffers, and returns a staticly sized hash. 

<h4>Requirements</h4>
Given:
<ul>
<li>T, a type which satisfies MessageDigestAlgorithm</li>
<li>t, a value of type T</li>
<li>in, a value of type T::input_type</li>
<li>out, a value of type T::output_type</li>
<li>sz, a value of std::size_t</li>
</ul>

<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics or Value</th>
		<th>Notes</th>
	</tr>
	<tr>
		<td>typename T::byte_type</td>
		<td>Type Alias</td>
		<td>The byte type</td>
		<td>Must satisfy Byte</td>
	</tr>
	<tr>
		<td>typename T::input_type</td>
		<td>Type Alias</td>
		<td>The input buffer type</td>
		<td></td>
	</tr>
	<tr>
		<td>typename T::output_type</td>
		<td>Type Alias</td>
		<td>The output buffer type</td>
		<td></td>
	</tr>
	<tr>
		<td>T::digestSize</td>
		<td>std::size_t</td>
		<td>The size of the resultant digest</td>
		<td></td>
	</tr>
	<tr>
		<td>t.init()</td>
		<td>void</td>
		<td>Initializes the MessageDigest</td>
		<td>UB if called multiple times without a call to doFinal()</td>
	</tr>
	<tr>
		<td>t.update(in,sz)</td>
		<td>void</td>
		<td>Updates the state with the given buffer</td>
		<td>UB if called before a call to init() or since a call to doFinal() but before a subsequent call to init()</td>
	</tr>
	<tr>
		<td>t.doFinal(out)</td>
		<td>void</td>
		<td>Finalizes the internal state, and moves the digest into out</td>
		<td>UB if called before a call to init(), or since a call to doFinal() but before a subsequent call to init()</td>
	</tr>
</table>

<h2>Concepts</h2>
Included with &lt;lclib-cxx/security/Concepts.hpp&gt;<br/>
Requires LIBLCCXX_HAS_CONCEPTS<br/>
The Security API Provides a Set of Concepts if Concepts are supported by the compiler. 
Each named requirement has a respective concept, declared in the security::concepts namespace.

<h2>Type Traits</h2>
Included with &lt;lclib-cxx/security/Concepts.hpp&gt;<br/>
The Security API Also provides a Set of legacy type traits, for when concepts may not be or are not available. 
Each named requirement has a respective type trait, declared in the security::traits namespace.


