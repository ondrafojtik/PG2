#pragma once

#include <GL/glew.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW\glfw3native.h>

#include <iostream>

struct Bounds
{
	float width;
	float height;
};

class Window
{
private: 
	GLFWwindow* m_Window;
	GLFWmonitor* m_Monitor;
	Bounds m_Bounds = { 0, 0 };

public:
	Window(float WindowWidth, float WindowHeight);
	~Window();

	inline GLFWwindow* GetWindow() const { return m_Window; }
	inline GLFWmonitor* GetMonitor() const { return m_Monitor; }
	inline float GetWindowWidth() const { return m_Bounds.width; }
	inline float GetWindowHeight() const { return m_Bounds.height; }

};
