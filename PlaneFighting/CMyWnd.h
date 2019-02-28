#pragma once
#include<afxwin.h>
#include<sstream>
#include"CGameAI.h"

class CMyWnd : public CFrameWnd
{
private:
    CDC * m_pDC;
    CBitmap * m_pBMPBuffer;

public:
    CDC * m_pBuffer;
    CRect m_rect;

public:
    CMyWnd();
    

    void drawBuffer();

    DECLARE_MESSAGE_MAP()
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};