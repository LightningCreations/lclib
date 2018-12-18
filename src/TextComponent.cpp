#include <lclib/Text.hpp>
#include <string>

using std::string;
using std::to_string;


    LIBLCAPI TextComponent::TextComponent():__tab(false),text(),c(Color::NONE),endl(false),bg(false){}
    LIBLCAPI TextComponent::TextComponent(const string& text):__tab(false),text(text),c(Color::NONE),endl(false),bg(false){}
    LIBLCAPI TextComponent::TextComponent(string&& text):__tab(false),text(text),c(Color::NONE),endl(false),bg(false){}
    LIBLCAPI TextComponent::TextComponent(Color color):__tab(false),text(),c(color),endl(false),bg(false){}
    LIBLCAPI TextComponent::TextComponent(int i):TextComponent(to_string(i)){}
    LIBLCAPI TextComponent::TextComponent(double d):TextComponent(to_string(d)){}
    LIBLCAPI TextComponent::TextComponent(bool b):TextComponent((b?"true":"false")){}
    LIBLCAPI TextComponent::TextComponent(const UUID& u):TextComponent(u.toString()){}
    LIBLCAPI TextComponent::TextComponent(Version v):TextComponent(std::string(v)){}
    LIBLCAPI TextComponent::TextComponent(endline_t):__tab(false),endl(true),c(Color::NONE),bg(false){}
    LIBLCAPI TextComponent::TextComponent(tab_t):__tab(true),endl(false),c(Color::NONE),bg(false){}
    LIBLCAPI TextComponent::TextComponent(const char* c):__tab(false),text(c),c(Color::NONE),endl(false),bg(false){}
    LIBLCAPI TextComponent::TextComponent(Color c,in_background_t):__tab(false),text(),c(c),endl(false),bg(true){}
    LIBLCAPI Color TextComponent::getColor()const{
        return c;
    }
    LIBLCAPI const string& TextComponent::getText()const{
        return text;
    }
    LIBLCAPI bool TextComponent::isEndl()const{
        return endl;
    }
    LIBLCAPI bool TextComponent::isBGColor()const{
        return bg;
    }
    LIBLCAPI bool TextComponent::isTab()const{
        return __tab;
    }
