#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Define varias opciones posibles para el movimiento de la cámara. Se utiliza como abstracción para mantenerse alejado de los métodos de entrada específicos del sistema de ventanas
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Valores por defecto de la camara
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

// Una clase de cámara abstracta que procesa la entrada y calcula los ángulos, vectores y matrices de Euler correspondientes para su uso en OpenGL
class Camera
{
public:
	// Constructor con vectores
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Constructor con escalares
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Devuelve la matriz de visualización calculada utilizando los ángulos de Euler y la matriz LookAt
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	// Procesa la entrada recibida desde cualquier sistema de entrada tipo teclado. Acepta el parámetro de entrada en la forma de cámara definida ENUM (para abstraerla de los sistemas de ventanas)
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->movementSpeed * deltaTime;

		if (direction == FORWARD)
		{
			this->position += this->front * velocity;
		}

		if (direction == BACKWARD)
		{
			this->position -= this->front * velocity;
		}

		if (direction == LEFT)
		{
			this->position -= this->right * velocity;
		}

		if (direction == RIGHT)
		{
			this->position += this->right * velocity;
		}
	}

	// Procesa la entrada recibida desde un sistema de entrada de ratón. Espera el valor de desplazamiento tanto en la dirección x como en la y
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
	{
		xOffset *= this->mouseSensitivity;
		yOffset *= this->mouseSensitivity;

		this->yaw += xOffset;
		this->pitch += yOffset;

		// Asegúrese de que cuando el tono está fuera de límites, la pantalla no se voltea
		if (constrainPitch)
		{
			if (this->pitch > 89.0f)
			{
				this->pitch = 89.0f;
			}

			if (this->pitch < -89.0f)
			{
				this->pitch = -89.0f;
			}
		}

		// Actualice los vectores de frente, derecha y arriba utilizando los ángulos de Euler actualizados
		this->updateCameraVectors();
	}

	// Procesa la entrada recibida de un evento de la rueda de desplazamiento del ratón. Solo requiere entrada en el eje vertical de la rueda
	void ProcessMouseScroll(GLfloat yOffset)
	{
		if (this->zoom >= 1.0f && this->zoom <= 45.0f)
		{
			this->zoom -= yOffset;
		}

		if (this->zoom <= 1.0f)
		{
			this->zoom = 1.0f;
		}

		if (this->zoom >= 45.0f)
		{
			this->zoom = 45.0f;
		}
	}

	GLfloat GetZoom()
	{
		return this->zoom;
	}

private:
	// Atributos de la camara
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Angulos de Euler
	GLfloat yaw;
	GLfloat pitch;

	// Opciones de camara
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	// Calcula el vector frontal a partir de los ángulos de Euler de la cámara (actualizados)
	void updateCameraVectors()
	{
		// Calcula el nuevo vector frontal
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		// También volver a calcular el vector de derecha y arriba
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalice los vectores, ya que su longitud se acerca a 0 cuanto más se mira hacia arriba o hacia abajo, lo que resulta en un movimiento más lento
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
};