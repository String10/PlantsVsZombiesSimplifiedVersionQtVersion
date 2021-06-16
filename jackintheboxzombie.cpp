#include "jackintheboxzombie.h"

#include <QPainter>
#include <QGraphicsScene>

#include "plantbase.h"

JackInTheBoxZombie::JackInTheBoxZombie()
{
    zombieType = ZOMBIETYPE_JACKINTHEBOX;
    hitPoint = JACKINTHEBOXZOMBIE_HITPOINT;

    int bombRechargeTime;
    srand(time(NULL));
    if(rand() % 100 < 5) {
        bombRechargeTime = 4500 + rand() % 2250;
    }
    else {
        bombRechargeTime = 9250 + rand() % 9750;
    }
    bombTrigger.setInterval(bombRechargeTime);
    bombTrigger.setSingleShot(true);
    QObject::connect(&bombTrigger, SIGNAL(timeout()), this, SLOT(boom()));

    trigger.setInterval(JACKINTHEBOXZOMBIE_RECHARGETIME);

    tick.setInterval(JACKINTHEBOXZOMBIE_TICK);
    tick.start();

    // Gif Display
    zombieGIF.setFileName(":/gifs/jackintheboxzombiewalk");
    zombieGIF.start();

    bombTrigger.start();
}
void JackInTheBoxZombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::gray);
//    painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);
//    painter->setBrush(Qt::red);
//    painter->drawRect(25, 50, 25, 25);

    if(zombieGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(-25, -10, 0, 0);
        painter->drawImage(bound, zombieGIF.currentImage());
    }

    if(isFrozen) {
        painter->setBrush(QColor(0, 0, 255, 50));
        painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);
    }
}
void JackInTheBoxZombie::boom()
{
    QGraphicsScene* platforms = scene();
    QList <QGraphicsItem*> items = platforms->items();
    for(int i = 0; i < items.size(); i++) {
        if((items[i]->type() == 1 || items[i]->type() == 4 || items[i]->type() == 5)
                && abs(items[i]->scenePos().x() - scenePos().x()) < 1.5 * PLANT_WIDTH
                && items[i]->scenePos().y() + PLANT_HEIGHT - scenePos().y() > 0
                && items[i]->scenePos().y() + PLANT_HEIGHT - scenePos().y() <= 2.75 * PLANT_HEIGHT) {
            ((PlantBase*)items[i])->reduceLife(JACKINTHEBOXZOMBIE_BOMBATK);
        }
    }

    emit deleteMe(this);
}
int JackInTheBoxZombie::getPoint() const
{
    if(hitPoint <= 0) {
        return 50;
    }
    return 0;
}
