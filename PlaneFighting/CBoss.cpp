#include "CBoss.h"

CBoss::CBoss()
{

}

CBoss::CBoss(bool bIsEnemy, const char * ssFileName_M, const char * ssFileName_S, int nX, int nY, int nHP, int nLife, int nShootInterval)
    :CPlane(bIsEnemy, ssFileName_M, ssFileName_S, nX, nY, nHP, nLife, nShootInterval)
{
    for (int shellType = 0; shellType < BOSS_SHELL_TYPE; shellType++)
    {
        m_nBossInterval[shellType] = nShootInterval;
    }
    this->getBMPSize(m_pBMP_M, m_bmp.bmWidth, m_bmp.bmHeight);
}

void CBoss::shoot()
{
    m_nBossInterval[0] = ((SHOOT_INTERVAL_BOSS == m_nBossInterval[0]) ? SHOOT_INTERVAL_BOSS : ++m_nBossInterval[0]);
    m_nBossInterval[1] = ((SHOOT_INTERVAL_BOSS - 3000 == m_nBossInterval[1]) ? SHOOT_INTERVAL_BOSS - 3000 : ++m_nBossInterval[1]);
    m_nBossInterval[2] = ((SHOOT_INTERVAL_BOSS - 6000 == m_nBossInterval[2]) ? SHOOT_INTERVAL_BOSS - 6000 : ++m_nBossInterval[2]);

    if (SHOOT_INTERVAL_BOSS == m_nBossInterval[0] && m_bExist)
    {
        int shellCount = 5;
        for (int shellNO = 0; shellNO < m_vecShell.size(); shellNO++)
        {
            if (!this->m_vecShell[shellNO].m_bExist)
            {
                this->m_vecShell[shellNO].changeShell(5);
                this->m_vecShell[shellNO].display();
                this->m_vecShell[shellNO].m_nX = this->m_nX + 20 * shellCount;
                this->m_vecShell[shellNO].m_nY = this->m_nY + this->m_bmp.bmHeight;
                this->m_nBossInterval[0] = 0;
                shellCount--;
                if (0 == shellCount)
                {
                    break;
                }
            }
        }
    }
    if ((SHOOT_INTERVAL_BOSS - 3000) == m_nBossInterval[1] && m_bExist)
    {
        int shellCount = 5;
        for (int shellNO = 0; shellNO < m_vecShell.size(); shellNO++)
        {
            if (!this->m_vecShell[shellNO].m_bExist)
            {
                this->m_vecShell[shellNO].changeShell(4);
                this->m_vecShell[shellNO].display();
                this->m_vecShell[shellNO].m_nX = this->m_nX + 20 * shellCount;
                this->m_vecShell[shellNO].m_nY = this->m_nY + this->m_bmp.bmHeight;
                this->m_nBossInterval[1] = 0;
                shellCount--;
                if (0 == shellCount)
                {
                    break;
                }
            }
        }
    }
    if ((SHOOT_INTERVAL_BOSS - 6000) == m_nBossInterval[2] && m_bExist)
    {
        int shellCount = 5;
        for (int shellNO = 0; shellNO < m_vecShell.size(); shellNO++)
        {
            
            if (!this->m_vecShell[shellNO].m_bExist)
            {
                this->m_vecShell[shellNO].changeShell(3);
                this->m_vecShell[shellNO].display();
                this->m_vecShell[shellNO].m_nX = this->m_nX + 20 * shellCount;
                this->m_vecShell[shellNO].m_nY = this->m_nY + this->m_bmp.bmHeight;
                this->m_nBossInterval[2] = 0;
                shellCount--;
                if (0 == shellCount)
                {
                    break;
                }
            }
        }
    }
}

void CBoss::move()
{
    if (m_bExist)
    {
        m_nY = ((20 < m_nY) ? 20 : m_nY += 1);
        if (20 <= m_nY)
        {
            if (m_nX > 50 && m_bIsGoLift) // 向左走
            {
                m_nX -= 1;
            }
            if (m_nX <= 50 && m_bIsGoLift) // 左走到头
            {
                m_nX += 1;
                m_bIsGoLift = false;
            }
            if (m_nX < 260 && !m_bIsGoLift) // 向右走
            {
                m_nX += 1;
            }
            if (m_nX >= 260 && !m_bIsGoLift) // 右走到头
            {
                m_nX -= 1;
                m_bIsGoLift = true;
            }
        }
    }
    
}

void CBoss::shellFly()
{
    if (m_bExist)
    {
        for (int shellNO = 0; shellNO < m_vecShell.size(); shellNO++)
        {
            this->m_vecShell[shellNO].move();

            if (720 < this->m_vecShell[shellNO].m_nY)
            {
                this->m_vecShell[shellNO].disappear();
            }
        }
    }
    
}