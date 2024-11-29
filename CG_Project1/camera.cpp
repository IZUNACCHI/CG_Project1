#include "Camera.h"


Camera::Camera(glm::vec3 cameraPosition, glm::vec3 up, float cameraYaw, float cameraPitch)
	: front(glm::vec3(0.f, 0.f, -1.f)), movementSpeed(2.5f), mouseSensitivity(0.1f), fov(45.f), hfront(glm::vec3(0.f, 0.f, -1.f))
{
	position = cameraPosition;
	worldUp = up;
	yaw = cameraYaw;
	pitch = cameraPitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboardInput(const Uint8* keyState, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	if (keyState[SDL_SCANCODE_W]) move(FORWARD, velocity);
	if (keyState[SDL_SCANCODE_S]) move(BACKWARD, velocity);
	if (keyState[SDL_SCANCODE_A]) move(LEFT, velocity);
	if (keyState[SDL_SCANCODE_D]) move(RIGHT, velocity);
	if (keyState[SDL_SCANCODE_Q]) move(DOWN, velocity);
	if (keyState[SDL_SCANCODE_E]) move(UP, velocity);
}

void Camera::processMouseInput(SDL_Event ev, float deltaTime) {

	float xoffset = ev.motion.xrel;
	float yoffset = -ev.motion.yrel; // reversed since y-coordinates range from bottom to top
	
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	yaw += xoffset;
	pitch += yoffset;
	
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateCameraVectors();
}


void Camera::processMouseScroll(float yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f) fov -= yoffset;
	if (fov <= 1.0f) fov = 1.0f;
	if (fov >= 45.0f) fov = 45.0f;
}

void Camera::updateCameraVectors()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);

	//project front along the horizontal plane
	hfront = glm::normalize(front * glm::vec3(1.0f, 0.0f, 1.0f));

	right = glm::normalize(glm::cross(hfront, worldUp));
	up = glm::normalize(glm::cross(right, hfront));
}

void Camera::move(CameraMovement direction, float velocity)
{
	if (direction == FORWARD) position += hfront* velocity;
	if (direction == BACKWARD) position -= hfront * velocity;
	if (direction == LEFT) position -= right * velocity;
	if (direction == RIGHT) position += right * velocity;
	if (direction == UP) position += up * velocity;
	if (direction == DOWN) position -= up * velocity;
}