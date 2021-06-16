#include "coneheadzombie.h"

#include <QPainter>

ConeheadZombie::ConeheadZombie()
{
    zombieType = ZOMBIETYPE_CONEHEAD;
    hitPoint = CONEHEADZOMBIE_HITPOINT;

    trigger.setInterval(CONEHEADZOMBIE_RECHARGETIME);

    // Gif Display
    zombieGIF.setFileName(":/gifs/coneheadzombie");
    zombieGIF.start();

    tick.start();
}
void ConeheadZombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::gray);
//    painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);
//    if(hitPoint >= CONEHEADZOMBIE_HITPOINT * 0.2) {
//        painter->setBrush(QColor(221, 121, 0));
//        painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT * 0.3);
//    }

    if(zombieGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(-30, 0, 0, 0);
        painter->drawImage(bound, zombieGIF.currentImage());
    }

    if(isFrozen) {
        painter->setBrush(QColor(0, 0, 255, 50));
        painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);
    }
}
