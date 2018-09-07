#ifndef __Text_hpp_2018_07_17_16_09
#define __Text_hpp_2018_07_17_16_09
#include <string>
#include <json/json.h>
#include <initializer_list>
#include <vector>
#include <memory>
#include <UUID.hpp>
#include <Version.hpp>
#include <mutex>
#include <Config.hpp>
#include <TypeInfo.hpp>

using std::initializer_list;
using std::vector;
using std::shared_ptr;





enum class Color:unsigned int{
    BLACK=0x000000, DARK_BLUE=0x00007f,DARK_GREEN=0x006400,DARK_AQUA=0x008B8B,
    DARK_RED=0x8B0000,DARK_PURPLE=0x4B0082,GOLD=0xDAA520,GRAY=0x808080,
    DARK_GRAY=0x404040,BLUE=0x0000ff,GREEN=0x00ff00,AQUA=0x00ffff,
    RED=0xff0000,PURPLE=0x800080,YELLOW=0xffff00,WHITE=0xffffff,
    Reset=0xf0f10180,BOLD=0xf0118f00,CLEAR_BOLD=0xf0118f80,
    NONE = 0xf0000000
};

using std::string;



struct endline_t{
public:
    explicit constexpr endline_t()=default;
};

const endline_t endline{};
constexpr bool isControl(Color c){
    return static_cast<uint32_t>(c)>0x1000000;
}
template<Color c> struct background_t{
public:
	static_assert(!isControl(c),"Cannot use a control color in background_t");
    explicit constexpr background_t()=default;
};



template<Color c> struct foreground_t{
public:
	static_assert(!isControl(c),"Cannot use a control color in foreground_t");
    explicit constexpr foreground_t()=default;
};


template<Color c> const foreground_t<c> foreground{};
template<Color c> const background_t<c> background{};

struct tab_t{
public:
    explicit constexpr tab_t()=default; 
};
const tab_t tab;


LIBLCAPI class TextComponent{
private:
    Color c;
    string text;
    bool endl:1;
    bool bg:1;
    bool __tab:1;
public:
    TextComponent();
    TextComponent(const string&);
    TextComponent(string&&);
    TextComponent(const char*);
    TextComponent(Color);
    TextComponent(int);
    TextComponent(double);
    TextComponent(bool);
    TextComponent(const UUID&);
    TextComponent(Version);
    TextComponent(endline_t);
    TextComponent(tab_t);
    template<Color __c> TextComponent(foreground_t<__c>):c(__c),text(),bg(false),__tab(true),endl(false){}
    template<Color __c> TextComponent(background_t<__c>):c(__c),text(),bg(true),__tab(true),endl(false){}
    TextComponent(const TextComponent&)=default;
    TextComponent(TextComponent&&)=default;
    TextComponent(const TextComponent&&)=delete;
    TextComponent& operator=(const TextComponent&)=default;
    TextComponent& operator=(TextComponent&&)=default;
    TextComponent& operator=(const TextComponent&&)=delete;
    Color getColor()const;
    const string& getText()const;
    bool isEndl()const;
    bool isBGColor()const;
    bool isTab()const;
};

/**
 * Terminal is the top level class of the Terminal Graphics Interface,
 * it bridges the High Level TGI system with the lower level implementation
 */
LIBLCAPI class Terminal{
private:
    std::recursive_mutex lock;
    Terminal(const Terminal&)=delete;
    Terminal(Terminal&&)=delete;
    Terminal& operator=(const Terminal&)=delete;
    Terminal& operator=(Terminal&&)=delete;
public:
    /**
     * Constructs a new Terminal. Any resources necessary to utilize the terminal graphics interface
     * are acquired. The terminal is also cleared and the formatting is reset.
     * If another terminal object exists at the time this constructor is used, the behavior is undefined.
     */
    Terminal();
    /**
     * Destroys the Terminal.
     * The terminal is cleared and the formatting is reset,
     * then any resources acquired by the constructor of Terminal are freed.
     */
    ~Terminal();
    
    /**
     * Displays a Given Text Component to the screen.
     * The effects of displaying the component depends on the content
     * \Returns: the current object, for chaining
     * \Exception Guarantee: this method will not throw an exception
     */
    Terminal& print(const TextComponent&)noexcept(true);
    /**
     * Prints a Variable number of Text Components or convertible objects.
     * The effects of displaying any given text component depends on the content.
     * \Returns: the current object, for chaining
     * \Exception Guarantee: this method will not throw an exception
     */
    template<typename... Args> Terminal& print(const TextComponent& first,const TextComponent& second,Args&&... rest)noexcept(true){
        std::lock_guard<std::recursive_mutex> sync(lock);
    	print(first);
        return print(second,rest...);
    }
    /**
     * Clears the terminal and resets the formatting.
     * \Returns: the current object, for chaining
     * \Exception Guarantee: this method will not throw an exception
     */
    Terminal& clear()noexcept(true);
    /**
     * Blocks until the user inputs any key. (the key is not echoed to the screen)
     * \Returns: the current object, for chaining
     * \Exception Guarantee: this method will not throw an exception
     */
    Terminal& wait()noexcept(true);
    /**
     * Blocks until the user inputs any key, and returns the value of that key (the key is not echoed to the screen)
     * \Returns: the code of the key pressed
     * \Exception Guarantee: this method will not throw an exception
     */
    int get()noexcept(true);
};

namespace types{
	template<> struct TypeHash<TextComponent>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return nameHash("TextComponent");
		}
	};
	template<> struct TypeHash<Terminal>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return nameHash("Terminal");
		}
	};
}

#endif
