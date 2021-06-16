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
