#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<QtNetwork/QTcpSocket>
#include<QtNetwork/QTcpServer>

#include<QSqlDatabase>
#include<QtSql>

#include "workthread.h"
#include "tcpserver.h"
namespace Ui {
class MainWindow;
}
const int MAXBoxNum=36;
const int MAXRoomNum=36;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    WorkThread *myThread;

    //QTcpSocket *m_tcpsocket;
    QTcpServer *m_tcpserver;

    Connection* m_pConnection;
    QMutex m_mutexConnection;//mutex to protect the array
    int m_nConnected;
    int m_nID;

    QSqlDatabase database;
    void mergeDataBase();

    QFile *m_settingfile;
    int *m_boxSetting;
    int *m_boxContent;
    void initFileandBoxSetting();
    void freshBoxContent(int boxset1, int boxset2, int boxset3);
    int checkDestinationBoxNumber(int keshiNumber,int boxseting1, int boxsetting2, int boxsetting3);

public slots:
    void newConnect();
    void readMessage();
    void sendMessage();
    void OnDisconnected();

    void on_database_merge(int num);
};

#endif // MAINWINDOW_H
