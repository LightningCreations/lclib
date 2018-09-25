/*
 * Shape.hpp
 *
 *  Created on: Sep 10, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_UI_SHAPE_HPP__2018_09_10_12_08
#define __INCLUDE_UI_SHAPE_HPP__2018_09_10_12_08
#include <cstddef>
#include <lclib-cxx/Config.hpp>
namespace graphics{
	struct LIBLCAPI Point{
	public:
		size_t x,y;
		constexpr Point(size_t x,size_t y)noexcept(true):x{x},y{y}{}
		constexpr Point(const Point&)noexcept(true)=default;
		constexpr Point(Point&&)noexcept(true)=default;
		Point(const Point&&)=delete;
		constexpr Point& operator=(const Point&)noexcept(true)=default;
		constexpr Point& operator=(Point&&)noexcept(true)=default;
		Point& operator=(const Point&&)=delete;
		constexpr Point operator+(Point p)const noexcept(true){
			return {x+p.x,y+p.y};
		}
		constexpr Point operator-(Point p)const noexcept(true){
			return {x-p.x,y-p.y};
		}
		constexpr Point operator/(int i)const noexcept(true){
			return {x/i,y/i};
		}
		double distanceTo(Point)const noexcept(true);
	};
	struct LIBLCAPI Shape{
	protected:
		Point c;
		virtual ~Shape()=default;
		Shape(size_t,size_t)noexcept(true);
		Shape(Point)noexcept(true);
	public:
		/**
		 * Checks if a given point is somewhere within the shape
		 */
		virtual bool isWithin(Point)const noexcept(true)=0;
		/**
		 * Returns the greatest vertical distance from the center of the shape to any point on the edge
		 * Used in smallestEnclosingRectangle
		 */
		virtual size_t getMaximumYDistance()const noexcept(true)=0;
		/**
		 * Returns the greatest horizontal distance from the center of the shape to any point on the edge.
		 * Used in smallestEnclosingRectangle
		 */
		virtual size_t getMaximumXDistance()const noexcept(true)=0;
		/**
		 * Gets the center point of the shape
		 */
		Point getCenter()const noexcept(true);
	};
	struct LIBLCAPI Rectangle:Shape{
	private:
		size_t l,w;
	public:
		Rectangle(Point tl,Point br)noexcept(true) EXPECTS(tl.x<br.x&&tl.y<br.y);
		Rectangle(size_t tlx,size_t tly,size_t l,size_t w)noexcept(true);
		bool isWithin(Point)const noexcept(true);
		size_t getMaximumYDistance()const noexcept(true);
		size_t getMaximumXDistance()const noexcept(true);
		bool isSquare()const noexcept(true);
		Point getTopLeft()const noexcept(true);
	};
	struct LIBLCAPI Square:Rectangle{
	public:
		Square(size_t tlx,size_t tly,size_t s)noexcept(true);
	};
	struct LIBLCAPI Elipse:Shape{
	private:
		size_t xr,yr;
	public:
		Elipse(size_t cx,size_t cy,size_t xr,size_t yr)noexcept(true);
		Elipse(Point c,size_t xr,size_t yr)noexcept(true);
		bool isWithin(Point)const noexcept(true);
		size_t getMaximumYDistance()const noexcept(true);
		size_t getMaximumXDistance()const noexcept(true);
		bool isCircle()const noexcept(true);
	};
	struct LIBLCAPI Circle:Elipse{
	public:
		Circle(size_t cx,size_t cy,size_t r)noexcept(true);
		Circle(Point c,size_t r)noexcept(true);
	};
	Rectangle getEnclosingRectange(const Shape&)noexcept(true) LIBLCAPI;
	Rectangle getEnclosingRectangle(const Rectangle&)noexcept(true) LIBLCAPI;
}
#endif /* __INCLUDE_UI_SHAPE_HPP__2018_09_10_12_08 */
