#pragma once
#include<sstream>
#include"CMyMenu.h"

#define OPTION_COUNT 4

using namespace std;

CMyMenu::CMyMenu()
{
    this->m_nOption = 0;

    this->m_pBg = new CBitmap;
    m_pBg->m_hObject = LoadImage(NULL, "menu/menubg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    for (int optionNO = 0; optionNO < OPTION_COUNT; optionNO++)
    {
        stringstream ss;
        ss << "menu/menu" << optionNO << ".bmp";
        CBitmap *bmp = new CBitmap;
        bmp->m_hObject = LoadImage(NULL, ss.str().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->m_vecOption.push_back(bmp);
    }

}