#include "mainscene.h"
#include "ui_mainscene.h"
#include "config.h"
#include <QIcon>
#include <QPainter>

#include <QTime>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include "mshop.h"

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //点击退出菜单项 实现退出
    connect (ui->actionquit,&QAction::triggered,[=]() {
        this->close();
    });


    //调用初始化场景
    initScene();

    //启动游戏
    playGame();

    this->m_winscene =new winscene;
    this->m_losescene =new losescene;

    // 创建定时器，每隔4秒创建一个新按钮
    sun_timer = new QTimer(this);
    connect(sun_timer, &QTimer::timeout, this, &MainScene::createsunBtn);
    sun_timer->start(4000);

}


MainScene::~MainScene()
{
    delete ui;

    // 清理所有按钮
    qDeleteAll(buttons);
    buttons.clear();
    delete sun_timer;
}


void MainScene::initScene()
{
    //设置窗口固定尺寸
    setFixedSize (GAME_WIDTH_1,GAME_HEIGHT);

    //设置标题
    setWindowTitle (GAME_TITLE);

    //设置图标
    setWindowIcon(QIcon(":/pictures/00-icon.png"));

    //定时器设置
    m_Timer.setInterval(GAME_RATE);

}


void MainScene::playGame()
{

        //启动定时器
        m_Timer.start();



    //监听定时器发送的信号
    connect(&m_Timer,&QTimer::timeout,[=]() {



        //更新游戏中元素的坐标
        updatePosition();

        //绘制到屏幕中
        update(); //paintEvent的系统自带函数

        //红色方块出场
        red_blocktoscene();

        //碰撞检测
        //加农炮1碰撞检测
        collisionDetection_connon1();
        //加农炮3碰撞检测左前
        collisionDetection_connon3_1();
        //加农炮3碰撞检测正前
        collisionDetection_connon3_2();
        //加农炮3碰撞检测右前
        collisionDetection_connon3_3();
        //滚轮碰撞检测
        collisionDetection_wheel();

        //计时器
        m_record++;

        // 阳光计时器，用于控制动画或消失时间(暂未实现)
        //sun_counter++;

        //判断游戏是否胜利并跳转界面
        int m_n_1=0;
        m_n_1=m_game();
        m_game_change(m_n_1);

    });

}


void MainScene::updatePosition()
{

    //加农炮1
    //发射加农炮1子弹
    m_connon1.shoot();
    //计算所有非空闲子弹的当前坐标
    for(int i=0;i<CONNON1_BULLET_NUM;i++)
    {
        //如果非空闲,计算发射位置
        if(m_connon1.m_bullets[i].m_Free == false){
            m_connon1.m_bullets[i].updatePosition();
        }
    }


    //加农炮3
    //发射加农炮3子弹
    m_connon3.shoot();
    //计算所有左前非空闲子弹的当前坐标
    for(int i=0;i<CONNON3_BULLET_NUM_1;i++)
    {
        //如果非空闲,计算发射位置
        if(m_connon3.m_bullets_1[i].m_Free == false){
            m_connon3.m_bullets_1[i].updatePosition();
        }
    }
    //计算所有正前非空闲子弹的当前坐标
    for(int i=0;i<CONNON3_BULLET_NUM_2;i++)
    {
        //如果非空闲,计算发射位置
        if(m_connon3.m_bullets_2[i].m_Free == false){
            m_connon3.m_bullets_2[i].updatePosition();
        }
    }
    //计算所有右前非空闲子弹的当前坐标
    for(int i=0;i<CONNON3_BULLET_NUM_3;i++)
    {
        //如果非空闲,计算发射位置
        if(m_connon3.m_bullets_3[i].m_Free == false){
            m_connon3.m_bullets_3[i].updatePosition();
        }
    }

    //滚轮
    m_wheel.updatePosition();


}

//绘制屏幕
void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this); //this指针，表示画到当前屏幕

    //绘制背景图
    painter.drawPixmap(0,0,m_map.m_map);

    //商店阳光对象
    painter.drawPixmap(GAME_WIDTH,0,m_shop.shop_sun);

    // 绘制阳光值
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 16));
    painter.drawText(GAME_WIDTH+55, 150, QString("%1").arg(sun_sum));
    //商店卡片对象
    painter.drawPixmap(GAME_WIDTH,168,m_shop.shop_card);

    //卡片对象1
    painter.drawPixmap(GAME_WIDTH+4,168,m_card.m_card);
    //卡片对象2
    painter.drawPixmap(GAME_WIDTH+4,368,m_card.m_card);
    //卡片对象3
    painter.drawPixmap(GAME_WIDTH+4,568,m_card.m_card);

    //绘制红色方块
    for(int i=0;i<BLOCK_NUM;i++)
    {
        //如果非空闲且存活,绘制红色方块
        if(m_block[i].m_Free == false && m_block[i].m_Free1 == true){
            painter.drawPixmap(m_block[i].m_red_block_X , m_block[i].m_red_block_Y , m_block[i].m_red_block);
        }
    }

    //绘制深红色方块
    //painter.drawPixmap(temp_dblock.m_dred_block_X,temp_dblock.m_dred_block_Y,temp_dblock.m_dred_block);


    //加农炮1
    //绘制加农炮1
    painter.drawPixmap(m_connon1.m_connon1_posX+15,m_connon1.m_connon1_posY,m_connon1.m_connon1);
    //绘制加农炮1子弹
    for(int i=0;i<CONNON1_BULLET_NUM;i++)
    {
        //如果非空闲,绘制子弹
        if(m_connon1.m_bullets[i].m_Free == false){
            painter.drawPixmap(m_connon1.m_bullets[i].m_bullet_X+3,m_connon1.m_bullets[i].m_bullet_Y+3,m_connon1.m_bullets[i].m_bullet);
        }
    }


    //加农炮3
    //绘制加农炮3
    painter.drawPixmap(m_connon3.m_connon3_posX,m_connon3.m_connon3_posY,m_connon3.m_connon3);
    //绘制加农炮3左前子弹
    for(int i=0;i<CONNON3_BULLET_NUM_1;i++)
    {
        //如果非空闲,绘制子弹
        if(m_connon3.m_bullets_1[i].m_Free == false){
            painter.drawPixmap(m_connon3.m_bullets_1[i].m_bullet_X+3,m_connon3.m_bullets_1[i].m_bullet_Y+3,m_connon3.m_bullets_1[i].m_bullet);
        }
    }
    //绘制加农炮3正前子弹
    for(int i=0;i<CONNON3_BULLET_NUM_2;i++)
    {
        //如果非空闲,绘制子弹
        if(m_connon3.m_bullets_2[i].m_Free == false){
            painter.drawPixmap(m_connon3.m_bullets_2[i].m_bullet_X+3,m_connon3.m_bullets_2[i].m_bullet_Y+3,m_connon3.m_bullets_2[i].m_bullet);
        }
    }
    //绘制加农炮3右前子弹
    for(int i=0;i<CONNON3_BULLET_NUM_3;i++)
    {
        //如果非空闲,绘制子弹
        if(m_connon3.m_bullets_3[i].m_Free == false){
            painter.drawPixmap(m_connon3.m_bullets_3[i].m_bullet_X+3,m_connon3.m_bullets_3[i].m_bullet_Y+3,m_connon3.m_bullets_3[i].m_bullet);
        }
    }

    //绘制滚轮
    painter.drawPixmap(m_wheel.m_wheel_posX,m_wheel.m_wheel_posY,m_wheel.m_wheel);

    //绘制敌机
    //painter.drawPixmap(m_enemy_plane.m_enemy_plane_posX,m_enemy_plane.m_enemy_plane_posY,m_enemy_plane.m_enemy_plane);

    //绘制云朵
    //painter.drawPixmap(m_cloud.m_cloud_posX,m_cloud.m_cloud_posY,m_cloud.m_cloud);

    //绘制加农炮4
    //painter.drawPixmap(m_connon4.m_connon4_posX+105,m_connon4.m_connon4_posY,m_connon4.m_connon4);

}

int MainScene::m_game( )
{
    int m_n=0;
    //超时游戏判负
    if(GAME_TIME<=m_record){
        return 1;
    }
    for(int i=0;i<BLOCK_NUM;i++){
        if(m_block[i].m_Free == false && m_block[i].m_Free1 == true ){
            m_n++;
        }
    }
    for(int i=0;i<DBLOCK_NUM;i++){
        if(md_block[i].md_Free == false && md_block[i].m_Free1 == true){
            m_n++;
        }
    }
    //在游戏时间内敌人数量已经减少到预设标准，游戏胜利
    if(m_n<=GAME_WIN){
        return 2;
    }
    //在游戏时间内未完成要求，游戏继续
    return 3;
}

void MainScene::m_game_change(int n)
{
    if(n==1){
        this->m_losescene->show();
        this->hide();
    }
    if(n==2){
        this->m_winscene->show();
        this->hide();
    }
    return;
}


//红色方块出场
void MainScene::red_blocktoscene()
{
    for(int i=0;i<BLOCK_NUM;i++){
        //如果是空闲且存活红色方块 出场
        if(m_block[i].m_Free && m_block[i].m_Free1){
            m_block[i].m_Free=false;

            //坐标
            m_block[i].m_red_block_X = 2+(i-(i/18)*18)*30;
            m_block[i].m_red_block_Y = 3+(i/18)*30;

            //更新边框
            m_block[i].m_Rect.moveTo(m_block[i].m_red_block_X, m_block[i].m_red_block_Y);
        }
    }
}


//加农炮1子弹与红色方块碰撞检测
void MainScene::collisionDetection_connon1()
{
    //遍历所有非空闲的红色方块
    for(int i = 0 ;i <BLOCK_NUM;i++){
        if(!m_block[i].m_Free1){
            //已死亡红色方块跳转下一次循环
            continue;
        }
        //遍历所有非空闲的加农炮1子弹
        for(int j=0;j<CONNON1_BULLET_NUM;j++){
            if(m_connon1.m_bullets[j].m_Free){
                //空闲子弹跳转下一次循环
                continue;
            }
            //如果子弹矩形框和红色方块矩形框相交,发生碰撞,子弹变为空闲状态，红色方块死亡
            if(m_block[i].m_Rect.intersects(m_connon1.m_bullets[j].m_Rect)){
                m_block[i].m_Free1 = false;//红色方块死亡
                m_connon1.m_bullets[j].m_Free = true;
            }
        }
    }
}


//加农炮3左前子弹与红色方块碰撞检测
void MainScene::collisionDetection_connon3_1()
{
    //遍历所有非空闲的红色方块
    for(int i = 0 ;i <BLOCK_NUM;i++){
        if(!m_block[i].m_Free1){
            //已死亡红色方块跳转下一次循环
            continue;
        }
        //遍历所有非空闲的加农炮3左前子弹
        for(int j=0;j<CONNON3_BULLET_NUM_1;j++){
            if(m_connon3.m_bullets_1[j].m_Free){
                //空闲子弹跳转下一次循环
                continue;
            }
            //如果子弹矩形框和红色方块矩形框相交,发生碰撞,子弹变为空闲状态，红色方块死亡
            if(m_block[i].m_Rect.intersects(m_connon3.m_bullets_1[j].m_Rect)){
                m_block[i].m_Free1 = false;//红色方块死亡
                m_connon3.m_bullets_1[j].m_Free = true;
            }
        }
    }

}

//加农炮3正前子弹与红色方块碰撞检测
void MainScene::collisionDetection_connon3_2()
{
    //遍历所有非空闲的红色方块
    for(int i = 0 ;i <BLOCK_NUM;i++){
        if(!m_block[i].m_Free1){
            //已死亡红色方块跳转下一次循环
            continue;
        }
        //遍历所有非空闲的加农炮3正前子弹
        for(int j=0;j<CONNON3_BULLET_NUM_2;j++){
            if(m_connon3.m_bullets_2[j].m_Free){
                //空闲子弹跳转下一次循环
                continue;
            }
            //如果子弹矩形框和红色方块矩形框相交,发生碰撞,子弹变为空闲状态，红色方块死亡
            if(m_block[i].m_Rect.intersects(m_connon3.m_bullets_2[j].m_Rect)){
                m_block[i].m_Free1 = false;//红色方块死亡
                m_connon3.m_bullets_2[j].m_Free = true;
            }
        }
    }

}

//加农炮3右前子弹与红色方块碰撞检测
void MainScene::collisionDetection_connon3_3()
{
    //遍历所有非空闲的红色方块
    for(int i = 0 ;i <BLOCK_NUM;i++){
        if(!m_block[i].m_Free1){
            //已死亡红色方块跳转下一次循环
            continue;
        }
        //遍历所有非空闲的加农炮3右前子弹
        for(int j=0;j<CONNON3_BULLET_NUM_3;j++){
            if(m_connon3.m_bullets_3[j].m_Free){
                //空闲子弹跳转下一次循环
                continue;
            }
            //如果子弹矩形框和红色方块矩形框相交,发生碰撞,子弹变为空闲状态，红色方块死亡
            if(m_block[i].m_Rect.intersects(m_connon3.m_bullets_3[j].m_Rect)){
                m_block[i].m_Free1 = false;//红色方块死亡
                m_connon3.m_bullets_3[j].m_Free = true;
            }
        }
    }

}

//滚轮碰撞检测
void MainScene::collisionDetection_wheel()
{
    //遍历所有非空闲的红色方块
    for(int i = 0 ;i <BLOCK_NUM;i++){
        if(!m_block[i].m_Free1){
            //已死亡红色方块跳转下一次循环
            continue;
        }

        //如果滚轮矩形框和红色方块矩形框相交,发生碰撞,子弹变为空闲状态，红色方块死亡
        if(m_block[i].m_Rect.intersects(m_wheel.m_Rect)){
            m_block[i].m_Free1 = false;//红色方块死亡
        }

    }
}



void MainScene::createsunBtn()
{
    // 如果按钮数量达到上限，不再创建
    if(buttons.size() >= MAX_BUTTONS) {
        return;
    }

    //buttons.size() 是获取 QList 容器中元素数量的方法，同时也可用于容器管理：
    //buttons.append(button);  // 添加按钮到列表
    //buttons.removeOne(button); // 从列表移除按钮


    // 初始化随机数生成器
    QTime time = QTime::currentTime();
    srand(time.msec() + time.second() * 1000);

    // 创建新按钮
    QPushButton *button = new QPushButton(this);
    QPixmap pix;
    pix.load(SUN_PATH);

    // 设置按钮大小
    //button->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    //设置按钮固定尺寸
    button->setFixedSize (pix.width(),pix.height());
    //设置不规则样式
    button->setStyleSheet("QPushButton{border:0px;)");
    //设置图标
    button->setIcon (pix);
    //设置图标大小
    button->setIconSize (QSize(pix.width(),pix.height()));


    // 随机位置
    int x = rand() % (GAME_WIDTH - BUTTON_SIZE);
    int y = rand() % (GAME_HEIGHT - BUTTON_SIZE);
    button->move(x, y);

    // 连接点击信号
    connect(button, &QPushButton::clicked, this, &MainScene::removesunBtn);

    // 显示按钮
    button->show();

    // 添加到按钮列表
    buttons.append(button);

    qDebug() << "Button created at (" << x << "," << y << ")";

}

void MainScene::removesunBtn()
{
    // 获取发送信号的按钮
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    if(button) {
        // 从列表中移除
        buttons.removeOne(button);

        // 删除按钮
        button->deleteLater();

        qDebug() << "Button removed";
        sun_sum+=25;
        qDebug()<<"获得25阳光";
    }
}
