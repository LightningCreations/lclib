#ifndef __Text_hpp_2018_07_17_16_09
#define __Text_hpp_2018_07_17_16_09
#include <string>
#include <initializer_list>
#include <vector>
#include <memory>
#include <lclib/UUID.hpp>
#include <lclib/Version.hpp>
#include <mutex>
#include <lclib/Config.hpp>
#include <json/json.h>
#include <utility>
using std::initializer_list;
using std::vector;
using std::shared_ptr;




/**
 * Color Representing an Text Color.
 * 16 Standard Colors are defined. Implementations may also support the full RGB Color space
 * in the Terminal. If not, the 16 standard colors must at least be supported, and similar color rules (from UI/Colors.md) apply.
 * There are also a set of Control Codes which are used in place of a Color Value to specify some
 * action to take. These "Control Colors" are defined with values greater than 0xffffff (RGB White).
 * 4 Standard Control Codes are defined, 2 of which are required to be supported (Reset and NONE),
 * and 2 which are optional to support. The effects of using a Non-standard Control Code is implementation defined.
 * Reset clears the background and foreground colors and reverts them to the terminal default.
 * NONE specifies that no color is used and that the current color is to be used
 * BOLD and CLEAR_BOLD set and Clear respectively Bold Text. If either are supported, both must be supported.
 */
enum class Color:unsigned int{
    BLACK=0x000000, DARK_BLUE=0x00007f,DARK_GREEN=0x006400,DARK_AQUA=0x008B8B,
    DARK_RED=0x8B0000,DARK_PURPLE=0x4B0082,GOLD=0xDAA520,GRAY=0x808080,
    DARK_GRAY=0x404040,BLUE=0x0000ff,GREEN=0x00ff00,AQUA=0x00ffff,
    RED=0xff0000,PURPLE=0x800080,YELLOW=0xffff00,WHITE=0xffffff,
    Reset=0xf0f10180,BOLD=0xf0118f00,CLEAR_BOLD=0xf0118f80,
    NONE = 0xf0000000
};

using std::string;


/**
 * Tag type for specifying that a newline character (or newline, carriage return) must be printed
 * and the buffer flushed.
 */
struct endline_t{
public:
    explicit constexpr endline_t()=default;
};

/**
 * Tag for selecting newline print
 */
const endline_t endline{};
/**
 * Checks if a given color is a control color.
 * Control Colors can only be used in certain cases, and never used to specify foreground or background color.
 */
constexpr bool isControl(Color c){
    return static_cast<uint32_t>(c)>=0x1000000;
}

/**
 * Tag type for statically specifying the background color.
 * The Color is then carried through until the color scheme is reset or the background color is changed.
 * A program which instantiates background_t for a Control color is Ill-Formed.
 */
template<Color c> struct background_t{
public:
	static_assert(!isControl(c),"Cannot use a control color in background_t");
    explicit constexpr background_t()=default;
};


/**
 * Tag type for statically specifying the foreground color.
 * The Color is then carried through until the color scheme is reset or the foreground color is changed.
 * A program which instantiates foreground_t for a Control color is Ill-Formed.
 */
template<Color c> struct foreground_t{
public:
	static_assert(!isControl(c),"Cannot use a control color in foreground_t");
    explicit constexpr foreground_t()=default;
};

/**
 * Tags for specifying foreground and background color.
 */
template<Color c> const foreground_t<c> foreground{};
template<Color c> const background_t<c> background{};

/**
 * Tag type for the directive to print a tab character ('\t' or some equivalent).
 */
struct tab_t{
public:
    explicit constexpr tab_t()=default; 
};
/**
 * Tab tag
 */
const tab_t tab{};

/**
 * Tag type for specifying a dynamic background color
 */
struct in_background_t{
public:
	explicit constexpr in_background_t()=default;
};
/**
 * in_background tag.
 */
const in_background_t in_background{};

/**
 * TextComponent is the basic type for using the TGI.
 */
class LIBLCAPI TextComponent{
private:
    Color c;
    string text;
    bool endl:1;
    bool bg:1;
    bool __tab:1;
public:
    /*
     * Default constructs a TextComponent.
     * Printing this component has no effect
     */
    TextComponent();
    /**
     * Constructs a Text Component from a given string.
     * Printing this component will result in the string being printed to the screen
     */
    TextComponent(const std::string&);
    /**
     * Constructs a Text Component from a given string.
     * Printing this component will result in the string being printed to the screen
     */
    TextComponent(std::string&&);
    /**
     * Constructs a Text Component from a given string.
     * Printing this component will result in the string being printed to the screen
     */
    TextComponent(const char*);
    /**
     * Constructs a Text Component from the given Color.
     * If Color represents a Control Code, the effects of printing it are Implementation Defined
     *  (except for the 4 standard Control Codes).
     * If the Color is an RGB Color, printing it changes the foreground to the specified Color
     */
    TextComponent(Color);
    /**
     * Constructs a Text Component from the given Color, setting it as the background.
     * Printing the Component changes the background color the specified color.
     * The behavior is undefined if the Color represents a Control Code
     */
    TextComponent(Color,in_background_t);
    /**
     * Prints the Given int value. TextComponent(i) is the same as TextComponent(to_string(i))
     */
    TextComponent(int);
    /**
     * Prints the given double value. TextComponent(d) is the same as TextComponent(to_string(d))
     */
    TextComponent(double);
    /**
     * Prints the given boolean. TextComponent(b) is the same as TextComponent(b?"true":"false")
     */
    TextComponent(bool);
    /**
     * Prints the given UUID by converting it to a string.
     */
    TextComponent(const UUID&);
    /**
     * Prints the given Version by converting it to a string.
     */
    TextComponent(Version);
    /**
     * Tag Constructor for the endline tag.
     * Printing this will print a platform dependent Linefeed String and flush
     * the terminal (if it buffers output).
     */
    TextComponent(endline_t);
    /**
     * Tag Constructor for the tab tag.
     */
    TextComponent(tab_t);
    /**
     * Tag Constructor for statically setting the foreground.
     * Printing this component will set the foreground color to the given input color.
     */
    template<Color __c> TextComponent(foreground_t<__c>):c(__c),text(),bg(false),__tab(true),endl(false){}
    /**
     * Tag Constructor for statically setting the background.
     * Printing this component will set the background color to the given input color.
     */
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
 * Terminals are Not Copyable and are thread safe. (Though the effects of using a Terminal accross multiple threads concurrently is unspecified).
 * Terminal will acquire resources necessary to operate the TGI at construction,
 * The Terminal Object then becomes the owner of the Handles to the Terminal Input and Output.
 * The behavior is undefined if these handles are interacted with in any way,
 * except through the Terminal object, if such an object is in lifetime.
 */
class LIBLCAPI Terminal{
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
     * There may not be 2 active instances of Terminal in a given program.
     * The behavior is undefined if a Terminal is constructed while another Terminal object is in Lifetime.
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
    template<typename... Args> std::enable_if_t<std::conjunction_v<std::is_convertible<Args,TextComponent>...>,Terminal>& print(Args&&... rest)noexcept(true)
	{
        std::lock_guard<std::recursive_mutex> sync(lock);
        const TextComponent comp[]{std::forward<Args>(rest)...};
    	for(const TextComponent& a:comp)
    		print(a);
    	return *this;
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


#endif
