#pragma once

#include <glm/glm.hpp>

enum class CameraProjection
{
	Perspective,
	Orthographic
};

class Camera
{
public:
	Camera(CameraProjection projection = CameraProjection::Perspective);

	~Camera();

	void update(double deltaTime);

	void setPosition(const glm::vec3& position);

	void setForward(const glm::vec3& forward);

	void setRight(const glm::vec3& right);

	void setUp(const glm::vec3& up);

	[[nodiscard]] const glm::vec3& position() const;

	[[nodiscard]] const glm::vec3& forward() const;

	[[nodiscard]] const glm::vec3& right() const;

	[[nodiscard]] const glm::vec3& up() const;

	[[nodiscard]] const glm::mat4& viewMatrix() const;

	void onResize(uint32_t width, uint32_t height);

	[[nodiscard]] uint32_t width() const;

	[[nodiscard]] uint32_t height() const;

	[[nodiscard]] glm::mat4 projectionMatrix() const;

	[[nodiscard]] CameraProjection projection() const;

	void setProjection(CameraProjection projection);

	void setPositionWithLookAt(const glm::vec3& position, const glm::vec3& lookAt);

private:
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_up;

	uint32_t m_width;
	uint32_t m_height;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	CameraProjection m_projection;
};