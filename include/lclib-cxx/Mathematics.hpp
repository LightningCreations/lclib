/*
 * Mathematics.hpp
 *
 *  Created on: Sep 3, 2018
 *      Author: connor
 */

#ifndef INCLUDE_MATHEMATICS_HPP_
#define INCLUDE_MATHEMATICS_HPP_
#include <type_traits>
#include <limits>
#include <functional>
#include <lclib-cxx/Operations.hpp>
#include <cmath>

constexpr const double dx = std::numeric_limits<double>::min();

struct Expression;

struct ExpressionWrapper{
private:
	const Expression* expn;
public:
	ExpressionWrapper(const Expression&);
	ExpressionWrapper operator+(const ExpressionWrapper&)const;
	ExpressionWrapper operator-(const ExpressionWrapper&)const;
	ExpressionWrapper operator*(const ExpressionWrapper&)const;
	ExpressionWrapper operator/(const ExpressionWrapper&)const;
	ExpressionWrapper operator^(const ExpressionWrapper&)const;
	ExpressionWrapper operator+(const ExpressionWrapper&)const;
	ExpressionWrapper operator-(const Expression&)const;
	ExpressionWrapper operator*(const Expression&)const;
	ExpressionWrapper operator/(const Expression&)const;
	ExpressionWrapper operator^(const Expression&)const;
	ExpressionWrapper operator+(double)const;
	ExpressionWrapper operator-(double)const;
	ExpressionWrapper operator*(double)const;
	ExpressionWrapper operator/(double)const;
	ExpressionWrapper operator^(double)const;
};

struct Expression{
public:
	virtual ~Expression()=default;
	ExpressionWrapper operator+(const ExpressionWrapper&)const;
	ExpressionWrapper operator-(const ExpressionWrapper&)const;
	ExpressionWrapper operator*(const ExpressionWrapper&)const;
	ExpressionWrapper operator/(const ExpressionWrapper&)const;
	ExpressionWrapper operator^(const ExpressionWrapper&)const;
	ExpressionWrapper operator+(const ExpressionWrapper&)const;
	ExpressionWrapper operator-(const Expression&)const;
	ExpressionWrapper operator*(const Expression&)const;
	ExpressionWrapper operator/(const Expression&)const;
	ExpressionWrapper operator^(const Expression&)const;
	/**
	 * Applies the expression and any subexpressions
	 */
	virtual double apply(double)const noexcept(true)=0;
	/*
	 * Computes the differential of the function (that is, the derivative*dx)
	 */
	virtual ExpressionWrapper differential()const noexcept(true)=0;
};


struct ln:public Expression{
private:
	ExpressionWrapper inner;
public:
	ln(const Expression& e);
	double apply(double)const noexcept(true);
	ExpressionWrapper differential()const noexcept(true);
};

/*
 * Computes the differential of expression.
 * To compute the derivative, call this function and devide by dx
 */
ExpressionWrapper d(const Expression&);




#endif /* INCLUDE_MATHEMATICS_HPP_ */
