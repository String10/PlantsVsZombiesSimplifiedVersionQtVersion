#ifndef JACKINTHEBOXZOMBIE_H
#define JACKINTHEBOXZOMBIE_H

#include "zombiebase.h"

#define JACKINTHEBOXZOMBIE_HITPOINT 500
#define JACKINTHEBOXZOMBIE_RECHARGETIME 1000
#define JACKINTHEBOXZOMBIE_TICK 44.0 / 9
#define JACKINTHEBOXZOMBIE_BOMBATK 18000

class JackInTheBoxZombie : public ZombieBase
{
    Q_OBJECT
public:
    JackInTheBoxZombie();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget);
    int getPoint() const;
public slots:
    void boom();
private:
    QTimer bombTrigger;
};

#endif // JACKINTHEBOXZOMBIE_H
