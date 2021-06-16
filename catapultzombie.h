#ifndef CATAPULTZOMBIE_H
#define CATAPULTZOMBIE_H

#include "zombiebase.h"

#define CATAPULTZOMBIE_HEIGHT 90
#define CATAPULTZOMBIE_WIDTH 150

#define CATAPULTZOMBIE_HITPOINT 800
#define CATAPULTZOMBIE_RECHARGETIME 1400
#define CATAPULTZOMBIE_TICK 50.0 / 9
#define CATAPULTZOMBIE_GRINDATK 18000

class CatapultZombie : public ZombieBase
{
    Q_OBJECT
public:
    CatapultZombie();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget);
    void grind();
public slots:
    void attack();
    void zombieMove();
signals:
    void addBullet(QPointF);
private:
    int bulletCount;
};

#endif // CATAPULTZOMBIE_H
