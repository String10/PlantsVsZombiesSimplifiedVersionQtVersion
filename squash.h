#ifndef SQUASH_H
#define SQUASH_H

#include "plantbase.h"

#define SQUASH_HITPOINT 300
#define SQUASH_ATTACKPOINT 1800
#define SQUASH_RECHARGETIME 600
#define SQUASH_SEARCHINTERVAL 20

class Squash : public PlantBase
{
    Q_OBJECT
public:
    Squash();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget);
public slots:
    void jump();
private:
    QTimer search;
    QTimer jumpTrigger;
};

#endif // SQUASH_H
