#ifndef WHEEL_H
#define WHEEL_H
#include <Qpixmap>

class wheel
{
public:
    wheel();

    //更新滚轮坐标
    void updatePosition();

    //设置滚轮的位置
    void setPosition(int x,int y);

    //滚轮资源对象
    QPixmap m_wheel;

    //滚轮坐标
    int m_wheel_posY;
    int m_wheel_posX;

    //滚轮的矩形边框(用于碰撞检测)
    QRect m_Rect;


};

#endif // WHEEL_H
