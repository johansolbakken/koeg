#pragma once

#include <glm/glm.hpp>

enum class KeyCode
{
	Unknown = -1,
	Space = 32,
	Apostrophe = 39, /* ' */
	Comma = 44, /* , */
	Minus = 45, /* - */
	Period = 46, /* . */
	Slash = 47, /* / */

	Num0 = 48, /* 0 */
	Num1 = 49, /* 1 */
	Num2 = 50, /* 2 */
	Num3 = 51, /* 3 */
	Num4 = 52, /* 4 */
	Num5 = 53, /* 5 */
	Num6 = 54, /* 6 */
	Num7 = 55, /* 7 */
	Num8 = 56, /* 8 */
	Num9 = 57, /* 9 */

	Semicolon = 59, /* ; */
	Equal = 61, /* = */

	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,

	LeftBracket = 91, /* [ */
	Backslash = 92, /* \ */
	RightBracket = 93, /* ] */
	GraveAccent = 96, /* ` */

	World1 = 161, /* non-US #1 */
	World2 = 162, /* non-US #2 */

	/* Function keys */
	Escape = 256,
	Enter = 257,
	Tab = 258,
	Backspace = 259,
	Insert = 260,
	Delete = 261,
	Right = 262,
	Left = 263,
	Down = 264,
	Up = 265,
	PageUp = 266,
	PageDown = 267,
	Home = 268,
	End = 269,
	CapsLock = 280,
	ScrollLock = 281,
	NumLock = 282,
	PrintScreen = 283,

	LeftShift = 340,
};

enum class MouseButton
{
	Left = 0,
	Right = 1,
	Middle = 2
};

enum class CursorMode
{
	Normal = 0,
	Hidden = 1,
	Disabled = 2
};

class Input
{
public:
	static bool isKeyPressed(KeyCode key);

	static bool isMouseButtonPressed(MouseButton button);

	static float mouseX();

	static float mouseY();

	static float mouseDeltaX();

	static float mouseDeltaY();

	static float mouseScrollDeltaX();

	static float mouseScrollDeltaY();

	static void setMousePosition(float x, float y);

	static void setMousePositionDelta(float x, float y);

	static void setMouseScrollDelta(float x, float y);

	static void setKeyPressed(KeyCode key, bool pressed);

	static void setMouseButtonPressed(MouseButton button, bool pressed);

	static void setMousePositionCallback(void(* mousePositionCallback)(float, float));

	static void setMousePositionDeltaCallback(void(* mousePositionDeltaCallback)(float, float));

	static void setMouseScrollDeltaCallback(void(* mouseScrollDeltaCallback)(float, float));

	static double mouseXLast();

	static double mouseYLast();

	static glm::vec2 mouseDelta();

	static void resetMouseDelta();

	static void setCursorMode(CursorMode mode);
};