#ifndef __Operations_hpp_2018_08_07_10_37
#define __Operations_hpp_2018_08_07_10_37


class NotCopyable{
private:
    NotCopyable(const NotCopyable&)=delete;
    NotCopyable& operator=(const NotCopyable&)=delete;
};

class NotMoveable{
private:
    NotMoveable(NotMoveable&&)=delete;
    NotMoveable& operator=(NotMoveable&&)=delete;
};

class NotAssignable{
private:
    NotAssignable& operator=(const NotAssignable&)=delete;
    NotAssignable& operator=(NotAssignable&&)=delete;
};

class Disable:NotMoveable,NotCopyable{
private:
    Disable()=delete;
};
#endif