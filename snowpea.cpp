#include "snowpea.h"

#include <QPainter>

SnowPea::SnowPea()
{
    plantType = PLANTTYPE_SNOWPEA;
    hitPoint = SNOWPEA_HITPOINT;
    trigger.setInterval(SNOWPEA_RECHARGETIME);

    // Gif Display
    plantGIF.setFileName(":/gifs/snowpea");
    plantGIF.start();
    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));
    repainter.start();

    trigger.start();
}
void SnowPea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(QColor(108, 218, 231));
//    painter->drawRect(0, 0, PLANT_WIDTH, PLANT_HEIGHT);
    if(plantGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(15, 15, -15, -5);
        painter->drawImage(bound, plantGIF.currentImage());
    }
}
