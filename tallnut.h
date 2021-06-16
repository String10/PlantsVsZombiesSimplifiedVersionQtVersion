#ifndef TALLNUT_H
#define TALLNUT_H

#include "plantbase.h"

#define TALLNUT_HITPOINT 8000

class TallNut : public PlantBase
{
    Q_OBJECT
public:
    TallNut();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget);
    bool isTall() const;
};

#endif // TALLNUT_H
