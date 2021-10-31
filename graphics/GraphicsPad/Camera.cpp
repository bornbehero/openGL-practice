#include <Camera.h>
#include <glm\gtx\transform.hpp>

const float Camera::Movement_speed = 0.1f;
const float Camera::Rotation_speed = 0.5f;

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(Position, Position + ViewDir, UpDir);
}

void Camera::move_forward()
{
	Position += ViewDir * Movement_speed;
}

void Camera::move_backward()
{
	Position += -ViewDir * Movement_speed;
}

void Camera::move_leftward()
{
	Position += -TengentDir * Movement_speed;
}

void Camera::move_rightward()
{
	Position += TengentDir * Movement_speed;
}

void Camera::move_upward()
{
	Position += UpDir * Movement_speed;
}

void Camera::move_downward()
{
	Position += -UpDir *Movement_speed;
}

void Camera::rotate_left()
{
	glm::mat4 rotator = glm::rotate(Rotation_speed, UpDir);
	ViewDir = glm::mat3(rotator) * ViewDir;
	TengentDir = glm::cross(ViewDir, UpDir);
}

void Camera::rotate_right()
{
	glm::mat4 rotator = glm::rotate(-Rotation_speed, UpDir);
	ViewDir = glm::mat3(rotator) * ViewDir;
	TengentDir = glm::cross(ViewDir, UpDir);
}

void Camera::rotate_up()
{
	glm::mat4 rotator = glm::rotate(Rotation_speed, TengentDir);
	ViewDir = glm::mat3(rotator) * ViewDir;
}

void Camera::rotate_down()
{
	glm::mat4 rotator = glm::rotate(-Rotation_speed, TengentDir);
	ViewDir = glm::mat3(rotator) * ViewDir;
}