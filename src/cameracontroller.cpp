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

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/quaternion.hpp>

namespace Hazel
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
			: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnUpdate(double ts)
	{
		if (Input::isKeyPressed(KeyCode::LeftAlt))
		{
			const glm::vec2& mouse{ Input::mouseX(), Input::mouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::isMouseButtonPressed(MouseButton::Middle))
				MousePan(delta);
			else if (Input::isMouseButtonPressed(MouseButton::Left))
				MouseRotate(delta);
			else if (Input::isMouseButtonPressed(MouseButton::Right))
				MouseZoom(delta.y);
		}

		UpdateView();
	}


	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
}