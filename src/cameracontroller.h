#pragma once

#include "camera.h"

#include <memory>

class CameraController
{
public:
	CameraController(const std::shared_ptr<Camera>& camera);

	void update(double deltaTime);

	void setSpeed(float speed);

	void setSensitivity(float sensitivity);

private:
	std::shared_ptr<Camera> m_camera;

	float m_speed;
	float m_sensitivity;

	float m_pitch;
	float m_yaw;
};