# Plants Vs. Zombies Simplified Version

<p align = "right">StringMax，*********</p>

<p align = "right">南京大学计算机科学与技术系</p>

[TOC]

## 一、主要内容

> 根据知名游戏《植物大战僵尸》，实现一个简化版本。

## 二、实现目标

> 在第二次课程设计的基础之上，将本项目拓展为GUI版本，并保证功能上有一定的增量，使其具有一定可玩性。

## 三、设计思路

### 1. 工具

* IDE: Qt Creator;
* Kits: Desktop Qt 5.12.11 MinGW 64-bit;

### 2. 主要实现思路：

1. 通过QGraphicsItem类, QGraphicsScene类, QGraphicsView类进行场景的建设，其中QGraphicsItem用于各种植物、僵尸以及卡片的实现，QGraphicsScene作为各图元的载体，而QGraphicsView用于呈现画面。
2. 使用状态机机制对游戏状态进行控制和转换。
3. 各图元之间以及图元与场景之间的交流使用Qt中的信号和槽机制实现。
4. 利用QTimer类实现场景中各部件的刷新和运动。
5. 利用Qt中的资源文件实现对图片、音效以及临时字体的管理与调用。
6. 利用QMovie类实现植物与僵尸GIF的播放。
7. 利用QSoundEffect类以及QSound类进行音效和背景音乐的播放，同时利用QThread类排除音效播放对主线程的阻塞。

### 3. 具体实现过程：

#### 1）植物

1. 实现一个继承自QObject与QGraphicsItem的类PlantBase作为所有植物的基类，添加一个表示植物类型的属性plantType以及各植物间相似的属性和方法，然后在各植物中针对不同功能实现具体的方法。
2. 属性：
   * 植物类型：int plantType;
   * 生命值：int hitPoint;
   * 攻击触发器：QTimer trigger;
   * 重绘触发器：QTimer repainter;
   * GIF播放器：QMovie plantGIF;
3. 方法：
   * 构造函数：PlantBase();
   * 边界矩形：QRectF boundingRect() const;
   * 右键删除：void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
   * 返回当前类型（主要用于区分植物、僵尸、子弹）：int type() const;
   * 返回当前植物类型（用于区分不同植物）：int getPlantType() const;
   * 减少血量：void reduceLife(int damage);
   * 返回是否是高坚果：virtual bool isTall() const;
4. 信号：
   * 攻击：void attack(PlantBase*);
   * 删除：void deleteMe(PlantBase*);
5. 槽：
   * 发送攻击信号：virtual void sendAttackSign();
   * 发送删除信号：void sendDeleteMe();
   * 重绘：void repaint();

##### 相关代码

```cpp
#ifndef PLANTBASE_H
#define PLANTBASE_H

#include <QGraphicsItem>

#include <QTimer>
#include <QMovie>
#include <QGraphicsSceneContextMenuEvent>

#define PLANT_HEIGHT 90
#define PLANT_WIDTH 90

#define PLANTTYPE_NULL -1
#define PLANTTYPE_SUNFLOWER 0
#define PLANTTYPE_PEASHOOTER 1
#define PLANTTYPE_REPEATER 2
#define PLANTTYPE_SNOWPEA 3
#define PLANTTYPE_WALLNUT 4
#define PLANTTYPE_TALLNUT 5
#define PLANTTYPE_SQUASH 6
#define PLANTTYPE_CHERRYBOMB 7
#define PLANTTYPE_GARLIC 8
#define PLANTTYPE_PUMPKIN 9

class PlantBase : public QObject, public QGraphicsItem
{
    Q_OBJECT
//    Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry)
//    Q_INTERFACES(QGraphicsItem)
public:
    PlantBase();
protected:
    int hitPoint;
    int plantType;
    QTimer trigger;
public:
    QRectF boundingRect() const;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    int type() const;
    int getPlantType() const;
    void reduceLife(int damage);
    virtual bool isTall() const;
signals:
    void attack(PlantBase*);
    void deleteMe(PlantBase*);
public slots:
    virtual void sendAttackSign();
    void sendDeleteMe();
    void repaint();
//private:
//    QRectF geometryRect;
//public:
//    QRectF geometry() const;
//    void setGeometry(const QRectF& value);

// Gif Display
protected:
    QMovie plantGIF;
    QTimer repainter;
};

#endif // PLANTBASE_H

```

#### 2）僵尸

1. 大致思路同植物；
2. 属性：
   * 僵尸类型：int zombieType;
   * 攻击触发器：QTimer trigger;
   * 移动触发器：QTimer tick;
   * 冰冻判定：bool isFrozen;
   * 冰冻计时：QTimer frozenTime;
   * 音效线程：QThread soundEffectThread;
   * 攻击音效：QSoundEffect* chompSoundEffect[2];
   * GIF播放器：QMovie zombieGIF;
3. 方法：
   * 构造函数：ZombieBase();
   * 析构函数：~ZombieBase();
   * 边界矩形：QRectF boundingRect() const;
   * 返回当前类型（主要用于区分植物、僵尸、子弹）：int type() const;
   * 返回当前僵尸类型（用于区分不同僵尸）：int getZombieType() const;
   * 减少血量：virtual void reduceLife(int damage);
   * 获取击倒僵尸应获得的分数：virtual void getPoint() const;
4. 信号：
   * 删除：void deleteMe(ZombieBase*);
   * 游戏结束：void gameover();
5. 槽：
   * 攻击：virtual void attack();
   * 移动：virtual void zombieMove();
   * 接触冰冻状态：void melt();
   * 进入冰冻状态：void freeze();

##### 相关代码

```cpp
#ifndef ZOMBIEBASE_H
#define ZOMBIEBASE_H

#include <QGraphicsItem>

#include <QTimer>

#include <QThread>
#include <QSoundEffect>

#include <QMovie>

#define ZOMBIETYPE_NULL -1
#define ZOMBIETYPE_NORMAL 0
#define ZOMBIETYPE_CONEHEAD 1
#define ZOMBIETYPE_NEWSPAPER 2
#define ZOMBIETYPE_POLEVAULTING 3
#define ZOMBIETYPE_JACKINTHEBOX 4
#define ZOMBIETYPE_CATAPULT 5

#define ZOMBIE_HEIGHT 90
#define ZOMBIE_WIDTH 50

#define ZOMBIE_TICK 10
#define ZOMBIE_SPEED -0.08
#define ZOMBIE_ATK 100

#define ZOMBIE_FROZENTIME 10000

class ZombieBase : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    ZombieBase();
    ~ZombieBase();
    QRectF boundingRect() const;
    int type() const;
    int getZombieType() const;
    virtual void reduceLife(int damage);
    virtual int getPoint() const;
protected:
    int hitPoint;
    int zombieType;
    QTimer trigger, tick;
    bool isFrozen;
    QTimer frozenTime;
signals:
    void deleteMe(ZombieBase*);
    void gameover();
public slots:
    virtual void attack();
    virtual void zombieMove();
    void melt();
    void freeze();

// Sound Effect
protected:
    QThread soundEffectThread;
    QSoundEffect* chompSoundEffect[2];

// Gif Display
protected:
    QMovie zombieGIF;
};

#endif // ZOMBIEBASE_H

```

#### 3）植物卡片

1. 植物卡片PlantCard类继承自QObject类和QGraphicsItem类，用来进行所表示植物的选中以及相关判定。
2. 属性：
   * 所表示植物类型：int plantType;
   * 冷却判定：bool isReady;
   * 冷却计时：QTimer coolDownTimer;
   * 重绘触发器：QTimer repainter;
   * 选中音效：QSoundEffect seedliftSoundEffect;
   * 选中失败音效：QSoundEffect notenoughsunSoundEffect;
   * 音效线程：QThread soundEffectThread;
3. 方法：
   * 构造函数：PlantCard();
   * 构造函数：PlantCard(int);
   * 获取植物所需阳光：int getCost() const;
   * 边界矩形：QRectF boundingRect() const;
   * 绘制图元：void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
   * 鼠标悬停事件：void hoverEnterEvent(QGraphicsSceneHoverEvent *event);*
   * 鼠标点击事件：void mousePressEvent(QGraphicsSceneMouseEvent* event);
   * 焦点离开事件：void focusOutEvent(QFocusEvent* event);
4. 信号：
   * 点击：void clicked(int type);
   * 播放选中音效：void sendSeedlift();
   * 播放选中失败音效：void sendNotenoughsun();
5. 槽：
   * 启动冷却计时器：void startTimer();
   * 启动冷却计时器：void startTimer(int type);
   * 冷却完毕：void getReady();
   * 重绘：void repaint();

##### 相关代码

```cpp
#ifndef PLANTCARD_H
#define PLANTCARD_H

#include <QGraphicsItem>

#include <QTimer>
#include <QFocusEvent>

#include <QThread>
#include <QSoundEffect>

#define CARD_HEIGHT 60
#define CARD_WIDTH 81

#define PLANT_CNT 10

class PlantCard : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    PlantCard();
    PlantCard(int);
    int getCost() const;
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget);
public slots:
    void startTimer();
    void startTimer(int type);
    void getReady();
    void repaint();
signals:
    void clicked(int type);
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void focusOutEvent(QFocusEvent* event);
private:
    int plantType;
    bool isReady;
    QTimer coolDownTimer;
    QTimer repainter;

// Sound Effect
private:
    QSoundEffect seedliftSoundEffect;
    QSoundEffect notenoughsunSoundEffect;
    QThread soundEffectThread;
signals:
    void sendSeedlift();
    void sendNotenoughsun();
};

#endif // PLANTCARD_H

```

#### 4）游戏场景

1. 作为各图元的载体，实现了继承自QGraphicsScene的游戏场景InterfaceScene类。
2. 属性：
   * 游戏开始按钮：QPushButton gameStartButton;
   * 游戏状态机：QStateMachine machine;
   * 游戏状态：QState* gameStart, * gameRunning, * gamePausing;
   * 终止状态：QFinalState* gameOver;
   * 暂停对话框：QMessageBox* pauseMessageBox;
   * 布局矩形：QRectF platformsRect, cardsRect, infoRect, sloganRect;
   * 被选中植物：int selectedPlant;
   * 植物卡片：PlantCard* cards[PLANT_CNT];
   * 阳光：int sunlight;
   * 阳光自然增加触发器：QTimer sunlightAdder;
   * 阳光显示部件：QLabel sunlightDisplay;
   * 分数：int point;
   * 分数展示部件：QLabel pointDisplay;
   * 僵尸生成向量：QVector \<int> zombieProducer;
   * 僵尸生成触发器：QTimer zombieProducerTick;
   * 游戏音效：QSoundEffect* awoogaSoundEffect, pointsSoundEffect, setplantSoundEffect, shoopSoundEffect, notenoughsunSoundEffect, kernelpultSoundEffect, sirenSoundEffect;
   * 音效线程：QThread soundEffectThread;
   * 背景音乐：QSound* bgmSound;
3. 方法：
   * 构造函数：InterfaceScene();
   * 析构函数：~InterfaceScene();
   * 键盘事件：void keyPressEvent(QKeyEvent* event);
   * 添加植物：bool addPlant(const QPointF point);
   * 鼠标单击事件：void mousePressEvent(QGraphicsSceneMouseEvent* event);
   * 鼠标双击事件（调试用）：void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
4. 信号：
   * 游戏结束：void gameIsOver();
   * 游戏暂停：void gameIsPausing();
   * 游戏继续：void gameIsContinuing();
   * 关闭窗口：void closeView();
   * 鼠标单击：void clicked();
   * 植物添加成功：void plantAddedSuccessfully(int type);
5. 槽：
   * 设置欢迎界面背景：void setCoverBackground();
   * 设置游戏界面背景：void setInterfaceBackground();
   * 呼出暂停消息框：void callPauseMessageBox();
   * 发送游戏暂停信号：void sendGamePausingSignal();
   * 发送游戏继续信号：void sendGameContinuingSignal();
   * 发送游戏结束信号：void sendGameOverSignal();
   * 发送关闭窗口信号：void sendCloseViewSignal();
   * 选择植物：void selectPlant(int type);
   * 植物攻击：void plantAttack(PlantBase* plant);
   * 移除植物：void plantDelete(PlantBase* plant);
   * 移除子弹：void bulletDelete(Bullet* bullet);
   * 移除僵尸：void zombieDelete(ZombieBase* zombie);
   * 增加阳光：void addSunlight();
   * 更新阳光的显示：void updateSunlight();
   * 添加子弹（投石车僵尸）：void addBullet(QPointF zombiePos);
   * 生产僵尸：void produceZombie();
   * 打印状态（测试用）：void printGameStartState();
   * 打印状态（测试用）：void printGameRunningState();
   * 打印状态（测试用）：void printGamePausingState();
   * 打印状态（测试用）：void printGameOverState();

##### 相关代码

```cpp
#ifndef INTERFACESCENE_H
#define INTERFACESCENE_H

#include <QGraphicsScene>

#include <QRectF>
#include <QTimer>
#include <QLabel>
#include <QPointF>
#include <QPushButton>
#include <QMessageBox>
#include <QGraphicsWidget>
//#include <QtStateMachine/QFinalState>
//#include <QtStateMachine/QStateMachine>
//#include <QtStateMachine/QHistoryState>
#include <QFinalState>
#include <QStateMachine>
#include <QHistoryState>

#include <QSound>
#include <QThread>
#include <QSoundEffect>

#include "plantcard.h"
#include "plantbase.h"

#include "bullet.h"

#include "zombiebase.h"

#define INTERFACE_HEIGHT 600
#define INTERFACE_WIDTH 900
#define STATE_GAMESTART 0
#define STATE_GAMERUNNING 1
#define STATE_GAMEPAUSING 2
#define STATE_GAMEOVER 3

#define PLATFORM_HEIGHT 90
#define PLATFORM_WIDTH 90

#define SUNLIGHT_INCREMENT 25
#define SUNLIGHT_INTERVAL 10050
#define SUNLIGHT_ICON_HEIGHT 40
#define SUNLIGHT_ICON_WIDTH 40

#define TYPE_PLANT 1
#define TYPE_BULLET 2
#define TYPE_ZOMBIE 3
#define TYPE_SPECIAL_GARLIC 4
#define TYPE_SPECIAL_PUMPKIN 5

#define BOX_WIDTH 700
#define BOX_HEIGHT 450

class InterfaceScene : public QGraphicsScene
{
    Q_OBJECT
public:
    InterfaceScene();
    ~InterfaceScene();
public slots:
    void setCoverBackground();
    void setInterfaceBackground();
    void callPauseMessageBox();
    void sendGamePausingSignal();
    void sendGameContinuingSignal();
    void sendGameOverSignal();
    void sendCloseViewSignal();
signals:
    void gameIsOver();
    void gameIsPausing();
    void gameIsContinuing();
    void closeView();
    void clicked();
protected:
    void keyPressEvent(QKeyEvent* event);
private:
    QPushButton gameStartButton;
    QStateMachine machine;
    QState* gameStart, * gameRunning, *gamePausing;
    QFinalState* gameOver;
    QHistoryState* history;
    QMessageBox* pauseMessageBox;

// Part Rects
private:
    QRectF platformsRect, cardsRect, infoRect, sloganRect;

// Plant Cards
private:
    int selectedPlant;
    PlantCard* cards[PLANT_CNT];
public slots:
    void selectPlant(int type);
signals:
    void plantAddedSuccessfully(int type);
public:
    bool addPlant(const QPointF point);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

// Plants and Bullets
public slots:
    void plantAttack(PlantBase* plant);
    void plantDelete(PlantBase* plant);
    void bulletDelete(Bullet* bullet);

// Zombies
public slots:
    void zombieDelete(ZombieBase* zombie);

// Game Info
private:
    int sunlight;
    QTimer sunlightAdder;
    QLabel sunlightDisplay;

    int point;
    QLabel pointDisplay;
public slots:
    void addSunlight();
    void updateSunlight();

// Catapult Zombie
public slots:
    void addBullet(QPointF zombiePos);

// Zombie Producer
private:
    QVector <int> zombieProducer;
    QTimer zombieProducerTick;
public slots:
    void produceZombie();

// Sound Effect
private:
    QSoundEffect* awoogaSoundEffect;
    QSoundEffect* pointsSoundEffect;
    QSoundEffect* setplantSoundEffect;
    QSoundEffect* shoopSoundEffect;
    QSoundEffect* notenoughsunSoundEffect;
    QSoundEffect* kernelpultSoundEffect;
    QSoundEffect* sirenSoundEffect;
    QThread soundEffectThread;
//signals:
//    void sendAwooga();
//    void sendPoints();
//    void sendSetplant();
//    void sendShoop();
//    void sendNotenoughsun();
//    void sendKernelpult();

// Background Music
private:
    QSound* bgmSound;

// Debug Functions
private slots:
    void printGameStartState();
    void printGameRunningState();
    void printGamePausingState();
    void printGameOverState();
protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // INTERFACESCENE_H

```

## 四、功能亮点

* 使用临时字体显示文字，使窗口更加美观；
* 使用GIF来显示植物与僵尸，使画面更加生动；
* 增加音效与背景音乐，使游戏更具可玩性；
* 利用右键菜单实现植物的移除，使得操作更加简洁；
* 增加历史记录积分榜，加强了游戏的竞争性；
* 制作游戏文件安装程序，完善发布过程。

## 五、操作方法

### 1. 游戏开始

游戏加载完成后，单击GAME START游戏开始。

![image-20210605175907479](C:\Users\STRING10\AppData\Roaming\Typora\typora-user-images\image-20210605175907479.png)

### 2. 种植植物

1） 开始游戏后，各植物卡片开始冷却。

![image-20210605180021180](C:\Users\STRING10\AppData\Roaming\Typora\typora-user-images\image-20210605180021180.png)

2）植物冷却完毕后，可以单击植物进行选择，选中后单击其他区域（除草地区域）可以取消选中。

![image-20210605180030987](C:\Users\STRING10\AppData\Roaming\Typora\typora-user-images\image-20210605180030987.png)

3）单击草地中某一地块，如果此时阳光充足，植物种植成功。

![image-20210605180050574](C:\Users\STRING10\AppData\Roaming\Typora\typora-user-images\image-20210605180050574.png)

附，植物花费阳光数一览：

| 植物种类 | 阳光花费 |
| :------: | :------: |
|  向日葵  |  50阳光  |
| 豌豆射手 | 100阳光  |
| 双发射手 | 200阳光  |
| 寒冰射手 | 175阳光  |
|  坚果墙  |  50阳光  |
|  高坚果  | 125阳光  |
|   倭瓜   |  50阳光  |
| 樱桃炸弹 | 150阳光  |
|   大蒜   |  50阳光  |
|  南瓜头  | 125阳光  |

### 3. 移除植物

右键点击植物呼出菜单，点击DELETE移除植物。

![image-20210605184100599](C:\Users\STRING10\AppData\Roaming\Typora\typora-user-images\image-20210605184100599.png)

### 4. 生成僵尸

生成僵尸阶段一览：

1. 分数在0到5内只生成普通僵尸，此时生成僵尸速度为30s/只，生成速度下同；
2. 分数达到5分之后开始增加路障僵尸，其与普通僵尸生成的概率相等，即每30s生成一只普通僵尸或者路障僵尸，生成逻辑下同，即等概率生成每一种僵尸；
3. 分数达到10分之后开始增加读报僵尸和撑杆跳僵尸；
4. 分数达到50之后开始增加小丑僵尸，同时将生成僵尸的间隔修改为10s/只；
5. 分数达到70之后开始生成投石车僵尸。

### 5. 攻击僵尸

1）种植攻击性植物，对僵尸进行攻击。

![image-20210605180707775](C:\Users\STRING10\AppData\Roaming\Typora\typora-user-images\image-20210605180707775.png)

2）击倒僵尸后获得相应分数。

![image-20210605180759382](C:\Users\STRING10\AppData\Roaming\Typora\typora-user-images\image-20210605180759382.png)

附，僵尸分数一览：

|  僵尸种类  | 对应分值 |
| :--------: | :------: |
|  普通僵尸  |   1分    |
|  路障僵尸  |   5分    |
|  读报僵尸  |   7分    |
| 撑杆跳僵尸 |   15分   |
|  小丑僵尸  | 50分/0分 |
| 投石车僵尸 |   70分   |

> 注：小丑僵尸自然引爆时不得分，如果在引爆前击倒小丑僵尸可以获得50分。

### 6. 抵抗入侵

抵抗僵尸的入侵，如果僵尸抵达终点线（游戏左侧的边界），则游戏失败。关闭窗口前将弹出对话框，记录成绩后显示排行榜。

![image-20210605184849063](C:\Users\STRING10\AppData\Roaming\Typora\typora-user-images\image-20210605184849063.png)

### 7. 退出游戏

按空格键呼出对话框，单击“Yes”键以退出游戏，记录成绩并显示排行榜后将关闭窗口。

**注意**：此时游戏并未暂停，单击“No”键可以返回进行中的游戏。

![image-20210605184518150](C:\Users\STRING10\AppData\Roaming\Typora\typora-user-images\image-20210605184518150.png)

### 8. 其他功能

按“0”键关闭背景音乐，再次按“0”键可以开启背景音乐。

## 六、遇到的问题与解决方法

### 1. 使用MFC时遇到的困难

起初对于这次课程设计我选用的是MFC来作为图形化的工具，但是在中途设计植物与僵尸的行为时遇到了障碍，发现MFC似乎不能很好的支持这样类型的实现，于是转战Qt。通过大约三天的学习与实操练习，在大致掌握Qt中QGraphicsItem、QGraphicsScene、QGraphicsView等与本次课程设计紧密相关的类之后，开始着手使用Qt实现本次设计。

### 2. 安装Qt版本不当产生的困难

出于对最新版的盲目追求，在未经了解的情况下选择下载Qt 6.0，但是由于Qt 6版本升级时做出了较大的更改，对于QStateMachine以及QMovie等类的使用与我了解的内容出现较大偏差，尤其是QMovie和QSoundEffect等类甚至在Qt 6.0 中被移除，需要等待Qt 6.2才能再次加入。在寻找替代方案失败后，我重新安装了Qt 5.12，继续未完成的设计。

### 3. 频繁调用音效造成的主线程阻塞

调用QSoundEffect类中的moveToThread方法，将其转移到另外的线程中运行。

### 4. 直接调用本地文件失败

选择Qt的资源文件，将要调用的字体、音效等文件添加到资源文件即可很方便的使用本地文件。
