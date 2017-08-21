#ifndef QSINGLEAPPLICATION_H
#define QSINGLEAPPLICATION_H
#include <QApplication>
#include <QObject>
#include <QLocalServer>
#include <QWidget>


class QSingleApplication : public QApplication
{
    Q_OBJECT
public:
    QSingleApplication(int &argc, char **argv);

    bool isRunning();
    QWidget *w;
private slots:
    // 有新连接时触发
   void _newLocalConnection();

private:
  // 初始化本地连接
   void _initLocalConnection();
     // 创建服务端
     void _newLocalServer();
     // 激活窗口
     void _activateWindow();

     bool _isRunning;                // 是否已經有实例在运行
     QLocalServer *_localServer;     // 本地socket Server
     QString _serverName;            // 服务名称

};

#endif // QSINGLEAPPLICATION_H
