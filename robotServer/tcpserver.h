#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMap>
#include <QTimer>

class Connection:public QObject
{
    Q_OBJECT
public:
    int m_nSocketID;//-1 indicate the socket is invalid; else is the global id of the connection
    int m_nMsgReceived;
    int m_nMsgSent;
    QTcpSocket* m_pTcpSocket;
public:
    Connection(){m_nSocketID = -1;}
    ~Connection()
    {
        if (m_pTcpSocket!=NULL)
        {
            delete m_pTcpSocket;
        }
    }
public:
    void InitializeConnection(QTcpSocket* socket, int id)
    {
        m_pTcpSocket = socket;
        m_pTcpSocket->setParent(this);
        m_nMsgReceived = 0;
        m_nMsgSent = 0;
        m_nSocketID = id;
    }
    void DeleteConnection()
    {
        m_nSocketID = -1;
        m_pTcpSocket->disconnect();
    //  delete m_pTcpSocket; 若delete会报错， ？
        m_pTcpSocket = NULL;
    }
};

#endif // TCPSERVER_H
