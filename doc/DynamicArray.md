# Dynamic Array #

In header `<lclib/DynamicArray.hpp>`.

DynamicArray is a container type which stores a fixed, though dynamically sized, array. 

The container does not automatically resize, however its size does not need to be known at compile time. 

DynamicArray satisfies *SequenceContainer*, *ContiguousContainer*, *ReversibleContainer*, and *AllocatorAwareContainer*. 

## Class Synopsis ##

```cpp
template<typename T,typename Allocator=std::allocator<T>> class DynamicArray{
public:
	using element_type = T;
	using reference = T&;
	using pointer = T*;
	using const_reference = const T&;
	using const_pointer = const T*;
	using allocator_type = Allocator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using iterator = /*implementation-defined*/;
	using const_iterator = /*implementation-defined*/;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	friend void swap(DynamicArray&,DynamicArray&)noexcept(/*see below*/);
	allocator_type& getAllocator();
	DynamicArray& operator=(DynamicArray);
	DynamicArray();
	explicit DynamicArray(size_type);
	DynamicArray(size_type,Allocator);
	template<typename Fn> explicit DynamicArray(size_type,Fn&&);
	template<typename Fn> explicit DynamicArray(size_type,Fn&&,Allocator);
	template<typename Container> EXPLICIT DynamicArray(const Container&);
	template<typename U> explicit DynamicArray(const std::initializer_list<U>&);
	template<typename InputIt> DynamicArray(InputIt,InputIt);
	template<typename... Tuple> explicit DynamicArray(std::piecewise_construct_t,Tuple&&...);
	DynamicArray(const DynamicArray&);
	DynamicArray(DynamicArray&&)noexcept(/*see below*/);
	size_type size()const;
	bool empty()const;
	pointer data();
	const_pointer data()const;
	reference operator[](size_type);
	const_reference operator[](size_type)const;
	reference operator()(size_type);
	const_reference operator()(size_type)const;
	iterator begin();
	const_iterator begin()const;
	const_iterator cbegin()const;
	iterator end();
	const_iterator end()const;
	const_iterator cend()const;
	reverse_iterator rbegin();
	const_reverse_iterator rbegin()const;
	const_reverse_iterator crbegin()const;
	reverse_iterator rend();
	const_reverse_iterator rend()const;
	const_reverse_iterator crend()const;
};
```

### Constructors ###

```cpp

```