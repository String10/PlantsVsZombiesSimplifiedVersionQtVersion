#include "interfacescene.h"

#include <QFile>
#include <QDebug>
#include <QObject>
#include <QPixmap>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
// #include <QtStateMachine/QHistoryState>
#include <QHistoryState>

#include <QSound>
#include <QSoundEffect>

#include <algorithm>

#include "plantcard.h"
#include "sunflower.h"
#include "peashooter.h"
#include "repeater.h"
#include "snowpea.h"
#include "wallnut.h"
#include "tallnut.h"
#include "squash.h"
#include "cherrybomb.h"
#include "garlic.h"
#include "pumpkin.h"

#include "bullet.h"

#include "normalzombie.h"
#include "coneheadzombie.h"
#include "newspaperzombie.h"
#include "polevaultingzombie.h"
#include "jackintheboxzombie.h"
#include "catapultzombie.h"

InterfaceScene::InterfaceScene()
{
    setSceneRect(0, 0,
                  INTERFACE_WIDTH, INTERFACE_HEIGHT);

    // Set GameStart Button
    QFont gameStartButtonFont = font();
    gameStartButtonFont.setPointSize(30);
    gameStartButton.setFont(gameStartButtonFont);
    gameStartButton.setText("GAME START!");
    gameStartButton.move(INTERFACE_WIDTH * 0.65, INTERFACE_HEIGHT * 0.7);
    gameStartButton.setStyleSheet("background-color:rgba(0, 0, 0, 0);color:green");
    gameStartButton.setCursor(Qt::PointingHandCursor);
    addWidget(&gameStartButton);

    // Set State Machine
    gameStart = new QState(&machine);
    gameRunning = new QState(&machine);
    gameOver = new QFinalState(&machine);
    gameRunning->setChildMode(QState::ParallelStates);
    gameStart->addTransition(&gameStartButton, SIGNAL(clicked()), gameRunning);
    gameRunning->addTransition(this, SIGNAL(gameIsOver()), gameOver);
    QObject::connect(gameStart, SIGNAL(entered()), this, SLOT(setCoverBackground()));
    QObject::connect(gameStart, SIGNAL(exited()), this, SLOT(setInterfaceBackground()));
    QObject::connect(&machine, SIGNAL(finished()), this, SLOT(sendCloseViewSignal()));
    machine.setInitialState(gameStart);
    machine.start();

    // Set Pause MessageBox
    pauseMessageBox = new QMessageBox;
    pauseMessageBox->setIcon(QMessageBox::Question);
    pauseMessageBox->setWindowTitle("PAUSE");
    pauseMessageBox->setText("DO YOU REALLY WANT TO QUIT?");
    pauseMessageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    // Set Game State History(Pause)
    history = new QHistoryState(gameRunning);
    history->setDefaultState(new QState(gameRunning));
    gamePausing = new QState(&machine);
    gamePausing->addTransition(this, SIGNAL(gameIsContinuing()), history);
    gameRunning->addTransition(this, SIGNAL(gameIsPausing()), gamePausing);

    // Set Machine State Properties
    gameStart->assignProperty(&machine, "state", STATE_GAMESTART);
    gameRunning->assignProperty(&machine, "state", STATE_GAMERUNNING);
    gamePausing->assignProperty(&machine, "state", STATE_GAMEPAUSING);

    // Plant Cards Settings
    selectedPlant = -1;
    for(int i = 0; i < PLANT_CNT; i++)
    {
        cards[i] = new PlantCard(i);
        QObject::connect(cards[i], SIGNAL(clicked(int)), this, SLOT(selectPlant(int)));
        QObject::connect(this, SIGNAL(plantAddedSuccessfully(int)), cards[i], SLOT(startTimer(int)));

        addItem(cards[i]);
        cards[i]->setPos(CARD_WIDTH * i, INTERFACE_HEIGHT - CARD_HEIGHT);
        cards[i]->setVisible(false);

        QObject::connect(gameRunning, SIGNAL(entered()), cards[i], SLOT(startTimer()));
    }

    // Part Rects Settings
    platformsRect = QRectF(0, 0, CARD_WIDTH * PLANT_CNT - 0.5, INTERFACE_HEIGHT - CARD_HEIGHT - 0.5);
    cardsRect = QRectF(0, INTERFACE_HEIGHT - CARD_HEIGHT, CARD_WIDTH * PLANT_CNT - 0.5, CARD_HEIGHT - 0.5);
    sloganRect = QRectF(CARD_WIDTH * PLANT_CNT, 0, INTERFACE_WIDTH - CARD_WIDTH * PLANT_CNT - 0.5, INTERFACE_HEIGHT - CARD_HEIGHT - 0.5);
    infoRect = QRectF(CARD_WIDTH * PLANT_CNT, INTERFACE_HEIGHT - CARD_HEIGHT, INTERFACE_WIDTH - CARD_WIDTH - 0.5, CARD_HEIGHT - 0.5);

    // Game Info
    sunlight = 50;
    sunlightAdder.setInterval(SUNLIGHT_INTERVAL);
    QObject::connect(&sunlightAdder, SIGNAL(timeout()), this, SLOT(addSunlight()));
    QObject::connect(gameRunning, SIGNAL(entered()), &sunlightAdder, SLOT(start()));
    addWidget(&sunlightDisplay);
    updateSunlight();
    sunlightDisplay.setVisible(false);
    sunlightDisplay.move(infoRect.topLeft().x() + SUNLIGHT_ICON_WIDTH,
                         infoRect.topLeft().y());
    QFont sunlightDisplayFont = font();
    sunlightDisplayFont.setPointSize(20);
    sunlightDisplay.setFont(sunlightDisplayFont);
    sunlightDisplay.setStyleSheet("background-color:rgba(0, 0, 0, 0);color:yellow");
    sunlightDisplay.resize(infoRect.width() - SUNLIGHT_ICON_WIDTH, SUNLIGHT_ICON_HEIGHT);

    point = 0;
    addWidget(&pointDisplay);
    QFont pointDisplayFont = font();
    pointDisplayFont.setPointSize(12);
    pointDisplay.setFont(pointDisplayFont);
    pointDisplay.setStyleSheet("background-color:rgba(0, 0, 0, 0);color:black");
//    pointDisplay.setNum(point);
    pointDisplay.setText(QString("POINT: %1").arg(point));
    pointDisplay.resize(infoRect.width(), infoRect.height() - SUNLIGHT_ICON_HEIGHT);
    pointDisplay.move(infoRect.topLeft().x() + 5,
                      infoRect.topLeft().y() + SUNLIGHT_ICON_HEIGHT - 5);
    pointDisplay.setVisible(false);

    // Zombie Producer
    zombieProducer.push_back(ZOMBIETYPE_NORMAL);
    zombieProducerTick.setInterval(30000);
    QObject::connect(&zombieProducerTick, SIGNAL(timeout()), this, SLOT(produceZombie()));

    // Sound Effect
    awoogaSoundEffect = new QSoundEffect();
    awoogaSoundEffect->setSource(QUrl("qrc:/audio/awooga"));
    awoogaSoundEffect->setLoopCount(1);
    awoogaSoundEffect->moveToThread(&soundEffectThread);
//    QObject::connect(this, SIGNAL(sendAwooga()), awoogaSoundEffect, SLOT(play()));

    pointsSoundEffect = new QSoundEffect();
    pointsSoundEffect->setSource(QUrl("qrc:/audio/points"));
    pointsSoundEffect->setLoopCount(1);
    pointsSoundEffect->moveToThread(&soundEffectThread);
//    QObject::connect(this, SIGNAL(sendPoints()), pointsSoundEffect, SLOT(play()));

    setplantSoundEffect = new QSoundEffect();
    setplantSoundEffect->setSource(QUrl("qrc:/audio/setplant"));
    setplantSoundEffect->setLoopCount(1);
    setplantSoundEffect->moveToThread(&soundEffectThread);
//    QObject::connect(this, SIGNAL(sendSetplant()), setplantSoundEffect, SLOT(play()));

    shoopSoundEffect = new QSoundEffect();
    shoopSoundEffect->setSource(QUrl("qrc:/audio/shoop"));
    shoopSoundEffect->setLoopCount(1);
    shoopSoundEffect->moveToThread(&soundEffectThread);
//    QObject::connect(this, SIGNAL(sendShoop()), shoopSoundEffect, SLOT(play()));

    notenoughsunSoundEffect = new QSoundEffect();
    notenoughsunSoundEffect->setSource(QUrl("qrc:/audio/notenoughsun"));
    notenoughsunSoundEffect->setLoopCount(1);
    notenoughsunSoundEffect->moveToThread(&soundEffectThread);
//    QObject::connect(this, SIGNAL(sendNotenoughsun()), notenoughsunSoundEffect, SLOT(play()));

    kernelpultSoundEffect = new QSoundEffect();
    kernelpultSoundEffect->setSource(QUrl("qrc:/audio/kernelpult"));
    kernelpultSoundEffect->setLoopCount(1);
    kernelpultSoundEffect->moveToThread(&soundEffectThread);
//    QObject::connect(this, SIGNAL(sendKernelpult()), kernelpultSoundEffect, SLOT(play()));

    sirenSoundEffect = new QSoundEffect();
    sirenSoundEffect->setSource(QUrl("qrc:/audio/siren"));
    sirenSoundEffect->setLoopCount(1);
    sirenSoundEffect->moveToThread(&soundEffectThread);

    soundEffectThread.start();

    // Background Music
    bgmSound = new QSound(":/audio/bgm", this);
    bgmSound->setLoops(QSound::Infinite);

    // Debugging
    QObject::connect(gameStart, SIGNAL(entered()), this, SLOT(printGameStartState()));
    QObject::connect(gameRunning, SIGNAL(entered()), this, SLOT(printGameRunningState()));
    QObject::connect(gamePausing, SIGNAL(entered()), this, SLOT(printGamePausingState()));
    QObject::connect(&machine, SIGNAL(finished()), this, SLOT(printGameOverState()));
}
InterfaceScene::~InterfaceScene()
{
    delete gameStart;
    delete gameRunning;
    delete gamePausing;
    delete gameOver;
    delete history;

    delete pauseMessageBox;

    for(int i = 0; i < PLANT_CNT; i++) {
        delete cards[i];
    }

    delete awoogaSoundEffect;
    delete pointsSoundEffect;
    delete setplantSoundEffect;
    delete shoopSoundEffect;
    delete notenoughsunSoundEffect;
    delete kernelpultSoundEffect;
}
void InterfaceScene::setCoverBackground() {
    QPixmap pix(":/imgs/coverBackground");
    QSize picSize(INTERFACE_WIDTH, INTERFACE_HEIGHT);
    setBackgroundBrush(pix.scaled(picSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
void InterfaceScene::setInterfaceBackground() {
    QPixmap pix(":/imgs/interfaceBackground");
    QSize picSize(INTERFACE_WIDTH, INTERFACE_HEIGHT);
    setBackgroundBrush(pix.scaled(picSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    gameStartButton.setVisible(false);
    for(int i = 0; i < PLANT_CNT; i++)
    {
        cards[i]->setVisible(true);
    }

    sunlightDisplay.setVisible(true);
    pointDisplay.setVisible(true);

    zombieProducerTick.start();

    // Siren Sound Effect
    awoogaSoundEffect->play();
//    qDebug() << "Siren!" << awoogaSoundEffect->status();
//    qDebug() << awoogaSoundEffect->source();
//    emit sendAwooga();

    // Background Music
    bgmSound->play();
}
void InterfaceScene::callPauseMessageBox()
{
    if(machine.property("state") == STATE_GAMERUNNING) {
        sendGamePausingSignal();
        int ret = pauseMessageBox->exec();
        switch (ret) {
        case QMessageBox::Yes :
            sendGameContinuingSignal();
            sendGameOverSignal();
            break;
        case QMessageBox::No :
            sendGameContinuingSignal();
            break;
        default:
            break;
        }
    }
}
void InterfaceScene::sendGamePausingSignal()
{
    emit gameIsPausing();
}
void InterfaceScene::sendGameContinuingSignal()
{
    emit gameIsContinuing();
}
void InterfaceScene::sendGameOverSignal()
{
    qDebug() << "Emitting SIGNAL gameIsOver...";
    emit gameIsOver();
}
void InterfaceScene::sendCloseViewSignal()
{
    // Best Players
//    QMessageBox bestPlayersMessageBox;
//    bestPlayersMessageBox.setIcon(QMessageBox::Information);
//    bestPlayersMessageBox.setWindowTitle("BEST PLAYERS");
//    bestPlayersMessageBox.setText("PLEASE ENTER YOUR NAME:");
//    bestPlayersMessageBox.setStandardButtons(QMessageBox::Save);
//    bestPlayersMessageBox.setFixedSize(BOX_WIDTH, BOX_HEIGHT);
//    bestPlayersMessageBox.setTextInteractionFlags(Qt::TextEditorInteraction);

//    int ret = bestPlayersMessageBox.exec();
//    switch (ret) {
//    case QMessageBox::Save:
//        break;
//    default:
//        break;
//    }

    zombieProducerTick.stop();
    QList <QGraphicsItem*> itemsList = items();
    for(int i = 0; i < itemsList.size(); i++) {
        if(itemsList[i]->type() >= 1 && itemsList[i]->type() <= 5) {
            removeItem(itemsList[i]);
//            delete itemsList[i];
        }
    }

    bool isOK;
    QString playerName = QInputDialog::getText(NULL, "RECORDER",
                                               "PLEASE ENTER YOUR NAME:",
                                               QLineEdit::Normal,
                                               "Your Name",
                                               &isOK);
    if(isOK) {
        qDebug() << playerName;
        QFile playersFile("players.txt");
        if(!playersFile.open(QIODevice::Append)) {
            playersFile.open(QIODevice::NewOnly);
        }
        QTextStream players(&playersFile);
//        players << playerName << QString(" %1").arg(point) << Qt::endl;
        players << playerName << QString(" %1").arg(point) << endl;

        playersFile.close();
    }
    QFile playersFile("players.txt");
    if(!playersFile.open(QIODevice::ReadOnly)) {
        playersFile.open(QIODevice::NewOnly);
    }
    QTextStream players(&playersFile);
    QVector <QPair<int, QString>> playersVec;
    while(!players.atEnd()) {
        int playerPointTmp;
        QString playerNameTmp;
        QString str = players.readLine();
//        players >> playerNameTmp >> playerPointTmp;
        if(str == QString("")) {
            continue;
        }
        playerNameTmp = str.left(str.lastIndexOf(' '));
        playerPointTmp = 0;
        for(int i = str.lastIndexOf(' ') + 1; i < str.length(); i++) {
            playerPointTmp = playerPointTmp * 10 + (str[i].toLatin1() - '0');
        }
        playersVec.push_back(qMakePair(playerPointTmp, playerNameTmp));
        qDebug() << playerNameTmp << playerPointTmp;
    }
    std::sort(playersVec.begin(), playersVec.end(),
              [](QPair<int, QString> a, QPair<int, QString> b){
        if(a.first == b.first) {
            return a.second < b.second;
        }
        return a.first > b.first;
    });
    QString playerInfo;
    for(int i = 0; i < 10 && i < playersVec.size(); i++) {
        playerInfo += QString("%1     \t\t%2\n").arg(playersVec[i].second).arg(playersVec[i].first);
    }
    QMessageBox scoreboard;
    scoreboard.setWindowTitle("SCOREBOARD");
    scoreboard.setText(playerInfo);
    scoreboard.addButton(QMessageBox::Ok);

    scoreboard.exec();

    emit closeView();
}
void InterfaceScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Space :
        callPauseMessageBox();
        break;
    case Qt::Key_0 :
        if(bgmSound->isFinished()) {
            bgmSound->play();
        }
        else {
            bgmSound->stop();
        }
    default:
        break;
    }
}

// Plant Cards
void InterfaceScene::selectPlant(int type)
{
    selectedPlant = type;
}
bool InterfaceScene::addPlant(const QPointF point)
{
    int platformX = point.x() / PLATFORM_WIDTH;
    int platformY = point.y() / PLATFORM_HEIGHT;
    qDebug() << "Adding Plant in" << platformX << platformY;

    QRectF platform = QRectF(platformX * PLATFORM_WIDTH + 0.5, platformY * PLATFORM_HEIGHT + 0.5,
                             PLATFORM_WIDTH - 0.5, PLATFORM_HEIGHT - 0.5);
    QPainterPath platformPath;
    platformPath.addRect(platform);
    QTransform platformTransform;
    setSelectionArea(platformPath, platformTransform);
    QList <QGraphicsItem*> itemsInPlatform = selectedItems();
    for(int i = 0; i < itemsInPlatform.size(); i++)
    {
        if(selectedPlant != PLANTTYPE_PUMPKIN
                && (itemsInPlatform[i]->type() == TYPE_PLANT
                    || itemsInPlatform[i]->type() == TYPE_SPECIAL_GARLIC)) {
            return false;
        }
        else if(selectedPlant == PLANTTYPE_PUMPKIN
                && itemsInPlatform[i]->type() == TYPE_SPECIAL_PUMPKIN) {
            return false;
        }
    }

    PlantBase* plant = nullptr;
    switch (selectedPlant) {
    case PLANTTYPE_SUNFLOWER: {
        plant = new SunFlower;
        break;
    }
    case PLANTTYPE_PEASHOOTER: {
        plant = new PeaShooter;
        break;
    }
    case PLANTTYPE_REPEATER:
        plant = new Repeater;
        break;
    case PLANTTYPE_SNOWPEA:
        plant = new SnowPea;
        break;
    case PLANTTYPE_WALLNUT:
        plant = new WallNut;
        break;
    case PLANTTYPE_TALLNUT:
        plant = new TallNut;
        break;
    case PLANTTYPE_SQUASH:
        plant = new Squash;
        break;
    case PLANTTYPE_CHERRYBOMB:
        plant = new CherryBomb;
        break;
    case PLANTTYPE_GARLIC:
        plant = new Garlic;
        break;
    case PLANTTYPE_PUMPKIN:
        plant = new Pumpkin;
        plant->setZValue(1);
        break;
    default:
        break;
    }
    if(plant != nullptr) {
        addItem(plant);
        plant->setPos(platform.topLeft());
        if(selectedPlant == PLANTTYPE_PUMPKIN) {
            plant->moveBy(0, PLANT_HEIGHT / 2);
        }
        QObject::connect(plant, SIGNAL(attack(PlantBase*)), this, SLOT(plantAttack(PlantBase*)));
        QObject::connect(plant, SIGNAL(deleteMe(PlantBase*)), this, SLOT(plantDelete(PlantBase*)));

        setplantSoundEffect->play();
//        emit sendSetplant();
    }

    return true;
}
void InterfaceScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->scenePos();
    qDebug() << platformsRect.contains(event->scenePos())
             << sloganRect.contains(event->scenePos())
             << cardsRect.contains(event->scenePos())
             << infoRect.contains(event->scenePos());
    if(selectedPlant != -1
            && machine.property("state") == STATE_GAMERUNNING
            && platformsRect.contains(event->scenePos())) {
        if(sunlight >= cards[selectedPlant]->getCost()) {
            if(addPlant(event->scenePos())) {
                emit plantAddedSuccessfully(selectedPlant);
                sunlight -= cards[selectedPlant]->getCost();
                updateSunlight();
            }
        }
        else {
            notenoughsunSoundEffect->play();
//            emit sendNotenoughsun();
        }
    }
//    else if(!platformsRect.contains(event->scenePos())) {
//        selectedPlant = -1;
//    }
    selectedPlant = -1;

    QGraphicsScene::mousePressEvent(event);
}

// Plants and Bullets
void InterfaceScene::plantAttack(PlantBase* plant)
{
    switch (plant->getPlantType()) {
    case PLANTTYPE_SUNFLOWER:
        qDebug() << "SunFlower Adds 25 Sunlight!";
        addSunlight();
        break;
    case PLANTTYPE_PEASHOOTER: case PLANTTYPE_REPEATER: {
        QPointF plantPoint = plant->pos();
//        qDebug() << plantPoint << plant->boundingRect().top();
        QList <QGraphicsItem*> itemsList = items();
        for(int i = 0; i < itemsList.size(); i++)
        {
            if(itemsList[i]->type() == TYPE_ZOMBIE
                    && itemsList[i]->scenePos().y() < plantPoint.y()
                    && itemsList[i]->scenePos().y() + ZOMBIE_HEIGHT > plantPoint.y()
                    && itemsList[i]->scenePos().x() > plantPoint.x() + PLANT_WIDTH / 3 * 2) {
                qDebug() << "Peashooter Adds a Bullet!";
                Bullet* normalBullet = new Bullet(BULLETTYPE_NORMAL);
                addItem(normalBullet);
                QObject::connect(normalBullet, SIGNAL(deleteMe(Bullet*)), this, SLOT(bulletDelete(Bullet*)));
                normalBullet->setPos(plantPoint);
                normalBullet->moveBy(PLATFORM_WIDTH / 3 * 2, PLATFORM_HEIGHT / 4);

                shoopSoundEffect->play();
//                emit sendShoop();
                break;
            }
        }
        break;
    }
    case PLANTTYPE_SNOWPEA: {
        QPointF plantPoint = plant->pos();
        QList <QGraphicsItem*> itemsList = items();
        for(int i = 0; i < itemsList.size(); i++)
        {
            if(itemsList[i]->type() == TYPE_ZOMBIE
                    && itemsList[i]->scenePos().y() < plantPoint.y()
                    && itemsList[i]->scenePos().y() + ZOMBIE_HEIGHT > plantPoint.y()
                    && itemsList[i]->scenePos().x() > plantPoint.x() + PLANT_WIDTH / 3 * 2) {
                qDebug() << "SnowPea Adds a Bullet!";
                Bullet* freezingBullet = new Bullet(BULLETTYPE_FREEZING);
                addItem(freezingBullet);
                QObject::connect(freezingBullet, SIGNAL(deleteMe(Bullet*)), this, SLOT(bulletDelete(Bullet*)));
                freezingBullet->setPos(plantPoint);
                freezingBullet->moveBy(PLATFORM_WIDTH / 3 * 2, PLATFORM_HEIGHT / 4);

                shoopSoundEffect->play();
//                emit sendShoop();
                break;
            }
        }
        break;
    }
    case PLANTTYPE_SQUASH: {
        qDebug() << "Squash Attacked in Interface!";
        QPointF plantPoint = plant->scenePos();
        QList <QGraphicsItem*> itemsList = items();
        qDebug() << "Squash Attacked!";
        for(int i = 0; i < itemsList.size(); i++)
        {
            if(itemsList[i]->type() == TYPE_ZOMBIE
                    && itemsList[i]->scenePos().y() < plantPoint.y()
                    && itemsList[i]->scenePos().y() + ZOMBIE_HEIGHT > plantPoint.y()
                    && itemsList[i]->scenePos().x() > plantPoint.x()
                    && itemsList[i]->scenePos().x() < plantPoint.x() + PLANT_WIDTH) {
                ((ZombieBase*)itemsList[i])->reduceLife(SQUASH_ATTACKPOINT);
                qDebug() << "Squash killed a zombie!";
            }
        }
        qDebug() << "Squash Deleted!";
        plantDelete(plant);
        break;
    }
    case PLANTTYPE_CHERRYBOMB: {
        QRectF destroyArea;
        destroyArea.setLeft(plant->scenePos().x() - PLANT_WIDTH > 0 ? plant->scenePos().x() - PLANT_WIDTH : 0);
        destroyArea.setTop(plant->scenePos().y() - PLANT_HEIGHT > 0 ? plant->scenePos().y() - PLANT_HEIGHT : 0);
        destroyArea.setRight(plant->scenePos().x() + PLANT_WIDTH * 2 < platformsRect.right() ? plant->scenePos().x() + PLANT_WIDTH * 2 : plant->scenePos().x() + PLANT_WIDTH);
        destroyArea.setBottom(plant->scenePos().y() + PLANT_HEIGHT * 2 < platformsRect.bottom() ? plant->scenePos().y() + PLANT_HEIGHT * 2 : plant->scenePos().y() + PLANT_HEIGHT);
        QList <QGraphicsItem*> itemsList = items();
        for(int i = 0; i < itemsList.size(); i++)
        {
            if(itemsList[i]->type() == TYPE_ZOMBIE
                    && (destroyArea.contains(QPoint(itemsList[i]->scenePos().x(), itemsList[i]->scenePos().y() + ZOMBIE_HEIGHT))
                        || destroyArea.contains(QPointF(itemsList[i]->scenePos().x() + ZOMBIE_WIDTH, itemsList[i]->scenePos().y() + ZOMBIE_HEIGHT)))) {
                ((ZombieBase*)itemsList[i])->reduceLife(CHERRYBOMB_ATK);
            }
        }
        plantDelete(plant);
        break;
    }
    default:
        break;
    }
}
void InterfaceScene::plantDelete(PlantBase* plant)
{
    removeItem(plant);
    delete plant;
}
void InterfaceScene::bulletDelete(Bullet* bullet)
{
    if(bullet->pos().x() < INTERFACE_BORDER) {
        kernelpultSoundEffect->play();
//        emit sendKernelpult();
    }

    qDebug() << "BULLET DELETE!";
    removeItem(bullet);
    delete bullet;
}

// Zombies
void InterfaceScene::zombieDelete(ZombieBase* zombie)
{
    point += zombie->getPoint();
    pointDisplay.setText(QString("POINT: %1").arg(point));

    if(point >= 5 && zombieProducer.size() == 1) {
        zombieProducer.push_back(ZOMBIETYPE_CONEHEAD);
    }
    if(point >= 10 && zombieProducer.size() == 2) {
        zombieProducer.push_back(ZOMBIETYPE_NEWSPAPER);
        zombieProducer.push_back(ZOMBIETYPE_POLEVAULTING);
    }
    if(point >= 50 && zombieProducer.size() == 4) {
        zombieProducer.push_back(ZOMBIETYPE_JACKINTHEBOX);
    }
    if(point >= 70 && zombieProducer.size() == 5) {
        zombieProducer.push_back(ZOMBIETYPE_CATAPULT);

        sirenSoundEffect->play();
        zombieProducerTick.setInterval(zombieProducerTick.interval() / 2);
    }

    removeItem(zombie);
    delete zombie;
}

// Game Info
void InterfaceScene::addSunlight()
{
    sunlight += SUNLIGHT_INCREMENT;
    updateSunlight();

    pointsSoundEffect->play();
//    emit sendPoints();
}
void InterfaceScene::updateSunlight()
{
    sunlightDisplay.setNum(sunlight);
}

// Catapult Zombie
void InterfaceScene::addBullet(QPointF zombiePoint)
{
    Bullet* bullet = new Bullet(BULLETTYPE_BASKETBALL);
    addItem(bullet);
    bullet->setPos(zombiePoint);
    bullet->moveBy(10, BULLET_HEIGHT * 2);
    bullet->setZValue(3);

    QObject::connect(bullet, SIGNAL(deleteMe(Bullet*)), this, SLOT(bulletDelete(Bullet*)));
}

// Zombie Producer
void InterfaceScene::produceZombie()
{
    ZombieBase* zombie = nullptr;

    srand(time(NULL));
    switch (zombieProducer[rand() % zombieProducer.size()]) {
    case ZOMBIETYPE_NORMAL:
        zombie = new NormalZombie;
        break;
    case ZOMBIETYPE_CONEHEAD:
        zombie = new ConeheadZombie;
        break;
    case ZOMBIETYPE_NEWSPAPER:
        zombie = new NewspaperZombie;
        break;
    case ZOMBIETYPE_POLEVAULTING:
        zombie = new PoleVaultingZombie;
        break;
    case ZOMBIETYPE_JACKINTHEBOX:
        zombie = new JackInTheBoxZombie;
        break;
    case ZOMBIETYPE_CATAPULT:
        zombie = new CatapultZombie;
        QObject::connect(zombie, SIGNAL(addBullet(QPointF)), this, SLOT(addBullet(QPointF)));
        break;
    default:
        break;
    }

    if(zombie != nullptr) {
        addItem(zombie);

        int platformX = 9;
        int platformY = rand() % 6;
        QRectF platform = QRectF(platformX * PLATFORM_WIDTH + 0.5, platformY * PLATFORM_HEIGHT + 0.5,
                                 PLATFORM_WIDTH - 0.5, PLATFORM_HEIGHT - 0.5);
        zombie->setPos(platform.topLeft());
        zombie->moveBy(0, -PLATFORM_HEIGHT / 4);
//        zombie->setZValue(2 + platformY);
        zombie->setZValue(2);
        QObject::connect(zombie, SIGNAL(deleteMe(ZombieBase*)), this, SLOT(zombieDelete(ZombieBase*)));
        QObject::connect(zombie, SIGNAL(gameover()), this, SLOT(sendGameOverSignal()));
    }
}

// Debug
void InterfaceScene::printGameStartState()
{
    qDebug() << "GameStart";
}
void InterfaceScene::printGameRunningState()
{
    qDebug() << "GameRunning";
}
void InterfaceScene::printGamePausingState()
{
    qDebug() << "GamePausing";
}
void InterfaceScene::printGameOverState()
{
    qDebug() << "GameOver";
}
//void InterfaceScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    qDebug() << event->scenePos();

//    QGraphicsScene::mousePressEvent(event);
//}
void InterfaceScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // callPauseMessageBox();
//    CatapultZombie* zombie = new CatapultZombie;
//    addItem(zombie);

//    int platformX = event->scenePos().x() / PLATFORM_WIDTH;
//    int platformY = event->scenePos().y() / PLATFORM_HEIGHT;
//    QRectF platform = QRectF(platformX * PLATFORM_WIDTH + 0.5, platformY * PLATFORM_HEIGHT + 0.5,
//                             PLATFORM_WIDTH - 0.5, PLATFORM_HEIGHT - 0.5);
//    zombie->setPos(platform.topLeft());
//    zombie->moveBy(0, -PLATFORM_HEIGHT / 4);
//    zombie->setZValue(2);
//    QObject::connect(zombie, SIGNAL(deleteMe(ZombieBase*)), this, SLOT(zombieDelete(ZombieBase*)));
//    QObject::connect(zombie, SIGNAL(gameover()), this, SLOT(sendGameOverSignal()));
//    QObject::connect(zombie, SIGNAL(addBullet(QPointF)), this, SLOT(addBullet(QPointF)));
}
