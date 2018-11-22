/*
 * GraphicsSpi.hpp
 *
 *  Created on: Sep 25, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_UI_GRAPHICSSPI_HPP__2018_09_25_12_10_12
#define __INCLUDE_LCLIB_CXX_UI_GRAPHICSSPI_HPP__2018_09_25_12_10_12
#include <lclib-cxx/UI/GraphicsForwards.hpp>
#include <lclib-cxx/UI/GraphicsBase.hpp>
#include <lclib-cxx/UI/Shape.hpp>
#include <lclib-cxx/Config.hpp>
#include <memory>

namespace graphics{
	/**
	 * Toolkit is the base type for the LCLib-C++ GUI Service Provider Framework.
	 * Toolkit is derived by the implementation to provide.
	 * A toolkit may be used by multiple threads. Toolkit MUST be Thread-Safe
	 */
	class Toolkit{
	private:
		Toolkit(const Toolkit&)=delete;
		Toolkit& operator=(const Toolkit&)=delete;
	protected:
		mutable std::recursive_mutex mtx;
		/**
		 * Cannot be created directly
		 */
		Toolkit()=default;
		/**
		 * Cannot delete pointer-to-base
		 */
		~Toolkit()=default;
	public:
		/**
		 * Creates a new Window Context.
		 * The GraphicsContext resulting should be independant of (though is drawn on top of),
		 * the Environment Context.
		 * \Exceptions:
		 *  \HeadlessException: if isHeadless() is true
		 *  \ResourcesUnavailbleException: if a new GraphicsContext cannot be created.
		 *  \GraphicsAccessInvalidException: If the OS or some intermediate Graphics Library denies the application from creating a new Context
		 *  \Unspecified Subclass of GraphicsException: If some other problem goes wrong
		 * \Returns:
		 *  \A Shared Handle to the new Graphics Context. The bounds specified are only a hint, the Toolkit is allowed to assign any position and/or size to the Context
		 * \Preconditions:
		 *  \The behavior is Implementation Defined (and may be undefined) if the Rectangle does overlap the Environment bounds.
		 */
		virtual std::shared_ptr<GraphicsContext> newWindowContext(const Rectangle&)=0;
		/**
		 * Checks if the given Graphics Context can validly have setUndecorated applied to it.
		 * If True, then ignoring concurrency requirements, a concurrent thread which executes setUndecorated with that context,and true,
		 * will not throw an exception. If false, such a request will yield some subclass of GraphicsException
		 * If the parameter points to the Environment, checks if all GraphicsContexts support this by default.
		 *  (same as supportsUndecorated())
		 */
		virtual bool supportsUndecorated(const std::shared_ptr<GraphicsContext>&)const=0;
		/**
		 * Effectively the same as supportsUndecorated(getEnvironment()) except that no exception will be thrown
		 * for the call to getEnvironment().
		 * Checks if by default, any GraphicsContext owned by this Toolkit supports being undecorated.
		 */
		virtual bool supportsUndecorated()const=0;
		/**
		 * Makes the given graphics context undecorated (that is, does not have exit/maximize/iconify buttons to top bar).
		 * Throws a GraphicsAccessInvalidException if the second parameter is true,
		 *  and if concurrently calling supportsUndecorated on the graphics context returns false.
		 */
		virtual void setUndecorated(const std::shared_ptr<GraphicsContext>&,bool)=0;
		/**
		 * Checks if the Toolkit is headless, that is, is not tied to any sort of Environment.
		 * Calling any methods which create new or access special graphics contexts will
		 * throw a HeadlessException
		 */
		virtual bool isHeadless()const=0;
		/**
		 * Plays an audio alert (usually a beep) if the implementation supports it.
		 * Returns true if the alert was played.
		 */
		virtual bool beep()const=0;
		/**
		 * Checks if RGB is supported by the Underlying Graphics Implementation.
		 * If true, all colors in the RGB Color Space Must be supported.
		 * Otherwise, Only the 15 standard Colors must be supported.
		 * If false, and a pixel is set with a non-standard color,
		 * the resultant color of that pixel is an unspecified similar color.
		 */
		virtual bool supportsRGB()const=0;
		/**
		 * Obtains the size of the environment.
		 * Effectively the same as getEnvironment()->getGraphicsBounds(),
		 * except that the call to getEnvironment() does not check access.
		 * If the Toolkit is headless, throws a HeadlessException.
		 * Otherwise returns some rectangle.
		 */
		virtual const Rectangle& getEnvironmentSize()const=0;
		/**
		 * Gets the number of Desktop Environments availble.
		 * By default this number is 1.
		 * If the toolkit supports multiple Desktop Environments, and detects more than one Monitor is availble,
		 * then the implementation may return something greater then that.
		 * If the toolkit is headless, throws a HeadlessException.
		 */
		virtual std::size_t getDesktopCount()const=0;
		/**
		 * Obtains a Shared Pointer to the Basic Graphics Environment.
		 * If The implementation or the OS denies access, throws a GraphicsAccessInvalidException.
		 * Otherwise, if the Toolkit is headless, throws a HeadlessException.
		 */
		virtual std::shared_ptr<GraphicsContext> getEnvironment()const=0;
		/**
		 * Obtains a shared pointer to the Desktop Environment specified by n.
		 * This has the same requirements as getEnvironment().
		 * The behavior is undefined if n is not less than getDesktopCount() (Contract Enforced if available and enabled)
		 */
		virtual std::shared_ptr<GraphicsContext> getDesktopEnvironment(std::size_t n)const=0 EXPECTS(n<getDesktopCount());
	};
	/**
	 * Handle for a GraphicsContext, used by a Graphics.
	 * Multiple Graphics objects will share a single GraphicsContext (graphics contexts are not copyable)
	 */
	class GraphicsContext{
	private:
		GraphicsContext(const GraphicsContext&)=delete;
		GraphicsContext& operator=(const GraphicsContext&)=delete;
	protected:
		GraphicsContext()=default;
	public:
		virtual ~GraphicsContext();
		virtual void setPixel(Color c,Point p)=0;
		virtual void blend(Color c,Point p);
		virtual void draw(const Shape& s,Color c);
		virtual void draw(const Shape& s,const Image& img);
		virtual Color getPixel(Point p)=0;
		virtual const Rectangle& getGraphicsBounds()const=0;
		virtual bool setDimensions(const Rectangle&)=0;
		virtual void setVisible(bool)=0;
		virtual bool isVisible()const=0;
		virtual void maximize()=0;
		virtual void minimize()=0;
		virtual void iconify()=0;
		virtual void deiconify()=0;
		/**
		 * Gets a reference to the owning Toolkit.
		 */
		virtual Toolkit& getToolkit()const=0;
	};
	Toolkit& getDefaultToolkit();
	class Graphics{
	private:
		mutable std::recursive_mutex mtx;
		std::shared_ptr<GraphicsContext> ctx;
		Rectangle region;
	public:
		Graphics();
		Graphics(std::shared_ptr<GraphicsContext>,const Rectangle&);
		Graphics getSubregion(const Rectangle&)const;
		const Rectangle& getRegion()const;
		void resizeAndMove(const Rectangle&);
		void draw(const Shape&,Color);
		void draw(const Shape&,Image);
	};
}

#endif /* __INCLUDE_LCLIB_CXX_UI_GRAPHICSSPI_HPP__2018_09_25_12_10_12 */
