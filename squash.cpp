#include "squash.h"

#include <QDebug>
#include <QPainter>
#include <QGraphicsScene>
#include <QPropertyAnimation>

#include "zombiebase.h"

Squash::Squash()
{
    plantType = PLANTTYPE_SQUASH;
    hitPoint = SQUASH_HITPOINT;
    search.setInterval(SQUASH_SEARCHINTERVAL);
    QObject::connect(&search, SIGNAL(timeout()), this, SLOT(jump()));

    // Gif Display
    plantGIF.setFileName(":/gifs/squash");
    plantGIF.start();
    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));
    repainter.start();

    search.start();
}
void Squash::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(QColor(201, 220, 135));
//    painter->drawRect(0, 0, PLANT_WIDTH, PLANT_HEIGHT);
    if(plantGIF.state() == QMovie::Running) {
        QRectF bound = boundingRect().adjusted(15, -80, 0, -5);
        painter->drawImage(bound, plantGIF.currentImage());
    }
}
void Squash::jump()
{
    QGraphicsScene* interfaceScene = scene();
    QList <QGraphicsItem*> itemsList = interfaceScene->items();
    QPointF zombiePoint(-1, -1);
    for(int i = 0; i < itemsList.size(); i++)
    {
        if(itemsList[i]->type() == 3
                && itemsList[i]->scenePos().y() < pos().y()
                && itemsList[i]->scenePos().y() + ZOMBIE_HEIGHT > pos().y()
                && itemsList[i]->scenePos().x() > pos().x() - 10
                && itemsList[i]->scenePos().x() < pos().x() + 1.5 * PLANT_WIDTH) {
            zombiePoint = itemsList[i]->scenePos();
            break;
        }
    }

    if(zombiePoint != QPointF(-1, -1)) {
        search.stop();
//        QPropertyAnimation *jump = new QPropertyAnimation(this, "geometry");
//        QObject::connect(jump, SIGNAL(finished()), this, SLOT(sendAttackSign()));
//        jump->setDuration(SQUASH_RECHARGETIME);
//        jump->setKeyValueAt(0, QRectF(pos().x(), pos().y(), PLANT_WIDTH, PLANT_HEIGHT));
//        jump->setKeyValueAt(0.6, QRectF((pos().x() + zombiePoint.x()) / 2, pos().y() + PLANT_HEIGHT * 0.4, PLANT_WIDTH * 0.9, PLANT_HEIGHT * 1.1));
//        jump->setKeyValueAt(1, QRectF(zombiePoint.x() + (ZOMBIE_WIDTH - PLANT_WIDTH) / 2, pos().y(), PLANT_WIDTH, PLANT_HEIGHT));
//        jump->setEasingCurve(QEasingCurve::OutQuart);
//        jump->start(QPropertyAnimation::DeleteWhenStopped);

        jumpTrigger.setInterval(SQUASH_RECHARGETIME);
        jumpTrigger.setSingleShot(true);
        QObject::connect(&jumpTrigger, SIGNAL(timeout()), this, SLOT(sendAttackSign()));

        qDebug() << "Squash Jumped!";
        moveBy((zombiePoint.x() - pos().x()) * 0.7, 0);
        jumpTrigger.start();

        setZValue(3);
    }
}
