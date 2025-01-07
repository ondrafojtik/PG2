#pragma once

#include <stdio.h>
#include "ObjectManager.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "MouseRay.h"
#include "glm/vec4.hpp"

#define DEBUG 0

static class EventHandler
{
private:
	static bool mouseEnable;

public:

	EventHandler() = delete;

	static Camera* camera;
	static MouseRay* mouseRay;
	static ObjectManager* object_manager;
	static glm::vec3 originPoint;
	static glm::vec3 destPoint;
	
	static void cursor_pos_callback(GLFWwindow* window, double mouseX, double mouseY);

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
	
	static bool GetMouseEnabled() { return mouseEnable; }

};
