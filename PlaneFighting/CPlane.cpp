#pragma once
#include<afxwin.h>
#include<sstream>
#include"CPlane.h"
#include"CShell.h"

CPlane::CPlane()
{
}

CPlane::CPlane(bool bIsEnemy, const char * ssFileName_M, const char * ssFileName_S, int nX, int nY, int nHP, int nLife, int nShootInterval)
{
    this->m_nShootInterval = nShootInterval;
    this->m_pBMP_M = new CBitmap;
    this->m_pBMP_M->m_hObject = LoadImage(NULL, ssFileName_M, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    this->m_pBMP_S = new CBitmap;
    this->m_pBMP_S->m_hObject = LoadImage(NULL, ssFileName_S, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    for (int shellNO = 0; shellNO < ENEMY_SHELL_MAX; shellNO++)
    {
        CShell shell(bIsEnemy, DAMAGE_NORMAL, nX, nY, 2);
        this->m_vecShell.push_back(shell);
    }
    this->initPlane(bIsEnemy, nX, nY, nHP);
}

void CPlane::initPlane(bool bIsEnemy, int nX, int nY, int nHP, int nLife)
{
    this->m_nX = nX;
    this->m_nY = nY;
    this->m_nHP = nHP;
    this->m_nLife = nLife;
    this->m_bExist = false;
    this->m_bIsEnemy = bIsEnemy;
    for (int shellNO = 0; shellNO < ENEMY_SHELL_MAX; shellNO++)
    {
        this->m_vecShell[shellNO].initShell(bIsEnemy, DAMAGE_NORMAL, nX, nY, 2);
    }
}

void CPlane::shoot()
{
    this->getBMPSize(this->m_pBMP_M, this->m_bmp.bmWidth, this->m_bmp.bmHeight);
    this->m_nShootInterval = ((SHOOT_INTERVAL_ENEMY == m_nShootInterval) ? SHOOT_INTERVAL_ENEMY : ++m_nShootInterval); // Éä»÷¼ä¸ôÎª4s
    if (SHOOT_INTERVAL_ENEMY == m_nShootInterval && m_bExist)
    {
        for (int shellNO = 0; shellNO < ENEMY_SHELL_MAX; shellNO++)
        {
            if (!this->m_vecShell[shellNO].m_bExist)
            {
                this->m_vecShell[shellNO].display();
                this->m_vecShell[shellNO].m_nX = this->m_nX + this->m_bmp.bmWidth / 3 - 1;
                this->m_vecShell[shellNO].m_nY = this->m_nY + this->m_bmp.bmHeight;
                this->m_nShootInterval = 0;
                break;
            }
        }
    }
}

void CPlane::move()
{
    if (this->m_bExist && this->m_bIsEnemy)
    {
        this->m_nY += ENEMY_STEP;
    }
}

void CPlane::getBMPSize(CBitmap * pbmp, int & width, int & height)
{
    BITMAP bmp;
    pbmp->GetBitmap(&bmp);
    width = bmp.bmWidth;
    height = bmp.bmHeight;
}

void CPlane::shellFly()
{
    for (int shellNO = 0; shellNO < m_vecShell.size(); shellNO++)
    {
        this->m_vecShell[shellNO].move();

        if (720 < this->m_vecShell[shellNO].m_nY)
        {
            this->m_vecShell[shellNO].disappear();
        }
    }
}

void CPlane::display()
{
    this->m_bExist = true;
}

void CPlane::disappear()
{
    this->m_bExist = false;
}

void CPlane::damaged(CShell shell)
{
    this->m_nHP -= shell.m_nDamage;
}

void CPlane::blowUp()
{
}
