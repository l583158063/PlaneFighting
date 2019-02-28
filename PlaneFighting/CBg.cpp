#pragma once
#include"CBg.h"
#include<sstream>


CBg::CBg()
{
    m_nX = 0;
    m_nY = 0;
    m_nSeq = 0;
    for (int bgNO = 0; bgNO < BG_COUNT; bgNO++)
    {
        std::stringstream ss;
        ss << "maps/bg" << bgNO << ".bmp";
        CBitmap *bmp = new CBitmap;
        bmp->m_hObject = LoadImage(NULL, ss.str().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        this->m_vecBMP.push_back(bmp);
    }
    this->m_vecBMP[0]->GetBitmap(&m_bmp);
}

void CBg::roll()
{
    m_nY = ((m_bmp.bmHeight <= m_nY) ? 0 : m_nY += 1);
}

void CBg::changeBg(int bgNO)
{

}