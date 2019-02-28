#include "CGameApp.h"
#include "TombRaiders.h"
#include "MainFrm.h"
#include "Input.h"
#include <sstream>
#include "resource.h"
#include "Sound.h"
CGameApp theApp;

BOOL CGameApp::InitInstance()
{
	CWinApp::InitInstance();
	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;

	int windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
	int windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;
	RECT rect{ windowLeft, windowTop, windowLeft + realWidth, windowTop + realHeight };
	AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);

	pFrame->Create(NULL, L"摸金校尉", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, rect);
	HICON icon = (HICON)LoadImage(m_hInstance, __TEXT("Soul.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

	pFrame->SetIcon(icon, true);
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	if (!(Sound::Get().Initialize(m_pMainWnd->GetSafeHwnd())))
	{
		return FALSE;
	}
	Sound::Get().LoadWaveFile("switch1", "../Assets/Sounds/switch1.wav");
	Sound::Get().LoadWaveFile("click1", "../Assets/Sounds/click1.wav");
	Sound::Get().LoadWaveFile("Dungeon", "../Assets/Sounds/dungeon.wav");
	Sound::Get().LoadWaveFile("Memoraphile", "../Assets/Sounds/Memoraphile - Spooky Dungeon.wav");
	Sound::Get().LoadWaveFile("hit1", "../Assets/Sounds/hit1.wav");
	Sound::Get().LoadWaveFile("hit2", "../Assets/Sounds/hit2.wav");
	Sound::Get().LoadWaveFile("hit3", "../Assets/Sounds/hit3.wav");
	Sound::Get().LoadWaveFile("Swordsmall", "../Assets/Sounds/Socapex - Swordsmall.wav");
	Sound::Get().LoadWaveFile("monster-1", "../Assets/Sounds/monster-1.wav");
	Sound::Get().LoadWaveFile("monster-2", "../Assets/Sounds/monster-2.wav");
	Sound::Get().LoadWaveFile("monster-3", "../Assets/Sounds/monster-3.wav");
	Sound::Get().LoadWaveFile("monster-4", "../Assets/Sounds/monster-4.wav");
	Sound::Get().LoadWaveFile("monster-5", "../Assets/Sounds/monster-5.wav");
	Sound::Get().LoadWaveFile("monster-6", "../Assets/Sounds/monster-6.wav");

	if (!DXInput::Get().Initialize(m_hInstance, m_pMainWnd->GetSafeHwnd()))
	{
		return FALSE;
	}
	return TRUE;
}

int CGameApp::Run()
{
	TombRaiders::Get().Initialize(this);
	MSG msg;
	m_Timer.Start();
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DXInput::Get().Update();
			TombRaiders::Get().Update();
			TombRaiders::Get().Render();
		}
		m_Timer.Tick();
		m_Fps.Frame();
		std::wstring str = L"摸金校尉 - FPS:" + std::to_wstring(m_Fps.GetFps());
		if (m_pMainWnd)
			m_pMainWnd->SetWindowText(str.c_str());
	}
	return 0;
}