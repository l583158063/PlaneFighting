#pragma once
#include<afxwin.h>
#include<mmsystem.h>
#include<random>
#include"CGameAI.h"
#include"CPlayer.h"
#include"CShell.h"
#include"CBg.h"
#include"Input.h"
#include"Sound.h"
#include"Math.h"
#include <afxwin1.inl>

CGameAI& CGameAI::Get()
{
    static CGameAI Instance;
    return Instance;
}

void CGameAI::setWnd(CMyWnd * wnd)
{
    this->m_pWnd = wnd;
    this->m_pBuffer = wnd->m_pBuffer;
}

CGameAI::CGameAI()
{
    this->m_nStatus = IN_MENU;
    this->m_pBuffer = new CDC;
    this->m_pWnd = new CMyWnd;

    CPlane * pPlayer = new CPlayer(); // 用子类对象初始化父类指针
    this->m_vecPlayer.push_back(pPlayer); // vector<CPlane *>
    this->m_pPlayer = new CPlayer;
    this->m_pPlayer = (CPlayer *)(this->m_vecPlayer[0]);

    // 初始化敌机
    std::mt19937 rng = std::mt19937(std::random_device()());
    std::uniform_int_distribution<int> randomX{ 30, 365 };

    this->m_vecEnemy.clear();
    for (int nEnemy = 0; nEnemy < MAX_ENEMY; nEnemy++)
    {
        int nEnemyNO = nEnemy % ENEMY_TYPE;
        std::stringstream ssM, ssS;
        ssM << "enemys/enemy" << nEnemyNO << "_M.bmp";
        ssS << "enemys/enemy" << nEnemyNO << "_S.bmp";
        CPlane enemy(true, ssM.str().c_str(), ssS.str().c_str(), randomX(rng), -65, 10);
        this->m_vecEnemy.push_back(enemy);
    }

    // 初始化BOSS
    std::stringstream ssM, ssS;
    int nBoss = 0;
    ssM << "enemys/boss" << nBoss << "_M.bmp";
    ssS << "enemys/boss" << nBoss << "_S.bmp";
    CPlane * boss = new CBoss(true, ssM.str().c_str(), ssS.str().c_str(), (this->m_pWnd->m_rect.right - 120) / 2, -95, 100, 0, 0);
    this->m_pBoss = (CBoss *)boss;



    //this->initGame();
}

void CGameAI::initGame()
{
    this->m_nProduceInterval = 0;
    this->m_nLevel = 0;
    this->m_lTime = timeGetTime();
    this->m_nTimelag = 0;
    this->m_lScorc = 0;
    this->m_fGameTime = 0.0;
    
    std::mt19937 rng = std::mt19937(std::random_device()());
    std::uniform_int_distribution<int> randomX{ 30, 365 };
    for (int nEnemy = 0; nEnemy < m_vecEnemy.size(); nEnemy++)
    {
        this->m_vecEnemy[nEnemy].initPlane(true, randomX(rng), -65, 10);
    }

    m_pBoss->initPlane(true, (this->m_pWnd->m_rect.right - 120) / 2, -95, 100);
    
    Sound::Get().Stop("sc2");
    Sound::Get().PlayWaveFile("sc1", true);

}

void CGameAI::run()
{
    updateData();
    render();
}

void CGameAI::updateData()
{

    if (IN_GAME == m_nStatus)
    {
        this->m_fGameTime = (timeGetTime() - this->m_lTime) / 1000.0f; // 更新游戏时间
        this->m_nProduceInterval++;
        if (this->m_nProduceInterval > 200)
        {
            this->m_nProduceInterval = 200;
        }

        // 主机偏移
        m_pPlayer->move();

        // 主机炮弹飞行
        m_pPlayer->shellFly();

        // 敌机移动
        for (int nEnemy = 0; nEnemy < m_vecEnemy.size(); nEnemy++)
        {
            m_vecEnemy[nEnemy].move();
        }
        // 敌机炮弹飞行
        for (int nEnemy = 0; nEnemy < m_vecEnemy.size(); nEnemy++)
        {
            m_vecEnemy[nEnemy].shellFly();
        }
        // BOSS移动
        m_pBoss->move();

        // BOSS炮弹飞行
        m_pBoss->shellFly();
        
    }

    processKeyDown();
}

void CGameAI::render()
{
    switch (this->m_nStatus)
    {
    case IN_MENU:
        showMenu();
        break;
    case IN_GAME:
        showGame();
        gameLogic();
        break;
    case IN_WIN:
        showWin();
        break;
    case IN_LOSS:
        showLoss();
        break;
    case IN_OPTION:
        showOption();
        break;
    case IN_INTRO:
        showIntro();
        break;
    default:
        break;
    }
}

void CGameAI::processKeyDown()
{
    switch (this->m_nStatus)
    {
    case IN_MENU:
        if (DXInput::Get().IsPressed(DIK_W) && !DXInput::Get().IsLastPressed(DIK_W))
        {
            this->m_menu.m_nOption--;
            this->m_menu.m_nOption = ((0 > this->m_menu.m_nOption) ? 0 : this->m_menu.m_nOption);
        }
        if (DXInput::Get().IsPressed(DIK_S) && !DXInput::Get().IsLastPressed(DIK_S))
        {
            this->m_menu.m_nOption++;
            this->m_menu.m_nOption = ((3 < this->m_menu.m_nOption) ? 3 : this->m_menu.m_nOption);
        }
        if (DXInput::Get().IsPressed(DIK_RETURN) && !DXInput::Get().IsLastPressed(DIK_RETURN))
        {
            switch (this->m_menu.m_nOption)
            {
            case OP_GAME_START:
                this->m_nStatus = IN_GAME;

                this->initGame();
                this->m_pPlayer->initPlane();

                break;
            case OP_GAME_OPTION:
                this->m_nStatus = IN_OPTION;
                break;
            case OP_GAME_INTRO:
                this->m_nStatus = IN_INTRO;
                break;
            case OP_EXIT:
                PostQuitMessage(0);
                break;
            default:
                break;
            }
        }
        
        break;
    case IN_GAME:
        // 主机移动
        if (DXInput::Get().IsPressed(DIK_W))
        {
            m_pPlayer->m_nY -= PLAYER_STEP;
        }
        if (DXInput::Get().IsPressed(DIK_A))
        {
            m_pPlayer->m_nX -= PLAYER_STEP;
            m_pPlayer->m_nPosture = 3;
        }
        if (DXInput::Get().IsPressed(DIK_S))
        {
            m_pPlayer->m_nY += PLAYER_STEP;
        }
        if (DXInput::Get().IsPressed(DIK_D))
        {
            m_pPlayer->m_nX += PLAYER_STEP;
            m_pPlayer->m_nPosture = 1;
        }
        /*if (DXInput::Get().IsPressed(DIK_D) && DXInput::Get().IsPressed(DIK_A))
        {
            m_pPlayer->m_nPosture = 2;
        }*/
        // 侧偏回归
        if ((DXInput::Get().IsLastPressed(DIK_A) && !DXInput::Get().IsPressed(DIK_A)) ||
            (DXInput::Get().IsLastPressed(DIK_D) && !DXInput::Get().IsPressed(DIK_D)) ||
            (DXInput::Get().IsPressed(DIK_D) && DXInput::Get().IsPressed(DIK_A)))
        {
            m_pPlayer->m_nPosture = 2;
        }

        // 主机射击
        if (DXInput::Get().IsPressed(DIK_J))
        {
            m_pPlayer->shoot();
        }

        //// 主机超武
        //if (DXInput::Get().IsPressed(DIK_K))
        //{
        //    m_pPlayer->useSuperWeapon();
        //}

        //// 主机道具
        //if (DXInput::Get().IsPressed(DIK_L))
        //{
        //    m_pPlayer->useProp();
        //}

        // 返回主菜单
        if (!DXInput::Get().IsPressed(DIK_ESCAPE) && DXInput::Get().IsLastPressed(DIK_ESCAPE))
        {
            Sound::Get().Stop("sc1");
            Sound::Get().PlayWaveFile("sc2", true);
            this->m_nStatus = IN_MENU;
        }

        break;
    case IN_WIN:
    case IN_LOSS:
        if (!DXInput::Get().IsPressed(DIK_RETURN) && DXInput::Get().IsLastPressed(DIK_RETURN))
        {
            this->m_nStatus = IN_MENU;
        }
        break;
    case IN_OPTION:
    case IN_INTRO:
        if (!DXInput::Get().IsPressed(DIK_ESCAPE) && DXInput::Get().IsLastPressed(DIK_ESCAPE))
        {
            this->m_nStatus = IN_MENU;
        }
        break;
    default:
        break;
    }
    
}

void CGameAI::showMenu()
{
    CClientDC dc(m_pWnd);
    CDC * mDC = new CDC;
    mDC->CreateCompatibleDC(&dc);

    BITMAP bmp;
    this->m_menu.m_vecOption[0]->GetBitmap(&bmp);

    CBitmap * old = mDC->SelectObject(this->m_menu.m_vecOption[this->m_menu.m_nOption]);

    m_pBuffer->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCCOPY);
    
    mDC->SelectObject(old);

    //old->DeleteObject();
    mDC->DeleteDC();
    
}

void CGameAI::showOption()
{
    CClientDC dc(m_pWnd);
    CDC * mDC = new CDC;
    mDC->CreateCompatibleDC(&dc);

    CBitmap pbmp;
    pbmp.m_hObject = LoadImage(NULL, "menu/option.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    BITMAP bmp;
    pbmp.GetBitmap(&bmp);

    CBitmap * old = mDC->SelectObject(&pbmp);

    m_pBuffer->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCCOPY);

    mDC->SelectObject(old);

    //pbmp->DeleteObject();
    //old->DeleteObject();
    mDC->DeleteDC();
}

void CGameAI::showIntro()
{
    CClientDC dc(m_pWnd);
    CDC * mDC = new CDC;
    mDC->CreateCompatibleDC(&dc);

    CBitmap pbmp;
    pbmp.m_hObject = LoadImage(NULL, "menu/intro.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    BITMAP bmp;
    pbmp.GetBitmap(&bmp);

    CBitmap * old = mDC->SelectObject(&pbmp);

    m_pBuffer->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCCOPY);

    mDC->SelectObject(old);

    //pbmp->DeleteObject();
    //old->DeleteObject();
    mDC->DeleteDC();
}

void CGameAI::showWin()
{
    CClientDC dc(m_pWnd);
    CDC * mDC = new CDC;
    mDC->CreateCompatibleDC(&dc);

    CBitmap pbmp;
    pbmp.m_hObject = LoadImage(NULL, "menu/win.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    BITMAP bmp;
    pbmp.GetBitmap(&bmp);

    CBitmap * old = mDC->SelectObject(&pbmp);

    m_pBuffer->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCCOPY);

    string sScore = "最终得分：" + to_string(this->m_lScorc);
    m_pBuffer->TextOutA(150, 400, sScore.c_str());

    mDC->SelectObject(old);

    //pbmp->DeleteObject();
    //old->DeleteObject();
    mDC->DeleteDC();
}

void CGameAI::showLoss()
{
    CClientDC dc(m_pWnd);
    CDC * mDC = new CDC;
    mDC->CreateCompatibleDC(&dc);

    CBitmap pbmp;
    pbmp.m_hObject = LoadImage(NULL, "menu/loss.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    BITMAP bmp;
    pbmp.GetBitmap(&bmp);

    CBitmap * old = mDC->SelectObject(&pbmp);

    m_pBuffer->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCCOPY);

    string sScore = "最终得分：" + to_string(this->m_lScorc);
    m_pBuffer->TextOutA(150, 400, sScore.c_str());

    mDC->SelectObject(old);

    //pbmp->DeleteObject();
    //old->DeleteObject();
    mDC->DeleteDC();
}

void CGameAI::showGame()
{
    CClientDC dc(m_pWnd);
    CDC * mDC = new CDC;
    mDC->CreateCompatibleDC(&dc);

    BITMAP bmp;
    CBitmap * old;

    //old = new CBitmap;
    //old->m_hObject = LoadImage(NULL, "maps/bg0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //mDC->SelectObject(old);
    //m_pBuffer->BitBlt(0, 0, 438, 720, mDC, 0, 0, SRCCOPY);
    
    
    // 贴背景
    old = mDC->SelectObject(this->m_CBg.m_vecBMP[this->m_CBg.m_nSeq]); // 选择背景位图并保存存根
    this->m_CBg.m_vecBMP[this->m_CBg.m_nSeq]->GetBitmap(&bmp); // 获取位图信息
    m_pBuffer->BitBlt(this->m_CBg.m_nX, this->m_CBg.m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCCOPY);
    m_pBuffer->BitBlt(this->m_CBg.m_nX, 0, bmp.bmWidth, this->m_CBg.m_nY, mDC, 0, bmp.bmHeight - this->m_CBg.m_nY, SRCCOPY); // 用背景滚下去的部分补充到屏幕上方
    //mDC->SelectObject(old); // 还原画刷
    this->m_CBg.roll();

    // 贴主机
    if (m_pPlayer->m_bExist)
    {
        mDC->SelectObject(m_pPlayer->m_vecBMPPlayerM[m_pPlayer->m_nPosture]);
        m_pPlayer->m_vecBMPPlayerM[m_pPlayer->m_nPosture]->GetBitmap(&bmp);
        m_pBuffer->BitBlt(m_pPlayer->m_nX, m_pPlayer->m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCAND);
        //mDC->SelectObject(old);
        mDC->SelectObject(m_pPlayer->m_vecBMPPlayerS[m_pPlayer->m_nPosture]);
        m_pPlayer->m_vecBMPPlayerS[m_pPlayer->m_nPosture]->GetBitmap(&bmp);
        m_pBuffer->BitBlt(m_pPlayer->m_nX, m_pPlayer->m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCPAINT);
        //mDC->SelectObject(old);
    }

    // 贴敌机
    for (int nEnemy = 0; nEnemy < m_vecEnemy.size(); nEnemy++)
    {
        CPlane enemy = m_vecEnemy[nEnemy];
        if (enemy.m_bExist)
        {
            mDC->SelectObject(enemy.m_pBMP_M);
            enemy.m_pBMP_M->GetBitmap(&bmp);
            m_pBuffer->BitBlt(enemy.m_nX, enemy.m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCAND);
            //mDC->SelectObject(old);
            mDC->SelectObject(enemy.m_pBMP_S);
            enemy.m_pBMP_S->GetBitmap(&bmp);
            m_pBuffer->BitBlt(enemy.m_nX, enemy.m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCPAINT);
            //mDC->SelectObject(old);
        }
        // 贴敌机炮弹
        for (int nShell = 0; nShell < enemy.m_vecShell.size(); nShell++)
        {
            CShell shell = enemy.m_vecShell[nShell];
            if (shell.m_bExist)
            {
                mDC->SelectObject(shell.m_pBMP_M);
                shell.m_pBMP_M->GetBitmap(&bmp);
                m_pBuffer->BitBlt(shell.m_nX, shell.m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCAND);
                //mDC->SelectObject(old);
                mDC->SelectObject(shell.m_pBMP_S);
                shell.m_pBMP_S->GetBitmap(&bmp);
                m_pBuffer->BitBlt(shell.m_nX, shell.m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCPAINT);
                //mDC->SelectObject(old);
            }
        }
    }

    // 贴BOSS
    if (m_pBoss->m_bExist)
    {
        mDC->SelectObject(m_pBoss->m_pBMP_M);
        m_pBoss->m_pBMP_M->GetBitmap(&bmp);
        m_pBuffer->BitBlt(m_pBoss->m_nX, m_pBoss->m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCAND);
        //mDC->SelectObject(old);
        mDC->SelectObject(m_pBoss->m_pBMP_S);
        m_pBoss->m_pBMP_S->GetBitmap(&bmp);
        m_pBuffer->BitBlt(m_pBoss->m_nX, m_pBoss->m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCPAINT);
        //mDC->SelectObject(old);
        // 贴BOSS炮弹
        for (int nShell = 0; nShell < m_pBoss->m_vecShell.size(); nShell++)
        {
            CShell shell = m_pBoss->m_vecShell[nShell];
            if (shell.m_bExist)
            {
                mDC->SelectObject(shell.m_pBMP_M);
                shell.m_pBMP_M->GetBitmap(&bmp);
                m_pBuffer->BitBlt(shell.m_nX, shell.m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCAND);
                //mDC->SelectObject(old);
                mDC->SelectObject(shell.m_pBMP_S);
                shell.m_pBMP_S->GetBitmap(&bmp);
                m_pBuffer->BitBlt(shell.m_nX, shell.m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCPAINT);
                //mDC->SelectObject(old);
            }
        }
    }

    // 贴主机炮弹
    for (int shellNO = 0; shellNO < PLAYER_SHELL_MAX; shellNO++)
    {
        CShell shell = m_pPlayer->m_vecShell[shellNO];
        if (shell.m_bExist)
        {
            mDC->SelectObject(shell.m_pBMP_M);
            shell.m_pBMP_M->GetBitmap(&bmp);
            m_pBuffer->BitBlt(shell.m_nX, shell.m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCAND);
            //mDC->SelectObject(old);
            mDC->SelectObject(shell.m_pBMP_S);
            shell.m_pBMP_S->GetBitmap(&bmp);
            m_pBuffer->BitBlt(shell.m_nX, shell.m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCPAINT);
            //mDC->SelectObject(old);
        }
        
    }

    // 贴爆炸效果
    for (int blowUpNO = 0; blowUpNO < m_vecBlowUp.size(); blowUpNO++)
    {
        mDC->SelectObject(m_vecBlowUp[blowUpNO].m_pBMP_M);
        m_vecBlowUp[blowUpNO].m_pBMP_M->GetBitmap(&bmp);
        m_pBuffer->BitBlt(m_vecBlowUp[blowUpNO].m_nX, m_vecBlowUp[blowUpNO].m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCAND);
        //mDC->SelectObject(old);
        mDC->SelectObject(m_vecBlowUp[blowUpNO].m_pBMP_S);
        m_vecBlowUp[blowUpNO].m_pBMP_S->GetBitmap(&bmp);
        m_pBuffer->BitBlt(m_vecBlowUp[blowUpNO].m_nX, m_vecBlowUp[blowUpNO].m_nY, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCPAINT);
        //mDC->SelectObject(old);
        if (m_vecBlowUp[blowUpNO].isFinish())
        {
            m_vecBlowUp.erase(m_vecBlowUp.begin() + blowUpNO);
        }
    }
    
    mDC->SelectObject(old);

    // 贴分数
        // 初始化字体
    SetTextColor(m_pBuffer->GetSafeHdc(), RGB(255, 255, 0));
    SetBkMode(m_pBuffer->GetSafeHdc(), TRANSPARENT);
    m_pBuffer->TextOutA(10, 20, std::to_string(this->m_lScorc).c_str());

    
    //old->DeleteObject();
    mDC->DeleteDC();
}

void CGameAI::gameLogic()
{
    if (IN_GAME == m_nStatus)
    {
        
        switch (gameOver())
        {
        case GAME_OVER: // 延迟播放爆炸动画
            showGame();
            m_nTimelag++;
            if (30 <= m_nTimelag)
            {
                Sound::Get().Stop("sc1");
                Sound::Get().PlayWaveFile("sc2", true);
                m_nStatus = IN_LOSS;
            }
            break;
        case GAME_WIN: // 延迟播放爆炸动画
            showGame();
            m_nTimelag++;
            if (30 <= m_nTimelag)
            {
                Sound::Get().Stop("sc1");
                Sound::Get().PlayWaveFile("sc2", true);
                m_nStatus = IN_WIN;
            }
            break;
        default:
            produceEnemy();
            enemyShoot();
            collisionDetection();
            break;
        }
    }
    
}

void CGameAI::produceEnemy()
{
    std::mt19937 rng = std::mt19937(std::random_device()());
    //std::uniform_int_distribution<int> randomProduce{ 3, 5 };
    std::uniform_int_distribution<int> randomX{ 30, 365 };

    if ((IN_GAME == m_nStatus) && (m_nProduceInterval >=120) && (m_fGameTime - BOSS_DISPLAY_TIME < 0.0f)) // 间隔3秒产生一架敌机，一分钟后不再产生
    {
        for (int nEnemy = 0; nEnemy < m_vecEnemy.size(); nEnemy++)
        {
            if (!m_vecEnemy[nEnemy].m_bExist)
            {
                m_vecEnemy[nEnemy].display();
                m_vecEnemy[nEnemy].m_nX = randomX(rng);
                m_vecEnemy[nEnemy].m_nY = -65;

                m_nProduceInterval = 0;
                break;
            }
        }
    }
    else if ((IN_GAME == m_nStatus) && (m_fGameTime - BOSS_DISPLAY_TIME > 0.0f)) // 召唤boss
    {
        this->m_pBoss->m_bExist = true;


    }

}

void CGameAI::enemyShoot()
{
    // 敌机射击
    for (int nEnemy = 0; nEnemy < m_vecEnemy.size(); nEnemy++)
    {
        if (m_vecEnemy[nEnemy].m_bExist && m_vecEnemy[nEnemy].m_bIsEnemy)
        {
            m_vecEnemy[nEnemy].shoot();
            
        }
    }

    // BOSS射击
    if ((IN_GAME == m_nStatus) && (m_fGameTime - BOSS_DISPLAY_TIME > 0.0f) && m_pBoss->m_bExist)
    {
        for (int nShell = 0; nShell < m_pBoss->m_vecShell.size(); nShell++)
        {
            if (m_pBoss->m_bExist && m_pBoss->m_vecShell[nShell].m_bIsEnemy)
            {
                m_pBoss->shoot();

            }
        }
    }
}

void CGameAI::collisionDetection()
{
    // 主机边缘检测
    if (5 >= m_pPlayer->m_nX)
    {
        m_pPlayer->m_nX = 5;
    }
    if (5 >= m_pPlayer->m_nY)
    {
        m_pPlayer->m_nY = 5;
    }
    if ((m_pWnd->m_rect.right - m_pPlayer->m_bmp.bmWidth) <= m_pPlayer->m_nX)
    {
        m_pPlayer->m_nX = m_pWnd->m_rect.right - m_pPlayer->m_bmp.bmWidth;
    }
    if ((m_pWnd->m_rect.bottom - m_pPlayer->m_bmp.bmHeight) <= m_pPlayer->m_nY)
    {
        m_pPlayer->m_nY = m_pWnd->m_rect.bottom - m_pPlayer->m_bmp.bmHeight;
    }

    // 敌机边缘检测
    for (int nEnemy = 0; nEnemy < m_vecEnemy.size(); nEnemy++)
    {
        if (m_pWnd->m_rect.bottom < m_vecEnemy[nEnemy].m_nY)
        {
            m_vecEnemy[nEnemy].disappear();
        }
    }

    // 我方炮弹命中
    for (int shell = 0; shell < m_pPlayer->m_vecShell.size(); shell++)
    {
        if (m_pPlayer->m_vecShell[shell].m_bExist)
        {
            // 敌机
            for (int nEnemy = 0; nEnemy < m_vecEnemy.size(); nEnemy++)
            {
                if (m_vecEnemy[nEnemy].m_bExist)
                {
                    if (Math::BoundingBox(m_vecEnemy[nEnemy].m_nX, m_vecEnemy[nEnemy].m_nY, m_vecEnemy[nEnemy].m_bmp.bmWidth, m_vecEnemy[nEnemy].m_bmp.bmHeight,
                                          m_pPlayer->m_vecShell[shell].m_nX, m_pPlayer->m_vecShell[shell].m_nY, m_pPlayer->m_vecShell[shell].m_bmp.bmWidth, m_pPlayer->m_vecShell[shell].m_bmp.bmHeight))
                    {
                        m_vecEnemy[nEnemy].damaged(m_pPlayer->m_vecShell[shell]);

                        if (m_vecEnemy[nEnemy].m_nHP <= 0)
                        {
                            this->m_lScorc += 100;
                            // 敌机消失，炮弹消失
                            m_vecEnemy[nEnemy].disappear();
                            m_pPlayer->m_vecShell[shell].disappear();
                            // 敌机爆炸
                            setBlowUp(m_vecEnemy[nEnemy].m_nX + m_vecEnemy[nEnemy].m_bmp.bmWidth / 2, m_vecEnemy[nEnemy].m_nY + m_vecEnemy[nEnemy].m_bmp.bmHeight / 2);
                            // 炮弹爆炸
                            //
                            //
                        }
                        
                    }
                }
            }
            // BOSS
            if (m_pBoss->m_bExist)
            {
                if (Math::BoundingBox(m_pBoss->m_nX, m_pBoss->m_nY, m_pBoss->m_bmp.bmWidth, m_pBoss->m_bmp.bmHeight,
                                      m_pPlayer->m_vecShell[shell].m_nX, m_pPlayer->m_vecShell[shell].m_nY, m_pPlayer->m_vecShell[shell].m_bmp.bmWidth, m_pPlayer->m_vecShell[shell].m_bmp.bmHeight))
                {
                    this->m_lScorc += 100;

                    m_pBoss->damaged(m_pPlayer->m_vecShell[shell]);
                    m_pPlayer->m_vecShell[shell].disappear();

                    // 命中炮弹的爆炸效果
                    setBlowUp(m_pPlayer->m_vecShell[shell].m_nX + m_pPlayer->m_vecShell[shell].m_bmp.bmWidth / 2, m_pPlayer->m_vecShell[shell].m_nY + m_pPlayer->m_vecShell[shell].m_bmp.bmHeight / 2);
                }
            }
        }
    }
}

int CGameAI::gameOver()
{
    // 敌机-主机
    if (m_pPlayer->m_bExist)
    {
        if (m_fGameTime - BOSS_DISPLAY_TIME < 0.0f)
        {
            for (int nEnemy = 0; nEnemy < m_vecEnemy.size(); nEnemy++)
            {
                if (m_vecEnemy[nEnemy].m_bExist)
                {
                    if (Math::BoundingBox(m_vecEnemy[nEnemy].m_nX, m_vecEnemy[nEnemy].m_nY, m_vecEnemy[nEnemy].m_bmp.bmWidth, m_vecEnemy[nEnemy].m_bmp.bmHeight,
                                          m_pPlayer->m_nX, m_pPlayer->m_nY, m_pPlayer->m_bmp.bmWidth, m_pPlayer->m_bmp.bmHeight))
                    {
                        m_vecEnemy[nEnemy].disappear();
                        // 敌机爆炸
                        setBlowUp(m_vecEnemy[nEnemy].m_nX + m_vecEnemy[nEnemy].m_bmp.bmWidth / 2, m_vecEnemy[nEnemy].m_nY + m_vecEnemy[nEnemy].m_bmp.bmHeight / 2);

                        m_pPlayer->disappear();
                        // 主机爆炸
                        setBlowUp(m_pPlayer->m_nX + m_pPlayer->m_bmp.bmWidth / 2, m_pPlayer->m_nY + m_pPlayer->m_bmp.bmHeight / 2);

                        return GAME_OVER;
                    }
                }

                // 敌方炮弹-主机
                for (int shell = 0; shell < m_vecEnemy[nEnemy].m_vecShell.size(); shell++)
                {
                    if (m_vecEnemy[nEnemy].m_vecShell[shell].m_bExist)
                    {
                        if (Math::BoundingBox(m_vecEnemy[nEnemy].m_vecShell[shell].m_nX, m_vecEnemy[nEnemy].m_vecShell[shell].m_nY, m_vecEnemy[nEnemy].m_vecShell[shell].m_bmp.bmWidth, m_vecEnemy[nEnemy].m_vecShell[shell].m_bmp.bmHeight,
                                              m_pPlayer->m_nX, m_pPlayer->m_nY, m_pPlayer->m_bmp.bmWidth, m_pPlayer->m_bmp.bmHeight))
                        {
                            m_pPlayer->disappear();
                            // 主机爆炸
                            setBlowUp(m_pPlayer->m_nX + m_pPlayer->m_bmp.bmWidth / 2, m_pPlayer->m_nY + m_pPlayer->m_bmp.bmHeight / 2);

                            return GAME_OVER;
                        }
                    }
                }
            }
        }


        // BOSS-主机
        if ((m_pBoss->m_bExist) && (m_fGameTime - BOSS_DISPLAY_TIME > 0.0f))
        {
            if (Math::BoundingBox(m_pBoss->m_nX, m_pBoss->m_nY, m_pBoss->m_bmp.bmWidth, m_pBoss->m_bmp.bmHeight,
                                  m_pPlayer->m_nX, m_pPlayer->m_nY, m_pPlayer->m_bmp.bmWidth, m_pPlayer->m_bmp.bmHeight))
            {
                m_pBoss->disappear();
                m_pPlayer->disappear();
                // BOSS爆炸
                setBlowUp(m_pBoss->m_nX + m_pBoss->m_bmp.bmWidth / 2, m_pBoss->m_nY + m_pBoss->m_bmp.bmHeight / 2);
                // 主机爆炸
                setBlowUp(m_pPlayer->m_nX + m_pPlayer->m_bmp.bmWidth / 2, m_pPlayer->m_nY + m_pPlayer->m_bmp.bmHeight / 2);

                return GAME_OVER;
            }
        }

        // BOSS炮弹-主机
        for (int shell = 0; shell < m_pBoss->m_vecShell.size(); shell++)
        {
            if (m_pBoss->m_vecShell[shell].m_bExist)
            {
                if (Math::BoundingBox(m_pBoss->m_vecShell[shell].m_nX, m_pBoss->m_vecShell[shell].m_nY, m_pBoss->m_vecShell[shell].m_bmp.bmWidth, m_pBoss->m_vecShell[shell].m_bmp.bmHeight,
                                      m_pPlayer->m_nX, m_pPlayer->m_nY, m_pPlayer->m_bmp.bmWidth, m_pPlayer->m_bmp.bmHeight))
                {
                    m_pPlayer->disappear();
                    m_pBoss->m_vecShell[shell].disappear();
                    // 主机爆炸
                    setBlowUp(m_pPlayer->m_nX + m_pPlayer->m_bmp.bmWidth / 2, m_pPlayer->m_nY + m_pPlayer->m_bmp.bmHeight / 2);

                    return GAME_OVER;
                }
            }
        }
    }
    else if (!(m_pPlayer->m_bExist))
    {
        return GAME_OVER;
    }
    
      

    // WIN
    if (m_pBoss->m_nHP <= 0)
    {
        m_pBoss->disappear();
        // BOSS爆炸
        setBlowUp(m_pBoss->m_nX + m_pBoss->m_bmp.bmWidth / 2, m_pBoss->m_nY + m_pBoss->m_bmp.bmHeight / 2);
        return GAME_WIN;
    }

    return GAME_CONTINUE;
}

void CGameAI::setBlowUp(int nCentreX, int nCentreY)
{
    CBlowUp blowUp;
    blowUp.setDir(nCentreX, nCentreY);
    m_vecBlowUp.push_back(blowUp);
}

