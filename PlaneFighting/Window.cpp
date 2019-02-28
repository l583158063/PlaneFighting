#pragma once
#include<afxwin.h>
#include<sstream>
#include"Input.h"
#include"Sound.h"
#include"CGameAI.h"
#include"CMyWnd.h"
#include"CMyApp.h"

CMyWnd::CMyWnd()
{
    int realWidth = 438;
    int realHeight = 720;
    int windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
    int windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;
    m_rect = CRect(windowLeft, windowTop, windowLeft + realWidth, windowTop + realHeight);
    AdjustWindowRect(&m_rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);

    Create(NULL, "雷霆战机", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, m_rect);

    CClientDC dc(this);

    m_pDC = new CDC;
    m_pDC->CreateCompatibleDC(&dc);

    m_pBuffer = new CDC;
    m_pBuffer->CreateCompatibleDC(&dc);

    m_pBMPBuffer = new CBitmap;
    m_pBMPBuffer->CreateCompatibleBitmap(&dc, m_rect.right, m_rect.bottom);

    m_pBuffer->SelectObject(m_pBMPBuffer);

    GetClientRect(m_rect);

    SetTimer(1, 25, NULL);
}

BOOL CMyApp::InitInstance()
{
    CMyWnd *pf = new CMyWnd;
    pf->ShowWindow(m_nCmdShow);
    pf->UpdateData();
    m_pMainWnd = pf;
    CGameAI::Get().setWnd(pf);
    if (!(DXInput::Get().Initialize(m_hInstance, pf->GetSafeHwnd())))
    {
        return FALSE;
    }
    if (!(Sound::Get().Initialize(pf->GetSafeHwnd())))
    {
        return FALSE;
    }
    Sound::Get().LoadWaveFile("Fire", "sound/Fire.wav");
    Sound::Get().LoadWaveFile("sc1", "sound/SC1-Terran 2.wav");
    Sound::Get().LoadWaveFile("sc2", "sound/SC2-Terran 2.wav");

    Sound::Get().PlayWaveFile("sc2", true);

    return TRUE;
}

CMyApp app;

BEGIN_MESSAGE_MAP(CMyWnd, CFrameWnd)
    ON_WM_TIMER()
END_MESSAGE_MAP()


void CMyWnd::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    DXInput::Get().Update();

    CGameAI::Get().run();

    drawBuffer();

    CFrameWnd::OnTimer(nIDEvent);
}

void CMyWnd::drawBuffer()
{
    CClientDC dc(this);

    //BITMAP bmp;
    //CGameAI::Get().m_menu.m_pBg->GetBitmap(&bmp);

    //m_pDC->SelectObject(CGameAI::Get().m_menu.m_pBg);

    m_pBuffer = CGameAI::Get().m_pBuffer;

    dc.BitBlt(0, 0, m_rect.right, m_rect.bottom, m_pBuffer, 0, 0, SRCCOPY);

}
