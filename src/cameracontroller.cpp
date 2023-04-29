#include "cameracontroller.h"

#include "input.h"
#include "log.h"

#include <glm/gtc/matrix_transform.hpp>

CameraController::CameraController(const std::shared_ptr<Camera>& camera)
		: m_camera(camera), m_speed(5.0f), m_sensitivity(0.2f)
{
}

void CameraController::update(double deltaTime)
{
	if (m_camera == nullptr)
	{
		LOG_WARN("CameraController::update: camera is nullptr");
		return;
	}

	if (Input::isMouseButtonPressed(MouseButton::Left))
	{
		Input::setCursorMode(CursorMode::Disabled);

		if (Input::isKeyPressed(KeyCode::W))
		{
			m_camera->setPosition(m_camera->position() + m_camera->forward() * static_cast<float>(deltaTime) * m_speed);
		}
		if (Input::isKeyPressed(KeyCode::S))
		{
			m_camera->setPosition(m_camera->position() - m_camera->forward() * static_cast<float>(deltaTime) * m_speed);
		}
		if (Input::isKeyPressed(KeyCode::A))
		{
			m_camera->setPosition(m_camera->position() - m_camera->right() * static_cast<float>(deltaTime) * m_speed);
		}
		if (Input::isKeyPressed(KeyCode::D))
		{
			m_camera->setPosition(m_camera->position() + m_camera->right() * static_cast<float>(deltaTime) * m_speed);
		}
		if (Input::isKeyPressed(KeyCode::Space))
		{
			m_camera->setPosition(m_camera->position() + m_camera->up() * static_cast<float>(deltaTime) * m_speed);
		}
		if (Input::isKeyPressed(KeyCode::LeftShift))
		{
			m_camera->setPosition(m_camera->position() - m_camera->up() * static_cast<float>(deltaTime) * m_speed);
		}

		if (m_firstMouse)
		{
			Input::resetMouseDelta();
			m_firstMouse = false;
			return;
		}

		float xOffset = Input::mouseX() - Input::mouseXLast();
		float yOffset = Input::mouseYLast() - Input::mouseY();

		xOffset *= m_sensitivity;
		yOffset *= m_sensitivity;

		m_pitch += yOffset;
		m_yaw += xOffset;

		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}

		m_camera->setForward(glm::normalize(glm::vec3(
				cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
				sin(glm::radians(m_pitch)),
				sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
		)));
		m_camera->setRight(glm::normalize(glm::cross(m_camera->forward(), m_camera->up())));
	}
	else
	{
		Input::setCursorMode(CursorMode::Normal);
	}

	Input::resetMouseDelta();

	m_camera->update(deltaTime);
}

void CameraController::setSpeed(float speed)
{
	m_speed = speed;
}

void CameraController::setSensitivity(float sensitivity)
{
	m_sensitivity = sensitivity;
}