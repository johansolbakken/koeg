#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(CameraProjection projection)
		: m_position(0.0f, 0.0f, 0.0f), m_forward(0.0f, 0.0f, -1.0f), m_right(1.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f),
		  m_width(1280), m_height(720), m_viewMatrix(1.0f), m_projectionMatrix(1.0f), m_projection(projection)
{
	update(0.0);
}

Camera::~Camera()
{
}

void Camera::update(double deltaTime)
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
	if (m_projection == CameraProjection::Perspective)
	{
		m_projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(m_width) / m_height, 0.1f,
				100.0f);
	}
	else
	{
		m_projectionMatrix = glm::ortho(-static_cast<float>(m_width) / 2.0f, static_cast<float>(m_width) / 2.0f,
				-static_cast<float>(m_height) / 2.0f, static_cast<float>(m_height) / 2.0f,
				-1.0f, 1.0f);
	}
}

void Camera::setPosition(const glm::vec3& position)
{
	m_position = position;
}

void Camera::setForward(const glm::vec3& forward)
{
	m_forward = forward;
}

void Camera::setRight(const glm::vec3& right)
{
	m_right = right;
}

void Camera::setUp(const glm::vec3& up)
{
	m_up = up;
}

const glm::vec3& Camera::position() const
{
	return m_position;
}

const glm::vec3& Camera::forward() const
{
	return m_forward;
}

const glm::vec3& Camera::right() const
{
	return m_right;
}

const glm::vec3& Camera::up() const
{
	return m_up;
}

const glm::mat4& Camera::viewMatrix() const
{
	return m_viewMatrix;
}

void Camera::onResize(uint32_t width, uint32_t height)
{
	m_width = width;
	m_height = height;

	update(0);
}

uint32_t Camera::width() const
{
	return m_width;
}

uint32_t Camera::height() const
{
	return m_height;
}

glm::mat4 Camera::projectionMatrix() const
{
	return m_projectionMatrix;
}

CameraProjection Camera::projection() const
{
	return m_projection;
}

void Camera::setProjection(CameraProjection projection)
{
	m_projection = projection;
}
