#include "mainwindow.h"

#include "interfacescene.h"

#include <QApplication>
#include <QFontDatabase>
#include <QGraphicsView>

extern void SetGameStartButton(InterfaceScene* scene);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set Global Font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/chiller");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontName);
    QApplication::setFont(font);

    // Set Icon
    a.setWindowIcon(QIcon(":/icon/icon"));

    InterfaceScene scene;

    // Show Scene
    QGraphicsView view(&scene);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setFixedSize(QSize(INTERFACE_WIDTH, INTERFACE_HEIGHT));
    view.show();

    QObject::connect(&scene, SIGNAL(closeView()), &view, SLOT(close()));

    return a.exec();
}
