#include "CShell.h"

CShell::CShell()
{

}

CShell::CShell(bool bIsEnemy, int nDamage, int nX, int nY, int nWhichShell)
{
    this->m_bExist = false;
    this->m_bIsEnemy = bIsEnemy;
    this->m_nDamage = nDamage;
    this->m_nX = nX;
    this->m_nY = nY;
    this->m_nWhichShell = nWhichShell;
    this->m_pBMP_M = new CBitmap;
    this->m_pBMP_S = new CBitmap;

    for (int shellNO = 0; shellNO < SHELL_COUNT; shellNO++)
    {
        CBitmap * pBMP = new CBitmap;
        std::stringstream ss;
        ss << "shell/Shell" << shellNO << "_M.bmp";
        pBMP->m_hObject = LoadImage(NULL, ss.str().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->m_vecBMP_M.push_back(pBMP);
    }
    for (int shellNO = 0; shellNO < SHELL_COUNT; shellNO++)
    {
        CBitmap * pBMP = new CBitmap;
        std::stringstream ss;
        ss << "shell/Shell" << shellNO << "_S.bmp";
        pBMP->m_hObject = LoadImage(NULL, ss.str().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->m_vecBMP_S.push_back(pBMP);
    }

    this->m_pBMP_M = this->m_vecBMP_M[m_nWhichShell];
    this->m_pBMP_S = this->m_vecBMP_S[m_nWhichShell];
    
}

void CShell::initShell(bool bIsEnemy, int nDamage, int nX, int nY, int nWhichShell)
{
    this->m_bExist = false;
    this->m_bIsEnemy = bIsEnemy;
    this->m_nDamage = nDamage;
    this->m_nX = nX;
    this->m_nY = nY;
    this->m_nWhichShell = nWhichShell;
}

void CShell::changeShell(int nWhichShell)
{
    this->m_nWhichShell = nWhichShell;
    this->m_pBMP_M = this->m_vecBMP_M[m_nWhichShell];
    this->m_pBMP_S = this->m_vecBMP_S[m_nWhichShell];
}

void CShell::move()
{
    if (m_bIsEnemy)
    {
        m_nY += ENEMY_SHELL_STEP;
    }
    if (!m_bIsEnemy)
    {
        m_nY -= PLAYER_SHELL_STEP;
    }
    
}

void CShell::display()
{
    this->m_bExist = true;
}

void CShell::disappear()
{
    this->m_bExist = false;
    this->changeShell(this->m_nWhichShell);
}
