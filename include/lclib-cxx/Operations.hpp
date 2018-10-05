#ifndef __Operations_hpp_2018_08_07_10_37
#define __Operations_hpp_2018_08_07_10_37


class NotCopyable{
private:
    NotCopyable(const NotCopyable&)=delete;
    NotCopyable& operator=(const NotCopyable&)=delete;
public:
    constexpr NotCopyable(NotCopyable&&)=default;
    constexpr NotCopyable& operator=(NotCopyable&&)=default;
    constexpr NotCopyable()=default;
    ~NotCopyable()=default;
};

class NotMoveable{
private:
    NotMoveable(NotMoveable&&)=delete;
    NotMoveable& operator=(NotMoveable&&)=delete;
public:
    constexpr NotMoveable(const NotMoveable&)=default;
    constexpr NotMoveable& operator=(const NotMoveable&)=default;
    constexpr NotMoveable()=default;
    ~NotMoveable()=default;
};

class NotAssignable{
private:
    NotAssignable& operator=(const NotAssignable&)=delete;
    NotAssignable& operator=(NotAssignable&&)=delete;
public:
    constexpr NotAssignable(const NotAssignable&)=default;
    constexpr NotAssignable(NotAssignable&&)=default;
    constexpr NotAssignable()=default;
    ~NotAssignable()=default;
};

class Disable:NotMoveable,NotCopyable{
private:
    Disable()=delete;
    ~Disable()=delete;
};

#endif
