// Brian Torres 24042974
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Shader.h>

//#define GLEW_STATIC
using namespace std;

// Dimensiones de la Ventana
const GLint WIDTH = 800, HEIGHT = 600;

// Funcion Principal, desde aqui la aplicacion inicia y mantiene un ciclo
int main() {

	// Inicia GLFW
	glfwInit();

	// Inicializa todas las opciones requeridas para GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Crea un objeto GLFWwindow que usaremos para funciones de GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Videojuego-3D", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window) {
		cout << "Fallo al crear la ventana GLFW" << endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// Inicializamos en true para que GLEW sepa utilizar un enfoque moderno para recuperar punteros de función y extensiones
	glewExperimental = GL_TRUE;

	// Inicializa GLEW para configurar los punteros de la función OpenGL
	if (GLEW_OK != glewInit()) {
		cout << "Fallo al inicializar GLEW" << endl;

		return EXIT_FAILURE;
	}

	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);

	// Construye y compila nuestro programa shader.
	Shader ourShader("resources/shaders/core.vs", "resources/shaders/core.frag");

	// Configurar datos de vértice (y búfer(es)) y punteros de atributo
	GLfloat vertices[] =
	{
		// Posiciones        // Colores
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Izquierda
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // Derecha
		0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f // Arriba
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Enlazar el objeto de matriz de vértice primero, luego vincular y establecer los buffers de vértice y los punteros de atributo.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Atributo de posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Atributo de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Desenlazar VAO

	// Ciclo del juego
	while (!glfwWindowShouldClose(window)) {
		// Revisa los eventos/entradas
		glfwPollEvents();

		// renderizado
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Dibujamos el triangulo
		ourShader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Dibujado OPENGL
		glfwSwapBuffers(window);
	}

	// Rechazar adecuadamente todos los recursos una vez que hayan superado su propósito
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Termina GLFW, borrando todos los recursos asignados por GLFW.
	glfwTerminate();

	return EXIT_SUCCESS;
}