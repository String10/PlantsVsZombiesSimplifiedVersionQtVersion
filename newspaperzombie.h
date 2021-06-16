#ifndef NEWSPAPERZOMBIE_H
#define NEWSPAPERZOMBIE_H

#include "zombiebase.h"

#define NEWSPAPERZOMBIE_HITPOINT 350
#define NEWSPAPERZOMBIE_RECHARGETIME 1000
#define NEWSPAPERZOMBIE_ANGRYTICK 4

class NewspaperZombie : public ZombieBase
{
    Q_OBJECT
public:
    NewspaperZombie();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget);
    void reduceLife(int damage);
};

#endif // NEWSPAPERZOMBIE_H
