#include "windowGL.h"

double WindowGL::mousePos[2] = { SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f };
GLboolean WindowGL::Keys[1024];
GLFWwindow* WindowGL::window;
bool WindowGL::mouseState = false;

int WindowGL::start() {

	//Use OpenGL Version 3.3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create Window
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ani", NULL, NULL);
	if (window == NULL) { //Close If Failed To Create Window

		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	//Setup Reading Inputs
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //Load OpenGL Functions And If They Are Not Loaded End The Program

		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	//Enable OpenGL To Cull Pixels Rendered Behind Others + Enable OpenGL To Render Transparency 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_FRONT);

	return 1;
}

GLFWwindow* WindowGL::getWindow() {

	return window;
}

GLboolean* WindowGL::getKeys() {

	return Keys;
}

double* WindowGL::getMousePos() {

	return mousePos;
}

void WindowGL::update() {

	glfwPollEvents();
}

void WindowGL::framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);
}

void WindowGL::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_TAB && action == GLFW_RELEASE) {
		mouseState = !mouseState;
		if (mouseState == true) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	if (key == GLFW_KEY_TAB && action == GLFW_RELEASE) {
		
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			Keys[key] = GL_TRUE;
		}
		else if (action == GLFW_RELEASE) {
			Keys[key] = GL_FALSE;
		}
	}
}

void WindowGL::mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	mousePos[0] = xpos;
	mousePos[1] = ypos;
}

void WindowGL::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	//camera.ProcessMouseScroll(yoffset);
}

void WindowGL::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	//When mouse button is pressed
	if (button >= 0 && button < 1024) {
		if (action == GLFW_PRESS) {
			Keys[button] = GL_TRUE;
		}
		else if (action == GLFW_RELEASE) {
			Keys[button] = GL_FALSE;
		}
	}
}

void WindowGL::end() {

	glfwTerminate();
}