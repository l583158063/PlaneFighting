#pragma once
#include<afxwin.h>

class CBlowUp
{
public:
    int m_nX;
    int m_nY;
    bool m_bExist;
    CBitmap * m_pBMP_M;
    CBitmap * m_pBMP_S;
    int m_nDuration;

public:
    CBlowUp();
    void setDir(int nCentreX, int nCentreY);
    bool isFinish();
    void display();
    void disappear();
};