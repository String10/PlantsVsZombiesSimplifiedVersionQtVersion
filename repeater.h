#ifndef REPEATER_H
#define REPEATER_H

#include "plantbase.h"

#define REPEATER_HITPOINT 300
#define REPEATER_RECHARGETIME 1400
#define REPEATER_DOUBLERECHARGETIME 150

class Repeater : public PlantBase
{
    Q_OBJECT
public:
    Repeater();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget);
public slots:
    void sendAttackSign();
private:
    QTimer doubleTrigger;
};

#endif // REPEATER_H
