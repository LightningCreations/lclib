/*
 * Shape.cpp
 *
 *  Created on: Sep 20, 2018
 *      Author: connor
 */


#include <lclib-cxx/UI/Shape.hpp>
#include <cmath>

namespace graphics{
	Shape::Shape(size_t cx,size_t cy)noexcept(true):c{cx,cy}{}
	Shape::Shape(Point c)noexcept(true):c{c}{}
	Point Shape::getCenter()const noexcept(true){
		return c;
	}
	Rectangle::Rectangle(Point tl,Point br)noexcept(true) EXPECTS(tl.x<br.x&&tl.y<br.y):Shape{(tl+br)/2},l{br.x-tl.x},w{br.y-tl.y}{}
	Rectangle::Rectangle(size_t tlx,size_t tly,size_t l,size_t w)noexcept(true):Shape{(tlx+l)/2,(tly+w)/2},l{l},w{w}{}
	bool Rectangle::isWithin(Point p)const noexcept(true){
		Point c = getTopLeft();
		return p.x>c.x&&p.x<(c.x+l)&&p.y>c.y&&p.y<(c.y+w);
	}
	size_t Rectangle::getMaximumXDistance()const noexcept(true){
		return l/2;
	}
	size_t Rectangle::getMaximumYDistance()const noexcept(true){
		return w/2;
	}
	bool Rectangle::isSquare()const noexcept(true){
		return w==l;
	}
	Point Rectangle::getTopLeft()const noexcept(true){
		return getCenter()-Point{l/2,w/2};
	}
	Square::Square(size_t tlx,size_t tly,size_t s)noexcept(true):Rectangle{tlx,tly,s,s}{}

	Elipse::Elipse(Point c,size_t xr,size_t yr)noexcept(true):Shape{c},xr{xr},yr{yr}{}
	Elipse::Elipse(size_t cx,size_t cy,size_t xr,size_t yr)noexcept(true):Shape{cx,cy},xr{xr},yr{yr}{}
	bool Elipse::isWithin(Point p)const noexcept(true){
		Point c = getCenter();
		return ((c.x-double(p.x))*(c.x-double(p.x))/(xr*xr)+(c.y-double(p.y))*(c.y-double(p.y))/yr*yr)<1;
	}
	size_t Elipse::getMaximumXDistance()const noexcept(true){
		return xr;
	}
	size_t Elipse::getMaximumYDistance()const noexcept(true){
		return yr;
	}
	bool Elipse::isCircle()const noexcept(true){
		return xr==yr;
	}
	Circle::Circle(size_t cx,size_t cy,size_t r)noexcept(true):Elipse{cx,cy,r,r}{}
	Circle::Circle(Point c,size_t r)noexcept(true):Elipse{c,r,r}{}
	Rectangle getEnclosingRectangle(const Shape& s)noexcept(true){
		Point c = s.getCenter();
		return Rectangle{c.x-s.getMaximumXDistance(),c.y-s.getMaximumYDistance(),2*s.getMaximumXDistance(),2*s.getMaximumYDistance()};
	}
	Rectangle getEnclosingRectange(const Rectangle& r)noexcept(true){
		return r;
	}
}


