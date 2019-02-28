#pragma once
#include<afxwin.h>
#include<vector>
#include<sstream>
#include"CShell.h"

#define SHOOT_INTERVAL_ENEMY 160
#define SHOOT_INTERVAL_BOSS 10000
#define DAMAGE_NORMAL 10
#define DAMAGE_BOSS 20
#define ENEMY_SHELL_MAX 30
#define ENEMY_STEP 2
#define ENEMY_TYPE 3


using namespace std;

struct bitmap
{
    int bmWidth;
    int bmHeight;
};

class CPlane
{
public:
    CBitmap * m_pBMP_M;
    CBitmap * m_pBMP_S;
    bitmap m_bmp;
    int m_nX;
    int m_nY;
    bool m_bExist;
    bool m_bIsEnemy;
    vector<CShell> m_vecShell;
    int m_nHP;
    int m_nLife;
    int m_nShootInterval;

public:
    CPlane();
    CPlane(bool bIsEnemy, const char * ssFileName_M, const char * ssFileName_S, int nX, int nY, int nHP, int nLife = 0, int nShootInterval = SHOOT_INTERVAL_ENEMY);
    void initPlane(bool bIsEnemy, int nX, int nY, int nHP, int nLife = 0);
    virtual void shoot();
    virtual void move();
    void getBMPSize(CBitmap * pbmp, int &width, int &height);
    void shellFly();
    void display();
    void disappear();
    void damaged(CShell);
    void blowUp();
};