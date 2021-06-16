#include "sunflower.h"

#include <QPainter>

SunFlower::SunFlower() : PlantBase()
{
    plantType = PLANTTYPE_SUNFLOWER;
    hitPoint = SUNFLOWER_HITPOINT;
    trigger.setInterval(SUNFLOWER_RECHARGETIME);

    trigger.start();

    // Gif Display
    plantGIF.setFileName(":/gifs/sunflower");
    plantGIF.start();
    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));
    repainter.start();
}
void SunFlower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::yellow);
//    painter->drawRect(0, 0, PLANT_WIDTH, PLANT_HEIGHT);
    if(plantGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(15, 15, -15, -5);
        painter->drawImage(bound, plantGIF.currentImage());
    }

    if(trigger.remainingTime() <= 0.02 * trigger.interval()) {
        painter->setBrush(QColor(255, 204, 0, 50));
        painter->drawEllipse(0, 0, PLANT_WIDTH, PLANT_HEIGHT);
    }
}
