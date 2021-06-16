#ifndef PLANTBASE_H
#define PLANTBASE_H

#include <QGraphicsItem>

#include <QTimer>
#include <QMovie>
#include <QGraphicsSceneContextMenuEvent>

#define PLANT_HEIGHT 90
#define PLANT_WIDTH 90

#define PLANTTYPE_NULL -1
#define PLANTTYPE_SUNFLOWER 0
#define PLANTTYPE_PEASHOOTER 1
#define PLANTTYPE_REPEATER 2
#define PLANTTYPE_SNOWPEA 3
#define PLANTTYPE_WALLNUT 4
#define PLANTTYPE_TALLNUT 5
#define PLANTTYPE_SQUASH 6
#define PLANTTYPE_CHERRYBOMB 7
#define PLANTTYPE_GARLIC 8
#define PLANTTYPE_PUMPKIN 9

class PlantBase : public QObject, public QGraphicsItem
{
    Q_OBJECT
//    Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry)
//    Q_INTERFACES(QGraphicsItem)
public:
    PlantBase();
protected:
    int hitPoint;
    int plantType;
    QTimer trigger;
public:
    QRectF boundingRect() const;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    int type() const;
    int getPlantType() const;
    void reduceLife(int damage);
    virtual bool isTall() const;
signals:
    void attack(PlantBase*);
    void deleteMe(PlantBase*);
public slots:
    virtual void sendAttackSign();
    void sendDeleteMe();
    void repaint();
//private:
//    QRectF geometryRect;
//public:
//    QRectF geometry() const;
//    void setGeometry(const QRectF& value);

// Gif Display
protected:
    QMovie plantGIF;
    QTimer repainter;
};

#endif // PLANTBASE_H
