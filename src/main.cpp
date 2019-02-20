// Brian Torres 24042974
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#define GLEW_STATIC
using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Videojuego-3D", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window) {
		cout << "Fallo al crear la ventana GLFW" << endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		cout << "Fallo al inicializar GLEW" << endl;

		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	while (!glfwWindowShouldClose(window)) {
		// Revisa los eventos/entradas
		glfwPollEvents();

		// renderizado
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Dibujado OPENGL
		glfwSwapBuffers(window);
	}

	return EXIT_SUCCESS;
}