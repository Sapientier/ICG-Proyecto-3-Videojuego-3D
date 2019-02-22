// Brian Torres 24042974
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <Shader.h>

//#define GLEW_STATIC

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
		std::cout << "Fallo al crear la ventana GLFW" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// Inicializamos en true para que GLEW sepa utilizar un enfoque moderno para recuperar punteros de función y extensiones
	glewExperimental = GL_TRUE;

	// Inicializa GLEW para configurar los punteros de la función OpenGL
	if (GLEW_OK != glewInit()) {
		std::cout << "Fallo al inicializar GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);

	// Habilita soporte alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Construye y compila nuestro programa shader.
	Shader ourShader("resources/shaders/core.vs", "resources/shaders/core.frag");

	// Configurar datos de vértice (y búfer(es)) y punteros de atributo
	GLfloat vertices[] =
	{
		// Posiciones        // Colores			 // Coordenadas de texturas
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
	};

	GLuint indices[] =
	{
		0, 1, 3, // Primer Triangulo
		1, 2, 3  // Segundo Triangulo
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Atributo de posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Atributo de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Atributo de coordenada textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Desenlazar VAO 

	 // Carga y crear la textura
	GLuint texture;

	int width, height;

	// ===================
	// Textura
	// ===================
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Inicializamos los parametros de textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Inicializamos el filtrado de textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Carga, crea la textura y genera mipmaps
	unsigned char *image = SOIL_load_image("resources/images/textura3.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Ciclo del juego
	while (!glfwWindowShouldClose(window)) {
		// Revisa los eventos/entradas
		glfwPollEvents();

		// renderizado
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Dibujamos el triangulo
		ourShader.Use();

		// Creamos las transformaciones
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// Obtener la ubicación uniforme de la matriz y establecemos la matriz
		GLint transformLocation = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, value_ptr(transform));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);

		// Dibuja el contenedor
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Dibujado OPENGL
		glfwSwapBuffers(window);
	}

	// Rechazar adecuadamente todos los recursos una vez que hayan superado su propósito
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Termina GLFW, borrando todos los recursos asignados por GLFW.
	glfwTerminate();

	return EXIT_SUCCESS;
}