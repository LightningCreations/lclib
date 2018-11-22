<h1>Span</h1>

In header `<lclib-cxx/Span.hpp>`<br/>

<h2>Synopsis</h2>

```cpp
constexpr const std::ptrdiff_t dynamic_extent;
template<typename T,std::ptrdiff_t extent=dynamic_extent> class span;
template<typename T,std::ptrdiff_t extent> span<const std::byte,B> asBytes(const span<T,extent>& s)noexcept;
template<typename T,std::ptrdiff_t extent> span<std::byte,B> asWriteableBytes(const span<T,extent>& s)noexcept;
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2> bool operator==(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept;
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
bool operator<(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept;
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2> bool operator!=(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept;
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
bool operator<=(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept;
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2> bool operator>=(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept;
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
bool operator>(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept;
```

<h2>Overview</h2>

The `<lclib-cxx/Span.hpp>` header provides a span implementation, based of the span type in the C++2a Working Draft. 
A span is a view to a contiguous container of some size, or part of such a container. 
By default, a span has a dynamic extent. A static extent can be specified by a Positive or Zero `std::ptrdiff_t` in the second argument. In addition, a dynamic extent can be explicitly specified by passing `dynamic_extent` as the extent template parameter (this is default). The behavior is undefined if a negative extent is specified, and the extent is not dynamic_extent.

<h3>Notes</h3>
The definition of span used is from https://en.cppreference.com/w/cpp/container/span.

<h2>span</h2>

```cpp
template<typename T,std::ptrdiff_t extent=dynamic_extent> class span{
	typedef T value_type;
	typedef std::remove_const_v<T> element_type;
	typedef T* pointer;
	typedef T& reference;
	typedef /*see below*/ iterator;
	typedef /*see below*/ const_iterator;
	typedef /*see below*/ reverse_iterator;
	typedef /*see below*/ const_reverse_iterator;
	typedef std::ptrdiff_t index_type;
	typedef std::ptrdiff_t difference_type;
	constexpr span()noexcept;
	constexpr span(pointer ptr,index_type type);
	constexpr span(pointer begin,pointer end);
	template<std::size_t N> constexpr span(T(&arr)[N])noexcept;
	template<std::size_t N> constexpr span(std::array<element_type,N>& arr)noexcept;
	template<std::size_t N> constexpr span(const std::array<element_type,N>& arr)noexcept;
	template<typename Container> constexpr span(Container& c);
	template<typename Container> constexpr span(const Container& c);
	template<typename U,std::size_t N> constexpr span(const span<U,N>& s)noexcept;
	constexpr span(const span&)noexcept=default;
	constexpr pointer data()const noexcept;
	constexpr index_type size()const noexcept;
	constexpr index_type size_bytes()const noexcept;
	constexpr bool empty()const noexcept;
	constexpr iterator begin()const noexcept;
	constexpr const_iterator cbegin()const noexcept;
	constexpr iterator end()const noexcept;
	constexpr const_iterator cend()const noexcept;
	constexpr reverse_iterator rbegin()const noexcept;
	constexpr const_reverse_iterator crbegin()const noexcept;
	constexpr reverse_iterator rend()const noexcept;
	constexpr const_reverse_iterator crend()const noexcept;
	constexpr reference operator[](index_type idx)const;
	constexpr reference operator()(index_type idx)const;
	template<std::ptrdiff_t Count> constexpr span<T,Count> first()const noexcept;
	constexpr span<T,dynamic_extent> first(std::ptrdiff_t count)const noexcept;
	template<std::ptrdiff_t Count> constexpr span<T,Count> last()const noexcept;
	constexpr span<T,dynamic_extent> last(std::ptrdiff_T count)const noexcept;
	template<std::ptrdiff_t Offset,std::ptrdiff_t Count=dynamic_extent> constexpr span<T,B> subspan()const noexcept;
	constexpr span<T,dynamic_extent> subspan(std::ptrdiff_t offset,std::ptrdiff_t count=dynamic_extent)const noexcept;
};
```

<h3>Member Types</h3>

```cpp
typedef T value_type;
typedef std::remove_const_v<T> element_type;
typedef T* pointer;
typedef T& reference;
typedef /*see below*/ iterator; //(1)
typedef /*see below*/ const_iterator; //(2)
typedef /*see below*/ reverse_iterator; //(3)
typedef /*see below*/ const_reverse_iterator; //(4)
typedef std::ptrdiff_t index_type;
typedef std::ptrdiff_t difference_type;
```

(1): A Contiguous, Constexpr, RandomAccessIterator. The iterator is mutable if and only if `std::is_const_v<T>` is false.
(2): A Contiguous, Constexpr, RandomAccessIterator that is immutable. 
(3): An iterator type that is the reverse of iterator
(4): An iterator type that is the reverse of const_iterator

<h3>Constructors</h3>

```cpp
constexpr span()noexcept; //(1)
constexpr span(pointer ptr,index_type size)noexcept; //(2)
constexpr span(pointer begin,pointer end)noexcept; //(3)
template<std::size_t N> constexpr span(T(&arr)[N])noexcept; //(4)
template<std::size_t N> constexpr span(std::array<element_type,N>& arr)noexcept; //(5)
template<std::size_t N> constexpr span(const std::array<element_type,N>& arr)noexcept; //(6)
template<typename Container> constexpr span(Container& c); //(7)
template<typename Container> constexpr span(const Container& c); //(8)
template<typename U,std::size_t N> constexpr span(const span<U,N>& s)noexcept; //(9)
constexpr span(const span& s)noexcept=default; //(10)
```

(1):Constructs a span viewing an empty range. This constructor only participates in overload resolution if extent==dynamic_extent||extent==0<br/>
(2): Constructs a span over the range (ptr,ptr+size]. The behavior is undefined if (ptr,ptr+size] is not a valid range, or extent!=dynamic_extent&&extent!=size<br/>
(3): Constructs a span over the range (begin,end]. The behavior is undefined if (begin,end] is not a valid range, or extent!=dynamic_extent&&extent!=(end-begin).<br/>
(4): Constructs a span over the elements of arr. This constructor only participates in overload resolution if `std::remove_pointer_t<decltype(std::data(arr))>(*)[]` is convertible to `T(*)[]` and if extent==dynamic_extent||extent==N.<br/>
(5),(6): Constructs a span over the elements of arr. This constructor only participates in overload resolution if `std::remove_pointer_t<decltype(std::data(arr))>(*)[]` is convertible to `T(*)[]` and if extent==dynamic_extent||extent==N.<br/>
(7),(8): Constructs a span over the elements of c. Container must satisfy ContiguousContainer. This constructor only participates in overload resolution if `std::remove_pointer_t<decltype(std::data(c))>(*)[]` is convertible to `T(*)[]`. The behavior is undefined if extent!=dynamic_extent&&extent!=std::size(c).<br/>
(9): Constructs a span over the range viewed by s. This constructor only participates in overload resolution if `U(*)[]` is convertible to `T(*)[]`, and if extent==dynamic_extent||extent==N.<br/>
(10): Trivial Copy Constructor. Views the same range as s<br/>

<h4>Exceptions</h4>
(1),(2),(3),(4),(5),(9),(10): Non-throwing<br/>
(7),(8): Throws whatever std::data(c) and std::size(c) throws.<br/>

<h3>data/size</h3>

```cpp
constexpr pointer data()const noexcept; //(1)
constexpr index_type size()const noexcept; //(2)
constexpr index_type size_bytes()const noexcept; //(3)
constexpr bool empty()const noexcept; //(4)
```

(1): Pointer to the beginning of the view the span. <br/>
(2): Returns the size (in elements) of the span's view.<br/>
(3): Returns the size (in bytes) of the span's view. Effectively size()*sizeof(T).<br/>
(4): Checks if the span is empty, that is the size of the view is 0.<br/>

<h3>Index Operators</h3>

```cpp
constexpr reference operator[](index_type n)const noexcept; //(1)
constexpr reference operator()(index_type n)const noexcept; //(2)
```
(1),(2): Gets a reference to the nth element of the span. The behavior is undefined if n<0||n>=size(). (2) is provided for consistency with the C++ Multidimensional Arrays TS.

<h4>Exceptions</h4>
All are non-throwing.

<h3>Iterators</h3>

```cpp
constexpr iterator begin()const noexcept; //(1)
constexpr iterator end()const noexcept; //(2)
constexpr const_iterator cbegin()const noexcept; //(3)
constexpr const_iterator cend()const noexcept; //(4)
constexpr reverse_iterator rbegin()const noexcept; //(5)
constexpr reverse_iterator rend()const noexcept; //(6)
constexpr const_reverse_iterator crbegin()const noexcept; //(7)
constexpr const_reverse_iterator crend()const noexcept; //(8)
```

(1),(3): Returns an iterator positioned at the beginning of the view.<br/>
(2),(4): Returns an iterator positioned at the end of the view.<br/>
(5),(7): Returns a reverse_iterator positioned at the end of the view, for reverse traversal.<br/>
(6),(8): Returns a reverse_iterator positioned at the beginning of the view, for reverse traversal. <br/>

<h4>Exceptions</h4>
All are Non-throwing

<h3>Subviews</h3>

```cpp
template<std::ptrdiff_t Count> constexpr span<T,Count> first()const noexcept; //(1)
constexpr span<T,dynamic_extent> first(std::ptrdiff_t count)const noexcept; //(2)
template<std::ptrdiff_t Count> constexpr span<T,Count> last()const noexcept; //(3)
constexpr span<T,dynamic_extent> last(std::ptrdiff_T count)const noexcept; //(4)
template<std::ptrdiff_t Offset,std::ptrdiff_t Count=dynamic_extent> constexpr span<T,B /*see below*/> subspan()const noexcept; //(5)
constexpr span<T,dynamic_extent> subspan(std::ptrdiff_t offset,std::ptrdiff_t count=dynamic_extent)const noexcept; //(6)
```



(1): Returns a span over the first Count elements of the span. The behavior is undefined if Count<0||Count>=size().<br/>
(2): Same as 1 except over a dynamic count<br/>
(3): Returns a span over the last Count elements of the span. The behavior is undefined if Count<0||Count>=size().<br/>
(4): Same as 3 except over a dynamic count<br/>

(5): Returns a span over a specific part of the span, which starts from Offset, and goes to Count. If Count is `dynamic_extent`, returns all elements from Offset. If Count is not `dynamic_extent`, then B is Count. Otherwise if extent is not `dynamic_extent` then B is extent-Offset. Otherwise B is `dynamic_extent`. The behavior is undefined if the resultant range is invalid. Specifically the behavior is undefined if Offset>size(), Offset<0, Count<0 and not `dynamic_extent`, or Count is not `dynamic_extent` and Offset+Count>size().
(6): Same as 5 but offset and count are dynamic. 

<h4>Exceptions</h4>
All are non-throwing


<h2>Non Member Functions</h2>

<h3>Byte views</h3>

```cpp
template<typename T,std::ptrdiff_t extent> span<const std::byte,B /*see below*/> asBytes(const span<T,extent>& s)const noexcept; //(1)
template<typename T,std::ptrdiff_t extent> span<std::byte,B /*see below*/> asWriteableBytes(const span<T,extent>& s)const noexcept; //(2)
```

If extent is `dynamic extent`, B is also `dynamic_extent`. Otherwise B is extent*sizeof(T).

(1): Obtains a span over the raw byte view of s. The returned view is immutable. Effectively returns `{reinterpret_cast<const std::byte*>(s.data()),s.size_bytes()}`.<br/>
(2): Obtains a span over the mutable raw byte view of s. Effectively returns `{reinterpret_cast<std::byte*>(s.data()),s.size_bytes()}`. This method does not participate in overload resolution if `std::is_const_v<T>` is true.<br/>

<h3>Comparison Operators</h3>

```cpp
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2> bool operator==(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept; //(1)
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
bool operator<(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept; //(2)
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2> bool operator!=(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept; //(3)
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
bool operator<=(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept; //(4)
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2> bool operator>=(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept; //(5)
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
bool operator>(const span<T1,E1>& s1,const span<T2,E2>& s2)noexcept; //(6)
```
(1): Compares the elements of the views, as though by `std::equal_to(s1.begin(),s1.end(),s2.begin(),s2.end())`.<br/>
(2): Compares the elements of the views, as though by `std::lexographical_compare(s1.begin(),s1.end(),s2.begin(),s2.end())`.<br/>
(3): Compares the elements of the views as though by !(s1==s2).<br/>
(4): Compares the elements of the views as though by (s1<s2||s1==s2)<br/>
(5): Compares the elements of the views as though by !(s1<s2)<br/>
(6): Compares the elements of the views as though by !(s<=s2)<br/>

<h2>Fields</h2>

```cpp
constexpr const std::ptrdiff_t dynamic_extent{/*see below*/};
```
Some negative ptrdiff representing a span which is over a container with an unknown size.


