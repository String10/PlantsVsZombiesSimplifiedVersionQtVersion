#include "interfacescene.h"

#include <QGraphicsRectItem>

void SetGameStartButton(InterfaceScene* scene) {
    QGraphicsRectItem* gamestart = new QGraphicsRectItem;
    scene->addItem(gamestart);
}
