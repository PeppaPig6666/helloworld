#include <QtGui>
#include "mainwindow.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));
    MainWindow w;
//    w.setAutoFillBackground(true);
//    QPixmap pixmap(":/png/yihetang.jpg");
//    QPalette pal;
//    pal.setBrush(w.backgroundRole(),QBrush(pixmap));
//    w.setPalette(pal);

    w.show();
    
    return a.exec();
}
