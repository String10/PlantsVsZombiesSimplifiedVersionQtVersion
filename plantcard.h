#ifndef PLANTCARD_H
#define PLANTCARD_H

#include <QGraphicsItem>

#include <QTimer>
#include <QFocusEvent>

#include <QThread>
#include <QSoundEffect>

#define CARD_HEIGHT 60
#define CARD_WIDTH 81

#define PLANT_CNT 10

class PlantCard : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    PlantCard();
    PlantCard(int);
    int getCost() const;
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget);
public slots:
    void startTimer();
    void startTimer(int type);
    void getReady();
    void repaint();
signals:
    void clicked(int type);
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void focusOutEvent(QFocusEvent* event);
private:
    int plantType;
    bool isReady;
    QTimer coolDownTimer;
    QTimer repainter;

// Sound Effect
private:
    QSoundEffect seedliftSoundEffect;
    QSoundEffect notenoughsunSoundEffect;
    QThread soundEffectThread;
signals:
    void sendSeedlift();
    void sendNotenoughsun();
};

#endif // PLANTCARD_H
