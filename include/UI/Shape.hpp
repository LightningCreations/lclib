/*
 * Shape.hpp
 *
 *  Created on: Sep 10, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_UI_SHAPE_HPP__2018_09_10_12_08
#define __INCLUDE_UI_SHAPE_HPP__2018_09_10_12_08
#include <cstddef>
namespace graphics{
	struct Point{
	public:
		size_t x,y;
		Point(size_t,size_t);
	};
	struct Shape{
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
	struct Rectangle:Shape{
	private:
		size_t l,w;
	public:
		Rectangle(size_t,size_t,size_t,size_t);
		bool isWithin(Point)const noexcept(true);
		size_t getMaximumYDistance()const noexcept(true);
		size_t getMaximumXDistance()const noexcept(true);
		bool isSquare()const noexcept(true);
	};
	struct Square:Rectangle{
	public:
		Square(size_t,size_t,size_t);
	};
	struct Elipse:Shape{
	private:
		size_t xr,yr;
	public:
		Elipse(size_t,size_t,size_t,size_t);
		bool isWithin(Point)const noexcept(true);
		size_t getMaximumYDistance()const noexcept(true);
		size_t getMaximumXDistance()const noexcept(true);
		bool isCircle()const noexcept(true);
	};
	struct Circle:Elipse{
	public:
		Circle(size_t,size_t,size_t);
	};
	Rectangle getMaximumEnclosingRectange(const Shape&)noexcept(true);
	Rectangle getMaximumEnclosingRectangle(const Rectangle&)noexcept(true);
}
#endif /* __INCLUDE_UI_SHAPE_HPP__2018_09_10_12_08 */
