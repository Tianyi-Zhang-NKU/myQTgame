#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include <QTimer>
#include "config.h"
#include "map.h"
#include "red_block.h"
#include "dred_block.h"
#include "connon1.h"
#include "connon3.h"
#include "connon4.h"
#include "wheel.h"
#include "enemy_plane.h"
#include "cloud.h"
#include "winscene.h"
#include "losescene.h"

#include <QPushButton>
#include "mshop.h"
#include "mcard.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainScene;
}
QT_END_NAMESPACE

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();

    //是否开始游戏
    bool m_start=false;

    //初始化场景
    void initScene();

    //初始化一个游戏胜利场景，用于后续的跳转界面
    winscene * m_winscene;

    //初始化一个游戏失败场景，用于后续的跳转界面
    losescene * m_losescene;

    //游戏胜利失败判断函数
    int m_game();

    //游戏胜利失败界面跳转函数
    void m_game_change(int);

    //启动游戏
    void playGame();

    //更新所有游戏中元素的坐标
    void updatePosition();

    //绘制到屏幕中 函数名称不可改
    void paintEvent(QPaintEvent*);

    //定时器
    QTimer m_Timer;

    //计时器
    int m_record=0;

    //地图对象
    Map m_map;

    //商店对象
    Mshop m_shop;

    //卡片对象
    Mcard m_card;


    // 关于阳光
    void createsunBtn();
    void removesunBtn();
    QTimer *sun_timer;
    QList<QPushButton*> buttons;


    // 阳光数量，初始值为200
    int sun_sum=200;
    // 计时器，用于控制动画或消失时间
    //int sun_counter=0;



    //红色方块“弹夹”
    red_block m_block[BLOCK_NUM];
    //红色方块出场
    void red_blocktoscene();

    //深红色方块“弹夹”
    dred_block md_block[DBLOCK_NUM];


    //加农炮1对象
    connon1 m_connon1;
    //碰撞检测
    void collisionDetection_connon1();


    //加农炮3对象
    connon3 m_connon3;
    //碰撞检测左前
    void collisionDetection_connon3_1();
    //碰撞检测正前
    void collisionDetection_connon3_2();
    //碰撞检测右前
    void collisionDetection_connon3_3();

    //滚轮对象
    wheel m_wheel;
    //碰撞检测
    void collisionDetection_wheel();

    //加农炮4对象
    connon4 m_connon4;

    //敌机对象
    enemy_plane m_enemy_plane;

    //云朵对象
    cloud m_cloud;



private:
    Ui::MainScene *ui;
};


#endif // MAINSCENE_H
