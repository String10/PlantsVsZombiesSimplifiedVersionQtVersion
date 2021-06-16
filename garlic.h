#ifndef GARLIC_H
#define GARLIC_H

#include "plantbase.h"

#define GARLIC_HITPOINT 400

class Garlic : public PlantBase
{
    Q_OBJECT
public:
    Garlic();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget);
    int type() const;
};

#endif // GARLIC_H
