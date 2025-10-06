#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << __FUNCTION__ << " " << __LINE__;
    MainWindow w;
#ifdef __ANDROID__
    qDebug() << __FUNCTION__ << " " << __LINE__;
    w.showMaximized();
#else  // __ANDROID__
    qDebug() << __FUNCTION__ << " " << __LINE__;
    w.show();
#endif // __ANDROID__
    return a.exec();
}
