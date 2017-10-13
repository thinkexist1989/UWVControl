#include "maindialog.h"
#include <QApplication>
#include <nbaseminiappwidget.h>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NBaseMiniAppWidget *loadwidget = new NBaseMiniAppWidget();
    loadwidget->setPixmap(QPixmap(":loadingImage"));
    loadwidget->setFixedWidth(756);
    loadwidget->setFixedHeight(454);
    loadwidget->show();
    QTimer::singleShot(2000, loadwidget,SLOT(deleteLater()));

    MainDialog w;
    QTimer::singleShot(2000, &w,SLOT(show()));
   // w.show();

    return a.exec();
}
