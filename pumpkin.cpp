#include "pumpkin.h"

#include <QPainter>

Pumpkin::Pumpkin()
{
    plantType = PLANTTYPE_PUMPKIN;
    hitPoint = PUMPKIN_HITPOINT;
    // Gif Display
    plantGIF.setFileName(":/gifs/pumpkinhead0");
    plantGIF.start();
    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));
    repainter.start();
}
QRectF Pumpkin::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(0 - adjust, 0 - adjust,
                  PLANT_WIDTH + adjust, PLANT_HEIGHT / 2 + adjust);
}
void Pumpkin::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(QColor(221, 121, 0));
//    painter->drawRect(0, 0, PLANT_WIDTH, PLANT_HEIGHT / 2);

    if(plantGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(5, 0, -5, 0);
        painter->drawImage(bound, plantGIF.currentImage());
    }

    QList <QGraphicsItem*> items = collidingItems();
    for(int i = 0; i < items.size(); i++) {
        if(items[i]->type() == 1 || items[i]->type() == 4) {
            if(plantGIF.fileName() != ":/gifs/pumpkinhead1") {
                plantGIF.stop();
                plantGIF.setFileName(":/gifs/pumpkinhead1");
                plantGIF.start();
            }
            return;
        }
    }
    if(plantGIF.fileName() != ":/gifs/pumpkinhead0") {
        plantGIF.stop();
        plantGIF.setFileName(":/gifs/pumpkinhead0");
        plantGIF.start();
    }
}
int Pumpkin::type() const
{
    return 5;
}
