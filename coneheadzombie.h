#ifndef CONEHEADZOMBIE_H
#define CONEHEADZOMBIE_H

#include "zombiebase.h"

#define CONEHEADZOMBIE_HITPOINT 500
#define CONEHEADZOMBIE_RECHARGETIME 1000

class ConeheadZombie : public ZombieBase
{
    Q_OBJECT
public:
    ConeheadZombie();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget);
};

#endif // CONEHEADZOMBIE_H
