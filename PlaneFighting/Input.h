#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class DXInput
{
public:
	static DXInput& Get();
	bool Initialize(HINSTANCE, HWND);
	void Shutdown();
	bool Update();

	LPPOINT GetMouseLocation();
	bool IsPressed(int);
	bool IsLastPressed(int);
	bool IsButtonDown(int);
	bool IsLastButtonDown(int);
	DIMOUSESTATE GetMouseState() { return m_mouseState; }
	DIMOUSESTATE GetLastMouseState() { return m_mouseLastState; }
private:
	DXInput();
	bool ReadKeyboard();
	bool ReadMouse();
private:
	IDirectInput8 * m_directInput;

	IDirectInputDevice8 * m_keyboard;
	IDirectInputDevice8 * m_mouse;

	BYTE m_keyboardState[256];
	BYTE m_lastKeyboardState[256];
	DIMOUSESTATE m_mouseState;
	DIMOUSESTATE m_mouseLastState;
	HWND m_hwnd;
};