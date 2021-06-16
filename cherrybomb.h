#ifndef CHERRYBOMB_H
#define CHERRYBOMB_H

#include "plantbase.h"

#define CHERRYBOMB_HITPOINT 300
#define CHERRYBOMB_ATK 1800
#define CHERRYBOMB_RECHARGETIME 1500

class CherryBomb : public PlantBase
{
    Q_OBJECT
public:
    CherryBomb();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget);
};

#endif // CHERRYBOMB_H
