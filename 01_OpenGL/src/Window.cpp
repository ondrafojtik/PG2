#include "Window.h"

Window::Window(float WindowWidth, float WindowHeight) : m_Bounds{ WindowWidth, WindowHeight }
{
	if (!glfwInit())
		std::cout << "Init fail! Check WindowInit" << std::endl;

	m_Monitor = glfwGetPrimaryMonitor();
	m_Window = glfwCreateWindow(m_Bounds.width, m_Bounds.height, "GLwindow", NULL, NULL);

	const GLFWvidmode* mode = glfwGetVideoMode(m_Monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	/////////////////////////////////////////////////////////////////////////////
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); //VSYNCH
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_TRUE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
	glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_FALSE);
	glfwMaximizeWindow(m_Window);

}

Window::~Window()
{
}
