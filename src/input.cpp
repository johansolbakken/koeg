#include "input.h"
#include "window.h"

bool keys[1024] = { false };
bool mouseButtons[32] = { false };
float mouseXPos = 0.0f;
float mouseYPos = 0.0f;
float mouseXDelta = 0.0f;
float mouseYDelta = 0.0f;
float mouseScrollXDelta = 0.0f;
float mouseScrollYDelta = 0.0f;

Window* inputWindow = nullptr;

bool Input::isKeyPressed(KeyCode key)
{
	return keys[(int)key];
}

bool Input::isMouseButtonPressed(MouseButton button)
{
	return mouseButtons[(int)button];
}

float Input::mouseX()
{
	return mouseXPos;
}

float Input::mouseY()
{
	return mouseYPos;
}

float Input::mouseDeltaX()
{
	return mouseXDelta;
}

float Input::mouseDeltaY()
{
	return mouseYDelta;
}

float Input::mouseScrollDeltaX()
{
	return mouseScrollXDelta;
}

float Input::mouseScrollDeltaY()
{
	return mouseScrollYDelta;
}

void Input::setMousePosition(float x, float y)
{
	mouseXDelta = x - mouseXPos;
	mouseYDelta = y - mouseYPos;

	mouseXPos = x;
	mouseYPos = y;
}

void Input::setMousePositionDelta(float x, float y)
{
	mouseXDelta = x;
	mouseYDelta = y;
}

void Input::setMouseScrollDelta(float x, float y)
{
	mouseScrollXDelta = x;
	mouseScrollYDelta = y;
}

void Input::setKeyPressed(KeyCode key, bool pressed)
{
	keys[(int)key] = pressed;
}

void Input::setMouseButtonPressed(MouseButton button, bool pressed)
{
	mouseButtons[(int)button] = pressed;
}

glm::vec2 Input::mouseDelta()
{
	return glm::vec2(mouseXDelta, mouseYDelta);
}

void Input::resetMouseDelta()
{
	mouseXDelta = 0.0f;
	mouseYDelta = 0.0f;
}

double Input::mouseXLast()
{
	return mouseXPos - mouseXDelta;
}

double Input::mouseYLast()
{
	return mouseYPos - mouseYDelta;
}

void Input::setCursorMode(CursorMode mode)
{
	constexpr auto cursorModeBase = 0x00034001;
	inputWindow->setCursorMode(cursorModeBase + (int)mode);
}