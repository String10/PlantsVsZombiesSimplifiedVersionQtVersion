#include "peashooter.h"

#include <QPainter>

PeaShooter::PeaShooter()
{
    plantType = PLANTTYPE_PEASHOOTER;
    hitPoint = PEASHOOTER_HITPOINT;
    trigger.setInterval(PEASHOOTER_RECHARGETIME);

    // Gif Display
    plantGIF.setFileName(":/gifs/peashooter");
    plantGIF.start();
    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));
    repainter.start();

    trigger.start();
}
void PeaShooter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::green);
//    painter->drawRect(0, 0, PLANT_WIDTH, PLANT_HEIGHT);
    if(plantGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(15, 15, -15, -5);
        painter->drawImage(bound, plantGIF.currentImage());
    }
}
