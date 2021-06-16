#include "wallnut.h"

#include <QDebug>
#include <QPainter>

WallNut::WallNut()
{
    plantType = PLANTTYPE_WALLNUT;
    hitPoint = WALLNUT_HITPOINT;

    // Gif Display
    plantGIF.setFileName(":/gifs/wallnut0");
    plantGIF.start();
    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));
    repainter.start();
}
void WallNut::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(QColor(196, 160, 60));
//    painter->drawRect(0, 0, PLANT_WIDTH, PLANT_HEIGHT);
    if(plantGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(15, 15, -15, -5);
        painter->drawImage(bound, plantGIF.currentImage());
    }

    if(hitPoint > WALLNUT_HITPOINT / 3 && hitPoint <= WALLNUT_HITPOINT / 3 * 2
            && plantGIF.fileName() != ":/gifs/wallnut1") {
        qDebug() << plantGIF.fileName();
        plantGIF.stop();
        plantGIF.setFileName(":/gifs/wallnut1");
        plantGIF.start();
    }
    if(hitPoint <= WALLNUT_HITPOINT / 3
            && plantGIF.fileName() != QString(":/gifs/wallnut2")) {
        plantGIF.stop();
        plantGIF.setFileName(":/gifs/wallnut2");
        plantGIF.start();
    }
}
