/*
 * Dectectors.cpp
 *
 *  Created on: Oct 1, 2018
 *      Author: connor
 */
#include <lclib-cxx/Config.hpp>
#include <lclib-cxx/UI/Graphics.hpp>
#include <vector>
#include <memory>
using namespace graphics;

#ifdef LIBLCCXX_USE_GNOME

#else
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glx.h>

class OpenGLGraphicsContext final:public GraphicsContext{
private:
	std::reference_wrapper<Toolkit> kit;
	GLXWindow window;
	GLXContext ctx;
public:
	OpenGLGraphicsContext(Toolkit& t,GLXWindow window,GLXContext ctx):kit(std::ref(t)),window(window),ctx(ctx){

	}
	Toolkit& getToolkit()const{
		return kit;
	}
};
class OpenGLToolkit final:public Toolkit{
private:
	std::vector<std::weak_ptr<GraphicsContext>> ownedContexts;
public:
	~O
};

#endif
