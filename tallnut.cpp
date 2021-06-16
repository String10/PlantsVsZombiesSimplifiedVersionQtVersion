#include "tallnut.h"

#include <QPainter>

TallNut::TallNut()
{
    plantType = PLANTTYPE_TALLNUT;
    hitPoint = TALLNUT_HITPOINT;

    // Gif Display
    plantGIF.setFileName(":/gifs/tallnut0");
    plantGIF.start();
    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));
    repainter.start();
}
void TallNut::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(QColor(196, 160, 60));
//    painter->drawRect(0, 0, PLANT_WIDTH, PLANT_HEIGHT);
    if(plantGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(15, 0, -5, -5);
        painter->drawImage(bound, plantGIF.currentImage());
    }

    if(hitPoint > TALLNUT_HITPOINT / 3 && hitPoint <= TALLNUT_HITPOINT / 3 * 2
            && plantGIF.fileName() != ":/gifs/tallnut1") {
        plantGIF.stop();
        plantGIF.setFileName(":/gifs/tallnut1");
        plantGIF.start();
    }
    if(hitPoint <= TALLNUT_HITPOINT / 3
            && plantGIF.fileName() != QString(":/gifs/tallnut2")) {
        plantGIF.stop();
        plantGIF.setFileName(":/gifs/tallnut2");
        plantGIF.start();
    }
}
bool TallNut::isTall() const
{
    return true;
}
