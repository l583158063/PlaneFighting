#pragma once
#include<sstream>
#include"CPlane.h"
#include"CProperty.h"

#define PLAYER_COUNT 1
#define POSTURE_COUNT 5
#define SHOOT_INTERVAL_PLAYER 20
#define PLAYER_HP 10
#define PLAYER_LIFE 3
#define PLAYER_SHELL_MAX 20


using namespace std;

class CPlayer : public CPlane
{
public:
    vector<CBitmap *> m_vecBMPPlayerM;
    vector<CBitmap *> m_vecBMPPlayerS;
    //vector<CProperty> m_vecProp;
    //int m_nCountSuperWeapon;
    int m_nPosture;

public:
    CPlayer();
    void initPlane();
    void shoot();
    void move();
    void shellFly();
    //void addProp(CProperty);
    //void useProp();
    //void useSuperWeapon();
};