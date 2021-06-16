#include "catapultzombie.h"

#include <QPainter>
#include <QGraphicsScene>

#include "plantbase.h"
#include "bullet.h"

CatapultZombie::CatapultZombie()
{
    zombieType = ZOMBIETYPE_CATAPULT;
    hitPoint = CATAPULTZOMBIE_HITPOINT;

    trigger.setInterval(CATAPULTZOMBIE_RECHARGETIME);
    bulletCount = 20;

    // Gif Display
    zombieGIF.setFileName(":/gifs/catapultzombie");
    zombieGIF.start();

    tick.setInterval(CATAPULTZOMBIE_TICK);
    tick.start();
}
QRectF CatapultZombie::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(0 - adjust, 0 - adjust,
                  CATAPULTZOMBIE_WIDTH + adjust, CATAPULTZOMBIE_HEIGHT + adjust);
}
void CatapultZombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::gray);
//    painter->drawRect(0, 0, CATAPULTZOMBIE_WIDTH, CATAPULTZOMBIE_HEIGHT);

    if(zombieGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(0, 0, 0, 0);
        painter->drawImage(bound, zombieGIF.currentImage());
    }

    if(isFrozen) {
        painter->setBrush(QColor(0, 0, 255, 50));
        painter->drawRect(0, 0, CATAPULTZOMBIE_WIDTH, CATAPULTZOMBIE_HEIGHT);
    }
}
void CatapultZombie::grind()
{
    QList <QGraphicsItem*> items = collidingItems();
    for(int i = 0; i < items.size(); i++) {
        if((items[i]->type() == 1 || items[i]->type() == 4 || items[i]->type() == 5)
                && items[i]->scenePos().y() > pos().y()) {
            ((PlantBase*)items[i])->reduceLife(CATAPULTZOMBIE_GRINDATK);
        }
    }
}
void CatapultZombie::attack()
{
    static bool flip = false;
    if(isFrozen) {
        if(flip) {
            flip = false;
            return;
        }
        else {
            flip = true;
        }
    }

    emit addBullet(pos());

    bulletCount--;
}
void CatapultZombie::zombieMove()
{
    static bool flip = false;

    grind();

    QList <QGraphicsItem*> items = scene()->items();
    for(int i = 0; i < items.size(); i++) {
        if((items[i]->type() == 1 || items[i]->type() == 4 || items[i]->type() == 5)
                && items[i]->scenePos().y() > pos().y()
                && items[i]->scenePos().y() < pos().y() + CATAPULTZOMBIE_HEIGHT
                && bulletCount > 0
                && sceneBoundingRect().right() <= PLANT_WIDTH * 9) {
            if(!trigger.isActive()) {
                trigger.start();
            }
            return;
        }
    }
    if(trigger.isActive()) {
        trigger.stop();
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
