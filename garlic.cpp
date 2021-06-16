#include "garlic.h"

#include <QPainter>

Garlic::Garlic()
{
    plantType = PLANTTYPE_GARLIC;
    hitPoint = GARLIC_HITPOINT;

    // Gif Display
    plantGIF.setFileName(":/gifs/garlic");
    plantGIF.start();
    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));
    repainter.start();
}
void Garlic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(QColor(198, 183, 155));
//    painter->drawRect(0, 0, PLANT_WIDTH, PLANT_HEIGHT);

    if(plantGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(15, 15, -15, -5);
        painter->drawImage(bound, plantGIF.currentImage());
    }
}
int Garlic::type() const
{
    return 4;
}
