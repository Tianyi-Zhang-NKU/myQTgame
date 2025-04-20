#include "wheel.h"
#include "config.h"

wheel::wheel() {

    //初始化加载滚轮对象
    m_wheel.load(WHEEL_PATH);

    //初始化测试滚轮坐标
    m_wheel_posX=GAME_WIDTH - 120-2;
    m_wheel_posY=GAME_HEIGHT - m_wheel.height();

    //滚轮边框
    m_Rect.setWidth(m_wheel.width());
    m_Rect.setHeight(m_wheel.height());
    m_Rect.moveTo(m_wheel_posX,m_wheel_posY);
}

    void wheel::updatePosition()
    {
            m_wheel_posY -= GAME_V;
            m_Rect.moveTo(m_wheel_posX, m_wheel_posY);
            return;
    }


