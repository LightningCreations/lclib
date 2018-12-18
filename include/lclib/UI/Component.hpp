/*
 * Component.hpp
 *
 *  Created on: Oct 1, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_UI_COMPONENT_HPP__2018_10_01_22_47_30
#define __INCLUDE_LCLIB_CXX_UI_COMPONENT_HPP__2018_10_01_22_47_30

#include <lclib/UI/Graphics.hpp>
#include <optional>
#include <vector>
#include <functional>
namespace graphics{
	class Container;
	class Component{
	private:
		friend class Container;
		Graphics gfx;
		Rectangle bounds;
		bool focused;
		bool visible;
		bool invalid;
		std::vector<std::function<void(int,bool,Point)>> clickListeners;
		std::vector<std::function<void(int,bool)>> keyListener;
		mutable std::recursive_mutex mtx;
		void sendMouseDown(int,Point);
		void sendMouseUp(int,Point);
		void sendKeyDown(int);
		void sendKeyUp(int);
	protected:
		Component();
		virtual bool paint(Graphics&)=0;
	public:
		virtual ~Component();
		void repaint();
		Graphics& getGraphics();
		const Rectangle& getBounds()const;
		void setBounds(Rectangle);
		void setVisible(bool);
		bool isVisible()const;
		void setFocused(bool);
		bool isVisible()const;
		void addMouseListener(std::function<void(int,bool,Point)>);
		void addKeyListener(std::function<void(int,bool)>);
		void invalidate();
	};
}


#endif /* __INCLUDE_LCLIB_CXX_UI_COMPONENT_HPP__2018_10_01_22_47_30 */
