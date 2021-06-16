#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "plantbase.h"

#define PEASHOOTER_HITPOINT 300
#define PEASHOOTER_RECHARGETIME 1400

class PeaShooter : public PlantBase
{
    Q_OBJECT
public:
    PeaShooter();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget);
};

#endif // PEASHOOTER_H
