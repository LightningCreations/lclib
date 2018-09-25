#include <lclib-cxx/Text.hpp>
#include <iostream>
#include <string>

#define RESET "\x1b[0m"

#define FG 38
#define BG 48

using namespace std::string_literals;

LIBLCHIDE string toFColorCode(Color c){
    string target = "\x1b[38;2;"s;
    uint32_t rgb = static_cast<uint32_t>(c);
    uint8_t r = rgb>>16;
    uint8_t g = (rgb>>8)&0xff;
    uint8_t b = rgb&0xff;
    target += std::to_string(int(r));
    target += ";";
    target += std::to_string(int(g));
    target += ";";
    target += std::to_string(int(b));
    target += "m";
    return std::move(target);
}
LIBLCHIDE string toGColorCode(Color c){
    string target = "\x1b[48;2;"s;
    uint32_t rgb = static_cast<uint32_t>(c);
    uint8_t r = rgb>>16;
    uint8_t g = rgb>>8;
    uint8_t b = rgb;
    target += std::to_string(int(r));
    target += ";";
    target += std::to_string(int(g));
    target += ";";
    target += std::to_string(int(b));
    target += "m";
    return std::move(target);
}

LIBLCHIDE string toCommandCode(Color c){
    switch(c){
        case Color::Reset:
            return RESET;
        break;
        case Color::BOLD:
            return "\x1b[1m"s;
        break;
        case Color::CLEAR_BOLD:
            return "\x1b[21m"s;
        break;
    }
}

Terminal::Terminal(){
    clear();
}

Terminal::~Terminal(){
    clear();
}

Terminal& Terminal::print(const TextComponent& t)noexcept(true){
    std::lock_guard<std::recursive_mutex> sync(lock);
    Color c = t.getColor();
    if(c!=Color::NONE){
        if(isControl(c))
            std::cout << toCommandCode(c);
        if(t.isBGColor())
            std::cout << toGColorCode(c);
        else
            std::cout << toFColorCode(c);
    }
    else if(t.isEndl())
        std::cout << std::endl;
    else if(t.isTab())
        std::cout << "\t";
    else
        std::cout << t.getText();
    return *this;
}
