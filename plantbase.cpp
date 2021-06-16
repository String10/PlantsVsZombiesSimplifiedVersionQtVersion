#include "plantbase.h"

#include <QRectF>
#include <QMenu>

PlantBase::PlantBase()
{
    plantType = PLANTTYPE_NULL;

    QObject::connect(&trigger, SIGNAL(timeout()), this, SLOT(sendAttackSign()));

    setFlag(QGraphicsItem::ItemIsSelectable);

//    qreal adjust = 0.5;
//    geometryRect = QRectF(0 - adjust, 0 - adjust,
//                  PLANT_WIDTH + adjust, PLANT_HEIGHT + adjust);
}
QRectF PlantBase::boundingRect() const
{
//    return geometryRect;
    qreal adjust = 0.5;
    return QRectF(0 - adjust, 0 - adjust,
                  PLANT_WIDTH + adjust, PLANT_HEIGHT + adjust);
}
void PlantBase::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction* deleteAction = menu.addAction("DELETE");

    QAction* selectedAction = menu.exec(event->screenPos());
    if(selectedAction == deleteAction) {
        sendDeleteMe();
    }
}
int PlantBase::type() const
{
    return 1;
}
int PlantBase::getPlantType() const
{
    return plantType;
}
void PlantBase::reduceLife(int damage)
{
    hitPoint -= damage;
    if(hitPoint <= 0) {
        sendDeleteMe();
    }
}
bool PlantBase::isTall() const
{
    return false;
}
void PlantBase::sendAttackSign()
{
    emit attack(this);
}
void PlantBase::sendDeleteMe()
{
    emit deleteMe(this);
}
void PlantBase::repaint()
{
    update();
}
//QRectF PlantBase::geometry() const
//{
//    return geometryRect;
//}
//void PlantBase::setGeometry(const QRectF &value)
//{
//    if(geometryRect != value) {
//        geometryRect = value;
//        update(geometryRect);
//    }
//}
