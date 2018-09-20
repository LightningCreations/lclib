/*
 * GraphicsBase.hpp
 *
 *  Created on: Sep 10, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_UI_GRAPHICSBASE_HPP__2018_09_10_09_31
#define __INCLUDE_UI_GRAPHICSBASE_HPP__2018_09_10_09_31
#include <lclib-cxx/Text.hpp>//For TGI Color
#include <cstddef>
#include <cstdint>
#include <lclib-cxx/Config.hpp>
#include <type_traits>
#include <lclib-cxx/TypeTraits.hpp>
namespace graphics{

using TGIColor = ::Color;
	/**
	* Defines a Color Model, and its traits
	* A conforming specialization must have 2 member typedefs:
	* value_type which is T, or const T&, and rgb_type which is either TGIColor or Color,
	* or a constant reference to either. (must support both const lvalues and rvalues)
	* In addition, it must have a static member function toRGB() which accepts a value_type,
	*  and returns remove_cvref_t<rgb_type>,
	* and a static member function fromRGB() which takes a rgb_type and returns a T.
	* The conversion toRGB(fromRGB(rgb)) is not necessarily an identity conversion.
	* It is only required that the resultant color from that expression is "similar" to rgb.
	* By default, identity conversions for the 16 "standard" colors must be supported
	* (black, dark blue, dark green, dark aqua, dark red, dark purple, gold, gray,
	* dark gray, blue, green, aqua, red, purple, yellow, white),
	* however no other colors must support this conversion as an identity.
	* (For example, fromRGB(toRGB(TGIColor::GOLD)) must be either TGIColor::GOLD,
	* or Color(TGIColor::GOLD), however fromRGB(toRGB(Color{101,101,101})) does not have
	* to yield TGIColor{0x656565} or Color{101,101,101}.
	* If the resultant color is not the same, it must be similar.
	* For example, Given n in the range (0,32] all Colors Color{n,n,n} are Similar to TGIColor::WHITE.
	* Colors are similar if they are within a specific range of colors, or if they are similar with the same
	* standard color. An exhaustive list of Similar Colors can be found in the LCLIB C++ GUI Reference Manual (todo)
	* For example: fromRGB(toRGB(Color{101,101,101})) may yield TGIColor::GRAY, or TGIColor::DARK_GRAY,
	* but may not yield TGIColor::GOLD. (By definition The standard colors are similar with themselves)
	* Notes:
	* If remove_cvref_t<rgb_type> is TGIColor, then Alpha may not be preserved,
	* If remove_cvref_t<rgb_type> is Color, then Alpha must be preserved.
	* If type T specializes color_traits in a way that satisfies the above,
	* then T satisfies ColorModel. If remove_cvref_t<rgb_type> is TGIColor, it additionally
	* satisfies OpaqueColorModel. If remove_cvref_t<rgb_type> is Color, it additionally
	* satisfies TransparentColorModel.
	* In addition, if for any value (except unsupported values, such as Control Colors)
	*  of type remove_cvref_t<rgb_type> c,
	* toRGB(fromRGB(c)) is an identity conversion, then T additional satisfies StandardColorModel.
	* (If T satisfies TransparentColorModel then this applies for any Color, not just values of type rgb_type),
	*/
	template<typename T> struct color_traits{};
	/**
	 * Specialization for TGIColor.
	 * TGIColor satisfies ColorModel, OpaqueColorModel, and StandardColorModel
	 */
	template<> struct color_traits<TGIColor>{
	public:
		using value_type = TGIColor;
		using rgb_type = TGIColor;
		constexpr static rgb_type toRGB(value_type t)noexcept(true){
			return t;
		}
		constexpr static TGIColor fromRGB(rgb_type r)noexcept(true){
			return r;
		}
	};
	template<typename T> using rgb_type_t = remove_cvref_t<typename color_traits<T>::rgb_type>;


	class LIBLCAPI Color{
	private:
		uint8_t a,r,g,b;
	public:
		/**
		 * Construct from a Given RGB Color
		 */
		Color(uint8_t,uint8_t,uint8_t)noexcept(true);
		/**
		 * Constructs from a Given RGBA Color
		 * Note that Order is inconsistant with uint32 constructor and conversion,
		 * as Alpha is the last parameter (whereas uint32 conversions are ordered ARGB)
		 */
		Color(uint8_t,uint8_t,uint8_t,uint8_t)noexcept(true);
		/**
		 * Obtains the Color based on the passed TGIColor.
		 * Same as Color(static_cast<uint32_t>(c)).
		 * Undefined Behavior if the Color is a TGI Control Color (value is >0xffffff)
		 */
		Color(TGIColor c)noexcept(true) EXPECTS(!isControl(c));
		/**
		 * Obtains the Color based on the passed TGIColor with a given alpha value.
		 * Same as Color(static_cast<uint32_t>(c)|(a<<uint32_t(24)))
		 * Undefined Behavior if the Color is a TGI Control Color (value is >0xffffff)
		 * If expects contract is provided (C++2a) and Contracts are not disabled,
		 *  throws contract_violation in this case
		 */
		Color(TGIColor c,uint8_t)noexcept(true) EXPECTS(!isControl(c));

		template<typename ColorModel,typename=decltype(color_traits<ColorModel>::toRGB)>
			explicit Color(const ColorModel& c)noexcept(noexcept(color_traits<ColorModel>::toRGB(c))):
			Color(color_traits<ColorModel>::toRGB(c)){}
		/**
		 * Obtains the color from the raw ARGB value
		 * Same as Color(uint8_t(argb>>16),uint8_t(argb>>8),uint8_t(argb),uint8_t(argb>>24))
		 */
		Color(uint32_t)noexcept(true);
		Color(const Color&)=default;
		Color(Color&&)=default;
		Color(const Color&&)=delete;
		Color& operator=(const Color&)=default;
		Color& operator=(Color&&)=default;
		Color& operator=(const Color&&)=delete;
		/**
		 * Convert to Raw RGB, as a TGI Color
		 * same as Color{c.toARGB()&0xffffff};
		 */
		TGIColor toRGB()const noexcept(true);
		/**
		 * Converts to ARGB uint32.
		 * If alpha is 0, same as static_cast<uint32_t>(toRGB());
		 * Otherwise, same as static_cast<uint32_t>(toRGB())|(getAlpha()<<24);
		 */
		uint32_t toARGB()const noexcept(true);
		/**
		 * Converts to a TGI Color.
		 * This discards the Alpha Component
		 * returns effectively TGIColor{r<<16|g<<8|b};
		 */
		operator TGIColor()const noexcept(true);
		/**
		 * Converts to a uint32_t, as though by toARGB().
		 * explicit to give operator ::Color() precedence in static casts and initialization,
		 * as it can generate bigger numbers then is supported in a normal RGB TGIColor Value.
		 * returns effectively a<<24|r<<16|g<<8|b;
		 */
		explicit operator uint32_t()const noexcept(true);

		/**
		 * Conversion constructor to any type which satisfies ColorModel.
		 *
		 */
		template<typename ColorModel,typename=decltype(color_traits<ColorModel>::fromRGB)>
			explicit operator ColorModel()const noexcept(true){
			return color_traits<ColorModel>::fromRGB(*this);
		}
		/**
		 * Obtains the color based on the given Hue, Saturation, and Lightness color.
		 * Each of Hue, Saturation, and Lightness are in the range (0,1)
		 * UB if any are outside of this range (throws contract_violation if
		 */
		static Color fromHSL(float h,float s,float l)noexcept(true) EXPECTS(h>=0&&h<=1&&s>=0&&s<=1&&l>=0&&l<=1);
		/**
		 * Obtains the color based on the given Hue, Saturation, Lightness, and Transparency color.
		 * Each of Hue, Saturation, Lightness, and Transparency are in the range (0,1)
		 */
		static Color fromHSL(float h,float s,float l,float t)noexcept(true) EXPECTS(h>=0&&h<=1&&s>=0&&s<=1&&l>=0&&l<=1&&t<=0&&t>=0);
		/**
		 * Gets the red component of the color
		 */
		uint8_t getRed()const noexcept(true);
		/**
		 * Gets the Blue component of the color
		 */
		uint8_t getBlue()const noexcept(true);
		/**
		 * Gets the Green Component of the Color
		 */
		uint8_t getGreen()const noexcept(true);
		/**
		 * Gets the Alpha Component of the Color
		 */
		uint8_t getAlpha()const noexcept(true);
		/**
		 * Converts the color to HSL and returns the Hue component
		 */
		float getHue()const noexcept(true);
		/**
		 * Converts the Color to HSL and returns the Saturation Component
		 */
		float getSaturation()const noexcept(true);
		/**
		 * Converts the Color to HSL and returns the Lightness Component
		 */
		float getLightness()const noexcept(true);
		/**
		 * Returns the floating point Transparency
		 * Effectively ((a+1)/256.f)
		 */
		float getTransparency()const noexcept(true);
		/**
		 * Combines the Colors using Root-Mean Color Transformation
		 */
		Color operator+(Color)const noexcept(true);
		Color operator+(TGIColor)const noexcept(true);
		Color& operator+=(Color)noexcept(true);
		Color& operator+=(TGIColor)noexcept(true);

		bool operator==(Color)const noexcept(true);
		bool operator!=(Color)const noexcept(true);
		bool operator==(TGIColor)const noexcept(true);
		bool operator!=(TGIColor)const noexcept(true);
	};
	bool operator==(TGIColor,Color)noexcept(true);
	bool operator!=(TGIColor,Color)noexcept(true);
	Color operator+(TGIColor,Color)noexcept(true);
	/**
	 * Specialization of color_traits for Color.
	 * Color Satisfies ColorModel, TransparentColorModel, and StandardColorModel
	 */
	template<> struct color_traits<Color>{
	public:
		using value_type = Color;
		using rgb_type = Color;
		inline rgb_type toRGB(value_type r){
			return r;
		}
		inline value_type fromRGB(rgb_type r){
			return r;
		}
	};
}





#endif /* __INCLUDE_UI_GRAPHICSBASE_HPP__2018_09_10_09_31 */
