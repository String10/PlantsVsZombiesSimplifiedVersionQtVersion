#include "zombiebase.h"

#include <plantbase.h>

#include <QDebug>

int zombiePoint[] = {
    1,
    5,
    7,
    15,
    50,
    70,
};

ZombieBase::ZombieBase()
{
    zombieType = ZOMBIETYPE_NULL;

    QObject::connect(&trigger, SIGNAL(timeout()), this, SLOT(attack()));

    tick.setInterval(ZOMBIE_TICK);
    QObject::connect(&tick, SIGNAL(timeout()), this, SLOT(zombieMove()));

    isFrozen = false;
    frozenTime.setInterval(ZOMBIE_FROZENTIME);
    frozenTime.setSingleShot(true);
    QObject::connect(&frozenTime, SIGNAL(timeout()), this, SLOT(melt()));

    // Sound Effect
    for(int i = 0; i < 2; i++) {
        chompSoundEffect[i] = new QSoundEffect;
        chompSoundEffect[i]->setSource(QString("qrc:/audio/chomp%1").arg(i));
        chompSoundEffect[i]->setLoopCount(1);
        chompSoundEffect[i]->moveToThread(&soundEffectThread);
    }

    soundEffectThread.start();
}
ZombieBase::~ZombieBase()
{
    qDebug() << "Zombie Deleted!!!!!!";
    soundEffectThread.terminate();
}
QRectF ZombieBase::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(0 - adjust, 0 - adjust,
                  ZOMBIE_WIDTH + adjust, ZOMBIE_HEIGHT + adjust);
}
int ZombieBase::type() const
{
    return 3;
}
int ZombieBase::getZombieType() const
{
    return zombieType;
}
void ZombieBase::reduceLife(int damage)
{
    qDebug() << "HitPoint reduced by" << damage;
    hitPoint -= damage;

    if(zombieType == ZOMBIETYPE_CONEHEAD
            && hitPoint < 300
            && zombieGIF.fileName() == ":/gifs/coneheadzombie") {
        zombieGIF.stop();
        srand(time(NULL));
        zombieGIF.setFileName(QString(":/gifs/normalzombie%1").arg(rand() % 3));
        zombieGIF.start();
    }
    if(zombieType == ZOMBIETYPE_CONEHEAD
            && hitPoint < 300
            && zombieGIF.fileName() == ":/gifs/coneheadzombieattack") {
        zombieGIF.stop();
        srand(time(NULL));
        zombieGIF.setFileName(":/gifs/normalzombieattack");
        zombieGIF.start();
    }

    if(hitPoint <= 0) {
        qDebug() << "Zombie died!";

        soundEffectThread.terminate();
        emit deleteMe(this);
    }
}
void ZombieBase::attack()
{
    static bool flip = false;
    if(isFrozen) {
        if(flip) {
            flip = false;
            return;
        }
        else {
            flip = true;
        }
    }

    srand(time(NULL));

    QList <QGraphicsItem*> items = collidingItems();
    for(int i = 0; i < items.size(); i++) {
        if((items[i]->type() == 5)
                && items[i]->scenePos().y() > pos().y()) {
            qDebug() << "Zombie ATTACKED!";
            chompSoundEffect[rand() % 2]->play();

            ((PlantBase*)items[i])->reduceLife(ZOMBIE_ATK);
            return;
        }
    }
    for(int i = 0; i < items.size(); i++) {
        if((items[i]->type() == 1 || items[i]->type() == 4)
                && items[i]->scenePos().y() > pos().y()) {
            qDebug() << "Zombie ATTACKED!";
            chompSoundEffect[rand() % 2]->play();

            if(items[i]->type() == 4) {
                srand(time(NULL));
                bool toRight = rand() % 2;
                if(toRight && pos().y() < 0) {
                    toRight = false;
                }
                if(!toRight && pos().y() >= PLANT_HEIGHT * 6) {
                    toRight = true;
                }
                setPos(QPointF(pos().x(), pos().y() + (toRight ? -1 : 1) * ZOMBIE_HEIGHT));
            }
            ((PlantBase*)items[i])->reduceLife(ZOMBIE_ATK);
            return;
        }
    }
}
void ZombieBase::zombieMove()
{
    update();
    static bool flip = false;

    QList <QGraphicsItem*> items = collidingItems();
    for(int i = 0; i < items.size(); i++) {
        if((items[i]->type() == 1 || items[i]->type() == 4 || items[i]->type() == 5)
                && items[i]->scenePos().y() > pos().y()) {
//            ((PlantBase*)(items[i]))->reduceLife(0);
            if(!trigger.isActive()) {
                trigger.start();

                switch (zombieType) {
                case ZOMBIETYPE_NORMAL:
                    zombieGIF.stop();
                    zombieGIF.setFileName(":/gifs/normalzombieattack");
                    zombieGIF.start();
                    break;
                case ZOMBIETYPE_CONEHEAD:
                    zombieGIF.stop();
                    if(hitPoint > 200) {
                        zombieGIF.setFileName(":/gifs/coneheadzombieattack");
                    }
                    else {
                        zombieGIF.setFileName(":/gifs/normalzombieattack");
                    }
                    zombieGIF.start();
                    break;
                case ZOMBIETYPE_NEWSPAPER:
                    zombieGIF.stop();
                    if(hitPoint > 200) {
                        zombieGIF.setFileName(":/gifs/newspaperzombieattack0");
                    }
                    else {
                        zombieGIF.setFileName(":/gifs/newspaperzombieattack1");
                    }
                    zombieGIF.start();
                    break;
                default:
                    break;
                }
            }
            return;
        }
    }
    if(trigger.isActive()) {
        trigger.stop();

        switch (zombieType) {
        case ZOMBIETYPE_NORMAL:
            srand(time(NULL));
            zombieGIF.stop();
            zombieGIF.setFileName(QString(":/gifs/normalzombie%1").arg(rand() % 3));
            zombieGIF.start();
            break;
        case ZOMBIETYPE_CONEHEAD:
            srand(time(NULL));
            zombieGIF.stop();
            if(hitPoint > 200) {
                zombieGIF.setFileName(":/gifs/coneheadzombie");
            }
            else {
                zombieGIF.setFileName(QString(":/gifs/normalzombie%1").arg(rand() % 3));
            }
            zombieGIF.start();
            break;
        case ZOMBIETYPE_NEWSPAPER:
            zombieGIF.stop();
            if(hitPoint > 200) {
                zombieGIF.setFileName(":/gifs/newspaperzombiewalk0");
            }
            else {
                zombieGIF.setFileName(":/gifs/newspaperzombiewalk1");
            }
            zombieGIF.start();
            break;
        default:
            break;
        }
    }

    if(isFrozen) {
        if(!flip) {
            moveBy(ZOMBIE_SPEED, 0);
            flip = true;
        }
        else {
            flip = false;
        }
    }
    else {
        moveBy(ZOMBIE_SPEED, 0);
    }

    if(sceneBoundingRect().right() <= 0) {
        emit gameover();
        emit deleteMe(this);
    }
}
void ZombieBase::melt()
{
    isFrozen = false;
}
void ZombieBase::freeze()
{
    isFrozen = true;
    frozenTime.start();
}
int ZombieBase::getPoint() const
{
    return zombiePoint[zombieType];
}
