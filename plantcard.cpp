#include "plantcard.h"

#include <QDebug>
#include <QTimer>
#include <QCursor>
#include <QPainter>

int coolDownTime[] = {
    7500, // SunFlower
    7500, // Peashooter
    7500, // Repeater
    7500, // SnowPea
    30000, // WallNut
    30000, // TallNut
    30000, // Squash
    50000, // CherryBomb
    7500, // Garlic
    30000, // Pumpkin
};
int cost[] {
    50, // SunFlower
    100, // Peashooter
    200, // Repeater
    175, // SnowPea
    50, // WallNut
    125, // TallNut
    50, // Squash
    150, // CherryBomb
    50, // Garlic
    125, // Pumpkin
};

PlantCard::PlantCard()
{
    plantType = -1;
    isReady = false;

    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
}
PlantCard::PlantCard(int type) {
    plantType = type;
    isReady = false;
    coolDownTimer.setInterval(coolDownTime[type]);
    coolDownTimer.setSingleShot(true);
    QObject::connect(&coolDownTimer, SIGNAL(timeout()), this, SLOT(getReady()));

    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);

    repainter.setInterval(10);
    QObject::connect(&repainter, SIGNAL(timeout()), this, SLOT(repaint()));

    // Sound Effect
    seedliftSoundEffect.setSource(QUrl("qrc:/audio/seedlift"));
    seedliftSoundEffect.setLoopCount(1);
    seedliftSoundEffect.moveToThread(&soundEffectThread);

    notenoughsunSoundEffect.setSource(QUrl("qrc:/audio/notenoughsun"));
    notenoughsunSoundEffect.setLoopCount(1);
    notenoughsunSoundEffect.moveToThread(&soundEffectThread);

    soundEffectThread.start();
}
int PlantCard::getCost() const
{
    return cost[plantType];
}
QRectF PlantCard::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(0 - adjust, 0 - adjust,
                  CARD_WIDTH + adjust, CARD_HEIGHT + adjust);
}
void PlantCard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(100, 100, 100, 100)));
//    painter->setBrush(Qt::blue);
//    painter->drawRect(0, 0, CARD_WIDTH, CARD_HEIGHT);

    painter->drawImage(boundingRect(), QImage(QString(":/imgs/plantcard%1").arg(plantType)));

    painter->setBrush(QColor(0, 0, 0, 50));
    painter->drawRect(0, 0,
                      CARD_WIDTH, CARD_HEIGHT * (1.0 * coolDownTimer.remainingTime() / coolDownTimer.interval()));
}
void PlantCard::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::PointingHandCursor);
}
void PlantCard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Card clicked";
    if(isReady) {
        setFocus();
        setOpacity(0.5);
        emit clicked(plantType);

        seedliftSoundEffect.play();
    }
    else {
        notenoughsunSoundEffect.play();
    }
}
void PlantCard::focusOutEvent(QFocusEvent *event)
{
    setOpacity(1.0);
}
void PlantCard::getReady()
{
    qDebug() << plantType << " is Ready!";
    isReady = true;
}
void PlantCard::repaint()
{
    update();
}
void PlantCard::startTimer()
{
    isReady = false;
    coolDownTimer.start();

    repainter.start();
}
void PlantCard::startTimer(int type)
{
    if(type == plantType) {
        startTimer();
    }
}
