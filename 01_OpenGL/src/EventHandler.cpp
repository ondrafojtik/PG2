#include "EventHandler.h"
#include "ApplicationInfo.h"

Camera* EventHandler::camera = nullptr;
bool EventHandler::mouseEnable = false;
MouseRay* EventHandler::mouseRay = new MouseRay();
ObjectManager* EventHandler::object_manager = new ObjectManager();
glm::vec3 EventHandler::originPoint = glm::vec3(0.0f);
glm::vec3 EventHandler::destPoint = glm::vec3(0.0f);

void EventHandler::cursor_pos_callback(GLFWwindow* window, double mouseX, double mouseY)
{
	if (mouseEnable)
		camera->mouseUpdate({ mouseX, mouseY });

	#if DEBUG
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	printf("cursorPos: %d, %d\n", (int)x, (int)y);
	#endif
}

void EventHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_E) && (action == GLFW_PRESS))
	{
		mouseEnable = !mouseEnable;
		printf("Mouse rotation toggle! (current state: %i)\n", (int)mouseEnable);
	}
}

void EventHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		printf("button_callback [%d,%d,%d]\n", button, action, mode);
		glm::dvec2 mouse;
		glfwGetCursorPos(window, &mouse.x, &mouse.y);
		mouseRay->update_camera(camera);
		mouseRay->set_mouse_position(mouse);
		mouseRay->update();

		// remove with given id

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		unsigned int _index;
		glReadPixels(x, glm::abs(y - ApplicationInfo::windowHeight), 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &_index);
		std::cout << "stencil: " << _index << std::endl;
		if (_index != 0)
			object_manager->RemoveWithID(_index);

	}
}
