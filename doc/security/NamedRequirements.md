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
		<td>std::decay_t<data_type> must yield data_type*, const data_type*, volatile data_type*, or const volatile data_type*</td>
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
		<td>Must satisfy Key</td>
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
<li>


		
