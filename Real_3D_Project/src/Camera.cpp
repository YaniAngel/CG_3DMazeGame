#include "camera.h"


/// <summary>
///		Constructor
/// </summary>
/// <param name="position">Initial camera position</param>
/// <param name="up">Up-vector</param>
/// <param name="yaw">Initial yaw</param>
/// <param name="pitch">Initial pitch</param>
/// <returns>Obj</returns>
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == Camera_Movement::FORWARD)
		Position += glm::vec3(Front.x, 0.0, Front.z) * velocity;
	if (direction == Camera_Movement::BACKWARD)
		Position -= glm::vec3(Front.x, 0.0, Front.z) * velocity;
	if (direction == Camera_Movement::LEFT)
		Position -= glm::vec3(Right.x, 0.0, Right.z) * velocity;
	if (direction == Camera_Movement::RIGHT)
		Position += glm::vec3(Right.x, 0.0, Right.z) * velocity;
	if (direction == Camera_Movement::UP)
		Position += glm::vec3(0.0, 1.0, 0.0) * velocity;
	if (direction == Camera_Movement::DOWN)
		Position -= glm::vec3(0.0, 1.0, 0.0) * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 40.0f)
		Zoom = 40.0f;
	if (Zoom > 60.0f)
		Zoom = 60.0f;
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	// Re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}