#include "widget.h"
#include "lyric.h"
#include <QApplication>
#include <QScreen>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();

    return a.exec();
}
