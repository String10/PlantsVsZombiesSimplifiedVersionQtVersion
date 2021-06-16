#ifndef SNOWPEA_H
#define SNOWPEA_H

#include "plantbase.h"

#define SNOWPEA_HITPOINT 300
#define SNOWPEA_RECHARGETIME 1400

class SnowPea : public PlantBase
{
    Q_OBJECT
public:
    SnowPea();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget);
};

#endif // SNOWPEA_H
