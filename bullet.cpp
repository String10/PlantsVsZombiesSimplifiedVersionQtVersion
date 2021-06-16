#include "bullet.h"

#include "plantbase.h"
#include "zombiebase.h"

#include <QList>
#include <QTimer>
#include <QPainter>
#include <QPainterPath>

#include <QDebug>

Bullet::Bullet()
{
    bulletType = BULLETTYPE_NORMAL;
    attackPoint = BULLET_ATK;
    tick.setInterval(BULLET_TICK);
    QObject::connect(&tick, SIGNAL(timeout()), this, SLOT(bulletMove()));

    tick.start();
}
Bullet::Bullet(int type)
{
    qDebug() << "Initializing a bullet in type" << type;
    bulletType = type;
    attackPoint = BULLET_ATK;
    tick.setInterval(BULLET_TICK);
    QObject::connect(&tick, SIGNAL(timeout()), this, SLOT(bulletMove()));

    // Gif Display
    switch (bulletType) {
    case BULLETTYPE_NORMAL:
        bulletGIF.setFileName(":/gifs/peabulletnormal");
        break;
    case BULLETTYPE_FREEZING:
        bulletGIF.setFileName(":/gifs/peabulletfreezing");
        break;
    default:
        break;
    }
    bulletGIF.start();
    bulletGIF.setPaused(true);

    tick.start();
}
QRectF Bullet::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(0 - adjust, 0 - adjust,
                  BULLET_WIDTH + adjust, BULLET_HEIGHT + adjust);
}
void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(bulletGIF.state() == QMovie::Paused) {
        qDebug() << "BULLET RUNNING";
        QRectF bound = boundingRect().adjusted(-BULLET_WIDTH, 0, 0, 0);
        painter->drawImage(bound, bulletGIF.currentImage());
    }

    switch (bulletType) {
//    case BULLETTYPE_NORMAL:
//        painter->setBrush(Qt::green);
//        break;
//    case BULLETTYPE_FREEZING:
//        painter->setBrush(QColor(91, 208, 230));
//        break;
    case BULLETTYPE_BASKETBALL:
        painter->setPen(QPen(QColor(100, 100, 100, 100)));
        painter->setBrush(QColor(221, 121, 0));
        painter->drawEllipse(0, 0, BULLET_WIDTH, BULLET_HEIGHT);
        break;
    default:
        break;
    }
}
QPainterPath Bullet::shape() const
{
    QPainterPath bulletShape;
    bulletShape.addEllipse(0, 0, BULLET_WIDTH, BULLET_HEIGHT);
    return bulletShape;
}
int Bullet::type() const
{
    return 2;
}
void Bullet::bulletMove()
{
    switch (bulletType) {
    case BULLETTYPE_NORMAL: case BULLETTYPE_FREEZING: {
        QList <QGraphicsItem*> items = collidingItems();
        for(int i = 0; i < items.size(); i++)
        {
            if(items[i]->type() == 3) {
                ZombieBase* zombie = (ZombieBase*)items[i];
                if(bulletType == BULLETTYPE_FREEZING) {
                    zombie->freeze();
                }
                zombie->reduceLife(attackPoint);

                emit deleteMe(this);
                return;
            }
        }
        moveBy(BULLET_SPEED, 0);
        if(pos().x() > INTERFACE_BORDER) {
            emit deleteMe(this);
        }
        break;
    }
    case BULLETTYPE_BASKETBALL: {
        QList <QGraphicsItem*> items = collidingItems();
        for(int i = 0; i < items.size(); i++)
        {
            if(items[i]->type() == 1 || items[i]->type() == 4 || items[i]->type() == 5) {
                PlantBase* plant = (PlantBase*)items[i];
                plant->reduceLife(attackPoint);

                emit deleteMe(this);
                return;
            }
        }
        moveBy(-BULLET_SPEED, 0);
        if(pos().x() <= 0) {
            emit deleteMe(this);
        }
        break;
    }
    default:
        break;
    }
}
