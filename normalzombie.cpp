#include "normalzombie.h"

#include "plantbase.h"

#include <QPainter>

#include <QDebug>

NormalZombie::NormalZombie()
{
    zombieType = ZOMBIETYPE_NORMAL;
    hitPoint = NORMALZOMBIE_HITPOINT;

    trigger.setInterval(NORMALZOMBIE_RECHARGETIME);

    // Gif Display
    srand(time(NULL));
    zombieGIF.setFileName(QString(":/gifs/normalzombie%1").arg(rand() % 3));
    zombieGIF.start();

    tick.start();
}
void NormalZombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::gray);
//    painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);

    if(zombieGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(-30, 0, 0, 0);
        painter->drawImage(bound, zombieGIF.currentImage());
    }

    if(isFrozen) {
        painter->setBrush(QColor(0, 0, 255, 50));
        painter->drawRect(0, 0, ZOMBIE_WIDTH, ZOMBIE_HEIGHT);
    }
}
//void NormalZombie::attack()
//{
//    static bool flip = false;
//    if(isFrozen) {
//        if(flip) {
//            flip = false;
//            return;
//        }
//        else {
//            flip = true;
//        }
//    }
//    QList <QGraphicsItem*> items = collidingItems();
//    for(int i = 0; i < items.size(); i++) {
//        if((items[i]->type() == 5)
//                && items[i]->scenePos().y() > pos().y()) {
//            qDebug() << "NormalZombie ATTACKED!";
//            ((PlantBase*)items[i])->reduceLife(NORMALZOMBIE_ATK);
//            return;
//        }
//    }
//    for(int i = 0; i < items.size(); i++) {
//        if((items[i]->type() == 1 || items[i]->type() == 4)
//                && items[i]->scenePos().y() > pos().y()) {
//            qDebug() << "NormalZombie ATTACKED!";
//            if(items[i]->type() == 4) {
//                srand(time(NULL));
//                bool toRight = rand() % 2;
//                if(toRight && pos().y() < 0) {
//                    toRight = false;
//                }
//                if(!toRight && pos().y() >= PLANT_HEIGHT * 6) {
//                    toRight = true;
//                }
//                setPos(QPointF(pos().x(), pos().y() + (toRight ? -1 : 1) * ZOMBIE_HEIGHT));
//            }
//            ((PlantBase*)items[i])->reduceLife(NORMALZOMBIE_ATK);
//            return;
//        }
//    }
//}
//void NormalZombie::zombieMove()
//{
//    QList <QGraphicsItem*> items = collidingItems();
//    for(int i = 0; i < items.size(); i++) {
//        if((items[i]->type() == 1 || items[i]->type() == 4 || items[i]->type() == 5)
//                && items[i]->scenePos().y() > pos().y()) {
//            if(!trigger.isActive()) {
//                trigger.start();
//            }
//            return;
//        }
//    }
//    if(trigger.isActive()) {
//        trigger.stop();
//    }
//    moveBy(ZOMBIE_SPEED, 0);
////    qDebug() << sceneBoundingRect().right();
//    if(sceneBoundingRect().right() <= 0) {
//        emit gameover();
//        emit deleteMe(this);
//        qDebug() << "SIGNAL gameover EMITTED!";
//    }
//}
