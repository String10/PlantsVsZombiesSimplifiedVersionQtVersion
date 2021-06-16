#ifndef PUMPKIN_H
#define PUMPKIN_H

#include "plantbase.h"

#define PUMPKIN_HITPOINT 4000

class Pumpkin : public PlantBase
{
    Q_OBJECT
public:
    Pumpkin();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                         QWidget* widget);
    int type() const;
};

#endif // PUMPKIN_H
