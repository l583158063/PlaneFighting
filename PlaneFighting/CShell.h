#pragma once
#include<afxwin.h>
#include<sstream>
#include<vector>

#define PLAYER_SHELL_STEP 6
#define ENEMY_SHELL_STEP 4
#define SHELL_COUNT 6

using namespace std;

class CShell
{
public:
    vector<CBitmap *> m_vecBMP_M;
    vector<CBitmap *> m_vecBMP_S;
    CBitmap * m_pBMP_M;
    CBitmap * m_pBMP_S;
    BITMAP m_bmp;
    int m_nX;
    int m_nY;
    bool m_bExist;
    bool m_bIsEnemy;
    int m_nDamage;
    int m_nWhichShell;

public:
    CShell();
    CShell(bool bIsEnemy, int nDamage, int nX, int nY, int nWhichShell);
    void initShell(bool bIsEnemy, int nDamage, int nX, int nY, int nWhichShell);
    void changeShell(int nWhichShell);
    void move();
    void display();
    void disappear();

};