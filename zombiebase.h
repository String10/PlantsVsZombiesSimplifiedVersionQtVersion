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
