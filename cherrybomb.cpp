#include "cherrybomb.h"

#include <QPainter>

CherryBomb::CherryBomb()
{
    plantType = PLANTTYPE_CHERRYBOMB;
    hitPoint = CHERRYBOMB_HITPOINT;
    trigger.setInterval(CHERRYBOMB_RECHARGETIME);

    // Gif Display
    plantGIF.setFileName(":/gifs/cherrybomb");
    plantGIF.setSpeed(35);
    plantGIF.start();
    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));
    repainter.start();

    trigger.start();
}
void CherryBomb::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::red);
//    painter->drawRect(0, 0, PLANT_WIDTH, PLANT_HEIGHT);

    if(plantGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(15, 15, -15, -5);
        painter->drawImage(bound, plantGIF.currentImage());
    }
}
