<h1>NBT Arrays</h1>
In library nbt<br/>
Included from `<lclib-cxx/nbt/NBTArray.hpp>`

<h2>Synopsis</h2>

```cpp
namespace nbt{
	(symbolic)template<typename T> class NBTArray:public NBTTagBase;
	class NBTTagByteArray:public NBTTagBase;
	class NBTTagIntArray:public NBTTagBase;
	class NBTTagLongArray:public NBTTagBase;
};
```

<h2>Classes</h2>
<h3>template<typename T> class NBTArray (Symbolic)</h3>

Symbolic Type for NBT Array Tags. NBTArray is only defined for T being `int8_t`, `int32_t` or `int64_t`. Note that NBTArray is only a symbolic type, to avoid individually defining the 3 actual tag types, which are similar in style. 
NBTArray Satisfies ContiguousContainer, and SequenceContainer. NBTArray does not satisfy AllocatorAwareContainer. 

<h4>Member Types</h4>

```cpp
typedef T value_type; //(1)
typedef T* pointer; //(2)
typedef T& reference; //(3)
typedef const T* const_pointer; //(4)
typedef const T& const_reference; //(5)
typedef std::size_t size_type; //(6)
typedef std::ptrdiff_t difference_type; //(7)
typedef /*see below*/ iterator; //(8)
typedef /*see below*/ const_iterator; //(9)
typedef /*see below*/ reverse_iterator; //(10)
typedef /*see below*/ const_reverse_iterator; //(11)
```

(1): The value type of the array. Defined as `T`
(2): The pointer type. Defined as `T*`
(3): The reference type. Defined as `T&`
(4): The constant pointer type. Defined as `const T*`
(5): The constant reference type. Defined as `const T&`.
(6): The result type of size(). Defined as `std::size_t`
(7): The type resultant of subtracting 2 objects of type pointer. Defined as `std::ptrdiff_t`
(8): A Mutable, Contiguous, RandomAccessIterator which has `value_type` being `T`, `size_type` being `std::size_t`, and `difference_type` being `std::ptrdiff_t`.
(9): A Constant, Contiguous, RandomAccessIterator which meets the above (except `value_type` should be `const T`)
(10): A reverse iterator for `iterator`
(11): A reverse iterator for `const_iterator`

<h4>Constructors and Destructor</h4>

```cpp
NBTArray();//(1)
NBTArray(size_type sz);//(2)
template<std::size_t N> NBTArray(const value_type(&arr)[N]); //(3)
template<std::size_t N> NBTArray(const std::array<value_type,N>& arr); //(4)
template<typename U,std::size_t N> NBTArray(const U(&arr)[N]); //(5)
template<typename U,std::size_t N> NBTArray(const std::array<U,N>& arr); //(6)
template<typename InputIt> NBTArray(InputIt begin,InputIt end); //(7)
NBTArray(const NBTArray& arr); //(8)
NBTArray(NBTArray&& arr);//(9)
~NBTArray();//(10)
```


(1):Constructs an empty array.<br/>
(2): Constructs an array with sz default constructed elements.<br/>
(3),(4): Constructs an array with N elements copied from arr<br/>
(5),(6): Constructs an array with N elements constructed from the elements of arr. This constructor only participates in overload resolution if `std::is_convertible_v<T,U>` is true.
(7): Constructs an array with the size of std::distance(begin,end), copied or constructed from the elements of the range (begin,end]. 
This constructor only participates in overload resolution if `std::iterator_triats<InputIt>::value_type` is well formed, is given by the type U, and `std::is_convertible_v<T,U>` is true. 
The Behavior is undefined if (begin,end] is not a valid range. <br/>
(8): Constructs an array from the elements of arr, with the same size.<br/>
(9): Moves the underlying array of arr to the new array.<br/>
(10): Destroys the underlying array if it has not been moved<br/>



