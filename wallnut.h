#ifndef WALLNUT_H
#define WALLNUT_H

#include "plantbase.h"

#define WALLNUT_HITPOINT 4000

class WallNut : public PlantBase
{
    Q_OBJECT
public:
    WallNut();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget);
};

#endif // WALLNUT_H
