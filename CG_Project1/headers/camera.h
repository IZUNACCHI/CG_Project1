#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	Camera(glm::vec3 cameraPosition, glm::vec3 up, float yaw, float pitch);

	glm::mat4 getViewMatrix() const;

	void processKeyboardInput(const Uint8* keyState, float deltaTime);
	void processMouseInput(SDL_Event ev, float deltaTime);
	void processMouseScroll(float yoffset);
	float getFov() const { return fov; };
	inline glm::vec3 GetPosition() { return position; };

private:
	void updateCameraVectors();
	void move(CameraMovement direction, float velocity);

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	glm::vec3 hfront;

	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float fov;

	float lastX, lastY;
	bool firstMouse;
};
