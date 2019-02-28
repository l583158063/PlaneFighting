#include "Input.h"
#define BUTTONDOWN(name, key) ( name.rgbButtons[key] & 0x80 )
#define KEYDOWN( name, key ) ( name[key] & 0x80 )

DXInput::DXInput()
{
	m_directInput = NULL;
	m_keyboard = NULL;
	m_mouse = NULL;
}

DXInput & DXInput::Get()
{
	static DXInput instance;
	return instance;
}

bool DXInput::Initialize(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;
	m_hwnd = hWnd;
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr))
	{
		return false;
	}
	hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(hr))
	{
		return false;
	}
	hr = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(hr))
	{
		return false;
	}
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}
	hr = m_keyboard->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
	{
		return false;
	}
	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}
	hr = m_mouse->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

void DXInput::Shutdown()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool DXInput::Update()
{
	m_keyboard->Acquire();
	m_mouse->Acquire();
	ReadKeyboard();
	ReadMouse();
	return true;
}

LPPOINT DXInput::GetMouseLocation()
{
	LPPOINT point = new POINT();
	GetCursorPos(point);
	LPRECT rect = new RECT();
	GetWindowRect(m_hwnd, rect);
	point->x -= rect->left;
	point->y -= rect->top;
	return point;
}

bool DXInput::ReadKeyboard()
{
	memcpy(m_lastKeyboardState, m_keyboardState, sizeof(m_keyboardState));
	m_keyboard->GetDeviceState(sizeof(m_keyboardState), &m_keyboardState);
	return true;
}

bool DXInput::ReadMouse()
{
	m_mouseLastState = m_mouseState;
	m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
	return true;
}

bool DXInput::IsPressed(int num)
{
	if (KEYDOWN(m_keyboardState, num))
	{
		return true;
	}
	return false;
}

bool DXInput::IsLastPressed(int num)
{
	if (KEYDOWN(m_lastKeyboardState, num))
	{
		return true;
	}
	return false;
}

bool DXInput::IsButtonDown(int mouse)
{
	if (BUTTONDOWN(m_mouseState, mouse))
	{
		return true;
	}
	return false;
}

bool DXInput::IsLastButtonDown(int mouse)
{
	if (BUTTONDOWN(m_mouseLastState, mouse))
	{
		return true;
	}
	return false;
}