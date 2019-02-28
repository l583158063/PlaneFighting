#pragma once
#include<afxwin.h>
#include<vector>

#define OP_GAME_START 0
#define OP_GAME_OPTION 1
#define OP_GAME_INTRO 2
#define OP_EXIT 3

using namespace std;

class CMyMenu
{
public:
    int m_nOption;
    CBitmap * m_pBg;
    vector<CBitmap *> m_vecOption;

public:
    CMyMenu();

};