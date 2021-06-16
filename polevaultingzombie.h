#ifndef POLEVAULTINGZOMBIE_H
#define POLEVAULTINGZOMBIE_H

#include "zombiebase.h"

#define POLEVAULTINGZOMBIE_HITPOINT 500
#define POLEVAULTINGZOMBIE_RECHARGETIME 1000
#define POLEVAULTINGZOMBIE_RUNNINGTICK 50.0 / 9
#define POLEVAULTINGZOMBIE_WALKINGTICK 10

class PoleVaultingZombie : public ZombieBase
{
    Q_OBJECT
public:
    PoleVaultingZombie();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget);
public slots:
    void zombieMove();
private:
    bool hasPole;
};

#endif // POLEVAULTINGZOMBIE_H
