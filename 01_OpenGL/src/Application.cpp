#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW\glfw3native.h>
#include <chrono>
#include "PlayGround.h"
#include "ApplicationInfo.h"

float MAX_FPS = 16; //(16ms per update)

int l_WindowWidth = (int)ApplicationInfo::windowWidth;
int l_WindowHeight = (int)ApplicationInfo::windowHeight;

int main(void)
{

	if (!glfwInit())
		std::cout << "Init fail! Check WindowInit" << std::endl;
	
	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	GLFWmonitor* monitor = monitors[0];
	//GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	GLFWwindow* window = glfwCreateWindow(l_WindowWidth, l_WindowHeight, "GLwindow", NULL, NULL);
	
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
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
	//glfwMaximizeWindow(window);



	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;


	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "glew version: " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "glfw version: " << glfwGetVersionString() << std::endl;

	GLCall(glEnable(GL_BLEND));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//imgui Init
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	PlayGround game;
	game.window = window;
	game.monitor = monitor;
	game.OnAttach();

	auto last = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(game.window))
	{
		auto time = std::chrono::high_resolution_clock::now() - last;
		
		if(time.count() > MAX_FPS * 1000000)
		{
			last = std::chrono::high_resolution_clock::now();
			game.OnUpdate();
			game.OnRender();

			ImGui_ImplGlfwGL3_NewFrame();

			game.ImGuiOnUpdate();

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			GLCall(glfwSwapBuffers(game.window));
			GLCall(glfwPollEvents());
		}
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
