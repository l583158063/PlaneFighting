#pragma once
#include<afxwin.h>
#include<vector>

#define BG_COUNT 1


class CBg
{
public:
    std::vector<CBitmap *> m_vecBMP;
    BITMAP m_bmp;
    int m_nX;
    int m_nY;
    int m_nSeq;

public:
    CBg();
    void roll();
    void changeBg(int);
};