#include "CBlowUp.h"

CBlowUp::CBlowUp()
{
    m_nX = 0;
    m_nY = 0;
    m_bExist = true;
    m_pBMP_M = new CBitmap;
    m_pBMP_S = new CBitmap;
    m_pBMP_M->m_hObject = LoadImage(NULL, "effect/boom_M.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_pBMP_S->m_hObject = LoadImage(NULL, "effect/boom_S.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_nDuration = 0;
}

void CBlowUp::setDir(int nCentreX, int nCentreY)
{
    BITMAP bmp;
    m_pBMP_M->GetBitmap(&bmp);
    /*m_nX = nCentreX + bmp.bmWidth;
    m_nY = nCentreY + bmp.bmHeight;*/
    m_nX = nCentreX;
    m_nY = nCentreY;
}

bool CBlowUp::isFinish()
{
    if (m_nDuration <= 20 && m_bExist)
    {
        display();
        m_nDuration++;
        return false;
    }
    else if (m_nDuration > 20 && m_bExist)
    {
        disappear();
        m_nDuration = 0;
        return true;
    }
    return true;
}

void CBlowUp::display()
{
    m_bExist = true;
}

void CBlowUp::disappear()
{
    m_bExist = false;
}
