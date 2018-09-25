/*
 * GraphicsBase.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: connor
 */
#include <lclib-cxx/UI/GraphicsBase.hpp>
#include <type_traits>
#include <algorithm>
#include <cmath>
#include <lclib-cxx/Config.hpp>

namespace graphics{
	namespace detail{
		LIBLCHIDE float getColorValue(float p,float q,float t){
            if(t < 0) t += 1;
            if(t > 1) t -= 1;
            if(t < 1/6) return p + (q - p) * 6 * t;
            if(t < 1/2) return q;
            if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
            return p;
		}
		LIBLCHIDE float combine(float a,float b,float t){
			return std::sqrt((1-t)*a*a+t*b*b);
		}
		LIBLCHIDE uint8_t combineAlpha(uint8_t a,uint8_t b){
			return std::max(a,b);
		}
	}
	Color::Color(uint8_t r,uint8_t g,uint8_t b)noexcept(true):r(r),g(g),b(b),a(0){}
	Color::Color(uint8_t r,uint8_t g,uint8_t b,uint8_t a)noexcept(true):r(r),g(g),b(b),a(a){}
	Color::Color(TGIColor c)noexcept(true):Color(static_cast<uint32_t>(c)){}
	Color::Color(uint32_t argb)noexcept(true):a(argb>>24),r(argb>>16),g(argb>>8),b(argb){}
	TGIColor Color::toRGB()const noexcept(true){
		return TGIColor{uint32_t(r)<<16|uint32_t(g)<<8|b};
	}
	uint32_t Color::toARGB()const noexcept(true){
		return uint32_t(a)<<24|uint32_t(r)<<16|uint32_t(g)<<8|b;
	}
	Color::operator TGIColor()const noexcept(true){
		return toRGB();
	}
	Color::operator uint32_t()const noexcept(true){
		return toARGB();
	}
	Color Color::fromHSL(float h,float s,float l)noexcept(true){
		if(s==0)
			return Color(255*l,255*l,255*l);
        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        float r = detail::getColorValue(q,p,h+(1/3.f));
        float g = detail::getColorValue(q,p,h);
        float b = detail::getColorValue(q,p,h-(1/3.f));
        return Color(255*r,255*g,255*b);
	}
	Color Color::fromHSL(float h,float s,float l,float a)noexcept(true){
		if(s==0)
			return Color(255*l,255*l,255*l);
        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        float r = detail::getColorValue(q,p,h+(1/3.f));
        float g = detail::getColorValue(q,p,h);
        float b = detail::getColorValue(q,p,h-(1/3.f));
        return Color(255*r,255*g,255*b,255*a);
	}
	uint8_t Color::getRed()const noexcept(true){
		return r;
	}
	uint8_t Color::getBlue()const noexcept(true){
		return b;
	}
	uint8_t Color::getGreen()const noexcept(true){
		return g;
	}
	uint8_t Color::getAlpha()const noexcept(true){
		return a;
	}
	float Color::getHue()const noexcept(true){
		uint8_t max = std::max(r,std::max(g,b)),min = std::min(r,std::min(g,b));
		if(max==min)
			return 0;
		else{
			float h;
			float d = max-min;
	            if(r==max) h = (g - b) / d + (g < b ? 6 : 0);
	            else if(g==max) h = (b - r) / d + 2;
	            else h = (r - g) / d + 4;
	        h/=6;
	        return h;
		}
	}
	float Color::getSaturation()const noexcept(true){
		uint8_t max = std::max(r,std::max(g,b)),min = std::min(r,std::min(g,b));
		if(max==min)
			return 0;
		else{
			float d = max-min;
			return (((max+min)/2.f) > 0.5 )?( d / (2 - max - min)) :( d / (max + min));
		}
	}
	float Color::getLightness()const noexcept(true){
		uint8_t max = std::max(r,std::max(g,b)),min = std::min(r,std::min(g,b));
		return (max+min)/2.f;
	}
	float Color::getTransparency()const noexcept(true){
		return (a+1)/256.f;
	}
	Color Color::operator+(Color c)const noexcept(true){
		float t = (getTransparency()+c.getTransparency())/2;
		return Color(256*detail::combine(r/256.f,c.r/256.f,t),
				256*detail::combine(g/256.f,c.g/256.f,t),
				256*detail::combine(b/256.f,c.b/256.f,t),
				std::max(a,c.a));
	}
	Color Color::operator+(TGIColor c)const noexcept(true){
		float t = (getTransparency()+1.f)/2;
		return Color(256*detail::combine(r/256.f,((static_cast<uint32_t>(c)>>16))/256.f,t),
				256*detail::combine(g/256.f,((static_cast<uint32_t>(c)>>8)&0xff)/256.f,t),
				256*detail::combine(b/256.f,(static_cast<uint32_t>(c)&0xff)/256.f,t));
	}
	Color& Color::operator+=(Color c)noexcept(true){
		return (*this = (*this+c));
	}
	Color& Color::operator+=(TGIColor c)noexcept(true){
		return (*this = (*this+c));
	}
	bool Color::operator==(Color c)const noexcept(true){
		return r==c.r&&g==c.g&&b==c.b&&a==c.a;
	}
	bool Color::operator!=(Color c)const noexcept(true){
		return !(*this==c);
	}
	Color operator+(TGIColor c1,Color c2)noexcept(true){
		return c2+c1;
	}
	bool operator==(TGIColor c1,Color c2)noexcept(true){
		return c2==c1;
	}
	bool operator!=(TGIColor c1,Color c2)noexcept(true){
		return c2!=c1;
	}
}

