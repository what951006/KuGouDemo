#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QDesktopWidget>

#include "singleApplication/qsingleapplication.h"
#include "mainwindow.h"
#include "dataBase/myDataBase.h"

#undef main
int main(int argc, char *argv[])
{
    QSingleApplication app(argc, argv);
    //connect database
    myDataBase::connectListinfo();
    myDataBase::connectSongInfo();

    if(!app.isRunning())
    {
        QDir dir;
        dir.mkpath("D:/ExcellentMusic");
        dir.mkpath("D:/ExcellentLrc");
        dir.mkpath("D:/ExcellentAlbum");

        app.setStyleSheet("QMenu{background-color: white;border: 1px solid rgb(214,214,214);}"
                        "QMenu::item{height: 28px;padding: 0px 20px 0px 40px;font-size : 12px;color: rgb(102, 102, 102);font-family: 微软雅黑;}"
                        "QMenu::item:focus{padding: -1;}"
                        "QMenu::item:!enabled{color:rgb(150,150,150);}"
                        "QMenu::item:selected{color: white;background-color: rgb(22, 154, 243);}"
                        "QMenu::icon{position: absolute;left: 12px;}"
                        "QMenu::separator{height:1px;background: rgb(209,209,209);margin:4px 0px 4px 0px;}"
                        "QPushButton:focus{padding: -1;}"
                        "QLabel:focus{padding: -1;}");

        mainWindow*pMain=mainWindow::InitInstance();
        app.w=pMain;
        pMain->show();
        pMain->setGeometry((QApplication::desktop()->width() - pMain->width())/2,(QApplication::desktop()->height() - pMain->height())/2,1008,698);//居中显示
        return app.exec();
    }
    return 0;
}
