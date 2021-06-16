#include "polevaultingzombie.h"

#include <QPainter>

#include "plantbase.h"

PoleVaultingZombie::PoleVaultingZombie()
{
    zombieType = ZOMBIETYPE_POLEVAULTING;
    hitPoint = POLEVAULTINGZOMBIE_HITPOINT;
    hasPole = true;

    trigger.setInterval(POLEVAULTINGZOMBIE_RECHARGETIME);

    // Gif Display
    zombieGIF.setFileName(":/gifs/polevaultingzombierun");
    zombieGIF.start();

    tick.setInterval(POLEVAULTINGZOMBIE_RUNNINGTICK);
    tick.start();
}
void PoleVaultingZombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::gray);
//    painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);
//    if(hasPole) {
//        painter->setBrush(Qt::white);
//        painter->drawRect(35, 25, 10, 40);
//    }

    if(zombieGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(-15, -25, 15, 0);
        painter->drawImage(bound, zombieGIF.currentImage());
    }

    if(isFrozen) {
        painter->setBrush(QColor(0, 0, 255, 50));
        painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);
    }
}
void PoleVaultingZombie::zombieMove()
{
    static bool flip = false;

    QList <QGraphicsItem*> items = collidingItems();
    if(hasPole) {
        for(int i = 0; i < items.size(); i++) {
            if(items[i]->type() == 1 && ((PlantBase*)items[i])->isTall()
                    && items[i]->scenePos().y() > pos().y()) {
                if(!trigger.isActive()) {
                    trigger.start();

                    zombieGIF.stop();
                    zombieGIF.setFileName(":/gifs/polevaultingzombieattack");
                    zombieGIF.start();
                }

                hasPole = false;
                tick.setInterval(POLEVAULTINGZOMBIE_WALKINGTICK);
                update();
                return;
            }
        }
    }
    for(int i = 0; i < items.size(); i++) {
        if((items[i]->type() == 1 || items[i]->type() == 4 || items[i]->type() == 5)
                && items[i]->scenePos().y() > pos().y()) {
            if(hasPole) {
                moveBy(-(ZOMBIE_WIDTH + PLANT_WIDTH + 5), 0);
                hasPole = false;
                tick.setInterval(POLEVAULTINGZOMBIE_WALKINGTICK);

                zombieGIF.stop();
                zombieGIF.setFileName(":/gifs/polevaultingzombiewalk");
                zombieGIF.start();

                return;
            }
            if(!trigger.isActive()) {
                trigger.start();

                zombieGIF.stop();
                zombieGIF.setFileName(":/gifs/polevaultingzombieattack");
                zombieGIF.start();
            }
            return;
        }
    }
    if(trigger.isActive()) {
        trigger.stop();

        zombieGIF.stop();
        zombieGIF.setFileName(":/gifs/polevaultingzombiewalk");
        zombieGIF.start();
    }

    if(isFrozen) {
        if(!flip) {
            moveBy(ZOMBIE_SPEED, 0);
            flip = true;
        }
        else {
            flip = false;
        }
    }
    else {
        moveBy(ZOMBIE_SPEED, 0);
    }

    if(sceneBoundingRect().right() <= 0) {
        emit gameover();
        emit deleteMe(this);
    }
}
