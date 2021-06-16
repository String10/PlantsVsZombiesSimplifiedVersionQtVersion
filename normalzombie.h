#ifndef NORMALZOMBIE_H
#define NORMALZOMBIE_H

#include "zombiebase.h"

#define NORMALZOMBIE_HITPOINT 200
//#define NORMALZOMBIE_ATK 100
#define NORMALZOMBIE_RECHARGETIME 1000

class NormalZombie : public ZombieBase
{
    Q_OBJECT
public:
    NormalZombie();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget);
//public slots:
//    void attack();
//    void zombieMove();
//signals:
//    void gameover();
};

#endif // NORMALZOMBIE_H
