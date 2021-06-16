#include "newspaperzombie.h"

#include <QPainter>

NewspaperZombie::NewspaperZombie()
{
    zombieType = ZOMBIETYPE_NEWSPAPER;
    hitPoint = NEWSPAPERZOMBIE_HITPOINT;

    trigger.setInterval(NEWSPAPERZOMBIE_RECHARGETIME);

    // Gif Display
    zombieGIF.setFileName(":/gifs/newspaperzombiewalk0");
    zombieGIF.start();

    tick.start();
}
void NewspaperZombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::gray);
//    painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);
//    if(hitPoint >= NEWSPAPERZOMBIE_HITPOINT * (4.0 / 7)) {
//        painter->setBrush(Qt::white);
//        painter->drawRect(0, 30, 10, 40);
//    }

    if(zombieGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(-25, -15, 25, 0);
        painter->drawImage(bound, zombieGIF.currentImage());
    }

    if(isFrozen) {
        painter->setBrush(QColor(0, 0, 255, 50));
        painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);
    }
}
void NewspaperZombie::reduceLife(int damage)
{
    hitPoint -= damage;
    if(hitPoint < NEWSPAPERZOMBIE_HITPOINT * (4.0 / 7)
            && tick.interval() != NEWSPAPERZOMBIE_ANGRYTICK) {
        tick.setInterval(NEWSPAPERZOMBIE_ANGRYTICK);

        if(zombieGIF.fileName() == ":/gifs/newspaperzombiewalk0") {
            zombieGIF.stop();
            zombieGIF.setFileName(":/gifs/newspaperzombiewalk1");
            zombieGIF.start();
        }
        if(zombieGIF.fileName() == ":/gifs/newspaperzombieattack0") {
            zombieGIF.stop();
            zombieGIF.setFileName(":/gifs/newspaperzombieattack1");
            zombieGIF.start();
        }

    }
    if(hitPoint <= 0) {
        soundEffectThread.terminate();

        emit deleteMe(this);
    }
}
