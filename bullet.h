#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsItem>

#include <QTimer>

#include <QMovie>

#define BULLET_SPEED 2
#define BULLET_TICK 10
#define BULLET_ATK 20

#define BULLETTYPE_NORMAL 0
#define BULLETTYPE_FREEZING 1
#define BULLETTYPE_BASKETBALL 2

#define BULLET_HEIGHT 22.5
#define BULLET_WIDTH 22.5

#define INTERFACE_BORDER 900

class Bullet : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Bullet();
    Bullet(int type);
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget);
    QPainterPath shape() const;
    int type() const;
private:
    int bulletType;
    int attackPoint;
    QTimer tick;
public slots:
    virtual void bulletMove();
signals:
    void deleteMe(Bullet*);

// Gif Display
private:
    QMovie bulletGIF;
};

#endif // BULLET_H
