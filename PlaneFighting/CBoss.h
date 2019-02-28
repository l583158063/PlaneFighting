#pragma once
#include"CPlane.h"

#define BOSS_SHELL_TYPE 3

class CBoss : public CPlane
{
private:
    bool m_bIsGoLift = true; //+++
    int m_nBossInterval[3]; //+++

public:
    CBoss();
    CBoss(bool bIsEnemy, const char * ssFileName_M, const char * ssFileName_S, int nX, int nY, int nHP, int nLife = 0, int nShootInterval = SHOOT_INTERVAL_ENEMY);
    void shoot();   //
    void move();    //
    void shellFly();//
};