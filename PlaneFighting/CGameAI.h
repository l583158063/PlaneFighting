#pragma once
#include<afxwin.h>
#include<vector>
#include"CMyMenu.h"
#include"CPlane.h"
#include"CBg.h"
#include"CPlayer.h"
#include"CProperty.h"
#include"CMyWnd.h"
#include"CBoss.h"
#include"CBlowUp.h"

#define IN_MENU 0
#define IN_GAME 1
#define IN_WIN 2
#define IN_LOSS 3
#define IN_OPTION 4
#define IN_INTRO 5

#define GAME_CONTINUE 1
#define GAME_WIN 2
#define GAME_OVER 3

#define PLAYER_STEP 3
#define MAX_ENEMY 15
#define MAX_BOSS 1
#define BOSS_DISPLAY_TIME 12.0f

using namespace std;

class CGameAI
{
public:
    CMyWnd * m_pWnd;
    CMyMenu m_menu;
    CDC * m_pBuffer;
    CPlayer * m_pPlayer;
    vector<CPlane *> m_vecPlayer;
    vector<CPlane> m_vecEnemy;
    CBoss * m_pBoss;
    vector<CBlowUp> m_vecBlowUp;
    //vector<CProperty> m_vecProperty;
    CBg m_CBg;
    int m_nLevel;
    long m_lTime;
    float m_fGameTime;
    int m_nTimelag;
    int m_nProduceInterval;
    long m_lScorc;
    int m_nStatus;

public:
    static CGameAI& Get();
    void setWnd(CMyWnd *);
    void initGame();
    void run();

private:
    CGameAI();
    void updateData();
    void render();
    void processKeyDown();
    void showMenu();
    void showOption();
    void showIntro();
    void showWin();
    void showLoss();
    void showGame();
    void gameLogic();
    void produceEnemy();
    void enemyShoot();
    void collisionDetection();
    int gameOver();
    void setBlowUp(int nCentreX, int nCentreY);
};