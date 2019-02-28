#pragma once
#include<afxwin.h>

class CProperty
{
public:
    CBitmap * m_pBMP;
    BITMAP m_bmp;
    int m_nX;
    int m_nY;
    bool m_bExist;
    int m_nType;

public:
    CProperty();
    void move();
    void display();
    void disappear();
    void active();
};