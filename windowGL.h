#pragma once

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include "includesGL.h"
#include <iostream>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

class WindowGL {

public:
	static int start();

	static GLFWwindow* getWindow();
	static GLboolean* getKeys();
	static double* getMousePos();

	static void update();

	static void end();

private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	static GLboolean Keys[1024];
	static double mousePos[2];
	static GLFWwindow* window;
};