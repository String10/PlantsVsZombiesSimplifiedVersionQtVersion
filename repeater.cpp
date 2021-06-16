#include "repeater.h"

#include <QPainter>

Repeater::Repeater()
{
    plantType = PLANTTYPE_REPEATER;
    hitPoint = REPEATER_HITPOINT;
    trigger.setInterval(REPEATER_RECHARGETIME);
    doubleTrigger.setInterval(REPEATER_DOUBLERECHARGETIME);
    doubleTrigger.setSingleShot(true);

    QObject::connect(&doubleTrigger, SIGNAL(timeout()), this, SLOT(sendAttackSign()));

    // Gif Display
    plantGIF.setFileName(":/gifs/repeater");
    plantGIF.start();
    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));
    repainter.start();

    trigger.start();
}
void Repeater::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::green);
//    painter->drawRect(0, 0, PLANT_WIDTH, PLANT_HEIGHT);
    if(plantGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(15, 15, -15, -5);
        painter->drawImage(bound, plantGIF.currentImage());
    }
}
void Repeater::sendAttackSign()
{
    static bool isFirst = true;
    emit attack(this);
    if(isFirst) {
        isFirst = false;
        doubleTrigger.start();
    }
    else {
        isFirst = true;
    }
}
