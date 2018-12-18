/*
 * GraphicsExceptions.hpp
 *
 *  Created on: Sep 25, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_UI_GRAPHICSEXCEPTIONS_HPP__2018_09_25_14_02_05
#define __INCLUDE_LCLIB_CXX_UI_GRAPHICSEXCEPTIONS_HPP__2018_09_25_14_02_05
#include <lclib/UI/GraphicsForwards.hpp>
#include <exception>
namespace graphics{
	class GraphicsException:public std::exception{};
	class ResourcesUnavailbleException final:public GraphicsException{
		const char* const what()const noexcept(true);
	};
	class HeadlessException final:public GraphicsException{
		const char* const what()const noexcept(true);
	};
	class GraphicsAccessInvalidException final:public GraphicsException{
		const char* const what()const noexcept(true);
	};
}




#endif /* __INCLUDE_LCLIB_CXX_UI_GRAPHICSEXCEPTIONS_HPP__2018_09_25_14_02_05 */
