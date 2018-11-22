/*
 * Exceptions.hpp
 *
 *  Created on: Oct 10, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_DATABASE_EXCEPTIONS_HPP__2018_10_10_16_33_28
#define __INCLUDE_LCLIB_CXX_DATABASE_EXCEPTIONS_HPP__2018_10_10_16_33_28
#include <stdexcept>

namespace db{
	class SQLException:std::runtime_error{
	public:
		using runtime_error::runtime_error;
	};
}




#endif /* __INCLUDE_LCLIB_CXX_DATABASE_EXCEPTIONS_HPP__2018_10_10_16_33_28 */
