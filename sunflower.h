#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include "plantbase.h"

#define SUNFLOWER_HITPOINT 300
#define SUNFLOWER_RECHARGETIME 24000

class SunFlower : public PlantBase
{
    Q_OBJECT
public:
    SunFlower();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget);
};

#endif // SUNFLOWER_H
