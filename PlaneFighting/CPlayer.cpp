#pragma once
#include"CGameAI.h"
#include"CPlayer.h"
#include"Input.h"
#include"Sound.h"


CPlayer::CPlayer()
{
    for (int playerNO = 0; playerNO < PLAYER_COUNT; playerNO++)
    {
        for (int postureNO = 0; postureNO < POSTURE_COUNT; postureNO++)
        {
            std::stringstream ss;
            ss << "player/player1_M_" << postureNO << ".bmp";
            CBitmap *bmp = new CBitmap;
            bmp->m_hObject = LoadImage(NULL, ss.str().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            this->m_vecBMPPlayerM.push_back(bmp);
        }
        for (int postureNO = 0; postureNO < POSTURE_COUNT; postureNO++)
        {
            std::stringstream ss;
            ss << "player/player1_S_" << postureNO << ".bmp";
            CBitmap *bmp = new CBitmap;
            bmp->m_hObject = LoadImage(NULL, ss.str().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            this->m_vecBMPPlayerS.push_back(bmp);
        }
    }
    for (int shellNO = 0; shellNO < PLAYER_SHELL_MAX; shellNO++)
    {
        CShell shell(false, 10, m_nX + (m_bmp.bmWidth / 3 + 3), m_nY - this->m_bmp.bmHeight, 0);
        this->m_vecShell.push_back(shell);
    }
    this->initPlane();
}

void CPlayer::initPlane()
{
    this->m_nPosture = 2;
    this->getBMPSize(m_vecBMPPlayerM[this->m_nPosture], m_bmp.bmWidth, m_bmp.bmHeight);
    this->m_bExist = true;
    //this->m_nX = (CGameAI::Get().m_pWnd->m_rect.right - m_bmp.bmWidth) / 2;
    //this->m_nY = CGameAI::Get().m_pWnd->m_rect.bottom - m_bmp.bmHeight;
    this->m_nX = 180;
    this->m_nY = 630;
    this->m_nHP = PLAYER_HP;
    this->m_nLife = PLAYER_LIFE;
    this->m_nShootInterval = 20;
    //this->m_nCountSuperWeapon = 1;
    for (int shellNO = 0; shellNO < PLAYER_SHELL_MAX; shellNO++)
    {
        this->m_vecShell[shellNO].initShell(false, 10, m_nX + (m_bmp.bmWidth / 3 + 3), m_nY - this->m_bmp.bmHeight, 0);
    }
}

void CPlayer::shoot()
{
    this->m_nShootInterval = ((SHOOT_INTERVAL_PLAYER == m_nShootInterval) ? SHOOT_INTERVAL_PLAYER : ++m_nShootInterval); // Éä»÷¼ä¸ôÎª0.5s
    if (SHOOT_INTERVAL_PLAYER == m_nShootInterval)
    {
        for (int shellNO = 0; shellNO < PLAYER_SHELL_MAX; shellNO++)
        {
            if (!this->m_vecShell[shellNO].m_bExist)
            {
                this->m_vecShell[shellNO].display();
                this->m_vecShell[shellNO].m_nX = this->m_nX + this->m_bmp.bmWidth / 3 + 3;
                this->m_vecShell[shellNO].m_nY = this->m_nY - this->m_vecShell[shellNO].m_bmp.bmHeight;
                this->m_nShootInterval = 0;

                Sound::Get().PlayWaveFile("Fire");

                break;
            }
        }
    }
}

void CPlayer::move()
{
    this->getBMPSize(m_vecBMPPlayerM[this->m_nPosture], m_bmp.bmWidth, m_bmp.bmHeight);
    
}

void CPlayer::shellFly()
{
    for (int shellNO = 0; shellNO < m_vecShell.size(); shellNO++)
    {
        this->m_vecShell[shellNO].move();

        if (0 > this->m_vecShell[shellNO].m_nY)
        {
            this->m_vecShell[shellNO].disappear();
        }
        
    }
}
//
//void CPlayer::addProp(CProperty prop)
//{
//    this->m_vecProp.push_back(prop);
//}
//
//void CPlayer::useProp()
//{
//    /*if (this->m_vecProp.size > 0)
//    {
//        m_vecProp[m_vecProp.size - 1].active();
//        m_vecProp.pop_back();
//    }*/
//}
//
//void CPlayer::useSuperWeapon()
//{
//    if (this->m_nCountSuperWeapon > 0)
//    {
//
//        this->m_nCountSuperWeapon--;
//    }
//}
