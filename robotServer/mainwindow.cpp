#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "workthread.h"

#include<QDataStream>
#include<QByteArray>
#include<QDebug>

#include<QSqlDatabase>
#include<QtSql>
#include "tcpserver.h"
const int PORT = 3000;
const int kBase = 1000;
const int BoxCapbility = 30;
const char *DefinedsocketErrors[100] = {"ConnectionRefusedError",
                                 "RemoteHostClosedError",
                                 "HostNotFoundError",
                                 "SocketAccessError",
                                 "SocketResourceError",
                                 "SocketTimeoutError",
                                 "DatagramTooLargeError",
                                 "NetworkError",
                                 "AddressInUseError",
                                 "SocketAddressNotAvailableError",
                                 "UnsupportedSocketOperationError",
                                 "ProxyAuthenticationRequiredError",
                                 "SslHandshakeFailedError",
                                 "UnfinishedSocketOperationError",
                                 "ProxyConnectionRefusedError",
                                 "ProxyConnectionClosedError",
                                 "ProxyConnectionTimeoutError",
                                 "ProxyNotFoundError",
                                 "ProxyProtocolError",
                                 "UnknownSocketError"
                                };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initFileandBoxSetting();

    m_nConnected = 0;
    m_pConnection = new Connection[10];

    m_tcpserver = new QTcpServer(this);
    m_tcpserver->listen(QHostAddress::Any,PORT);
    ui->textBrowser->setText("listenning incoming request from robot");
    connect(m_tcpserver,SIGNAL(newConnection()), this,SLOT(newConnect()));

    QSqlDatabase database = QSqlDatabase::addDatabase("QODBC");
    database.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                   "SERVER=%1;"
                                   "DATABASE=%2;"
                                   "UID=%3;"
                                   "PWD=%4;").arg("127.0.0.1\\SQLEXPRESS")
                           .arg("robot")
                           .arg("sa")
                           .arg("mysql"));
    qDebug()<<"port is "<<database.port();
    database.setPort(1433);
    qDebug()<<"port is "<<database.port();

    if(!database.open())
    {
    qDebug()<<database.lastError();
    qDebug()<<"failed to connect.";
    ui->textBrowser->setTextColor(QColor(255,0,0));
    ui->textBrowser->append("failed to open database, please check system settings");
    }
    else
    {
        qDebug()<<"ok to connect local database.";
        myThread = new WorkThread();
        connect(myThread, SIGNAL(mergeDatabase(int)),
                    this, SLOT(on_database_merge(int)));
        myThread->start();
    }
}
int MainWindow::checkDestinationBoxNumber(int keshiNumber,int boxseting1, int boxsetting2, int boxsetting3)
{
    int boxvaule[2] = {0};
    bool boxexist[2]={false};
    int boxIndex = 0;
    int validboxNum = 0;

    if((keshiNumber > 36)||(keshiNumber<1))
        return -1;
    qDebug()<<"input value is"<<keshiNumber;
    for(int i=0;i<MAXBoxNum;i++)
        qDebug()<<"box "<<i+1 <<"setting to keshi "<<m_boxSetting[i];
    for(int i=0;i<MAXBoxNum;i++)
    {
        if(m_boxSetting[i] == keshiNumber)
        {
            boxvaule[boxIndex++] = i+1;
            qDebug()<<"box value is"<<boxvaule[boxIndex-1];
        }
        if(boxIndex >= 2)
            break;
    }
    qDebug()<<"boxIndex is"<<boxIndex;
    for(int i=0;i<boxIndex;i++)
    {
        if(boxvaule[i] < 13)
            boxexist[i] = (boxseting1 &(0x01<<(boxvaule[i]-1)))?true:false;
        if((boxvaule[i] > 12) && (boxvaule[i] < 25))
            boxexist[i] = (boxsetting2 &(0x01<<(boxvaule[i]-1)))?true:false;
        if((boxvaule[i] > 24) && (boxvaule[i] < 37))
            boxexist[i] = (boxsetting3 &(0x01<<(boxvaule[i]-1)))?true:false;
        if(boxexist[i] == true)
            validboxNum++;
        qDebug()<<"box exist "<<i<<" is "<<boxexist[i];
    }
    int ret = -1;
    int contentA = m_boxContent[boxvaule[0]];
    int contentB = m_boxContent[boxvaule[1]];
    qDebug()<<"content is "<<contentA<<" and "<<contentB;
    switch(validboxNum)
    {
    case 1:
        qDebug()<<"in case 1";
        if(contentA < BoxCapbility)
        {
            ret = boxvaule[0];
        }
        else
        {
            ui->textBrowser->setTextColor(QColor(255,0,0));
            ui->textBrowser->append(QStringLiteral("第")+QString::number(boxvaule[0],10)+QStringLiteral("号箱子已满"));
            ui->textBrowser->setTextColor(QColor(0,0,0));
        }
        break;
    case 2:
        qDebug()<<"in case 2";
        if(( contentA < BoxCapbility) && (contentB < BoxCapbility))
        {
            qDebug()<<"in case 2 1";
            ret = (contentA >= contentB) ? (boxvaule[0]) : (boxvaule[1]);
            break;
        }
        else if(( contentA >= BoxCapbility) && (contentB < BoxCapbility))
        {
            qDebug()<<"in case 2 2";
            ret = boxvaule[1];
            break;
        }
        else if(( contentA < BoxCapbility) && (contentB >= BoxCapbility))
        {
            qDebug()<<"in case 2 3";
            ret = boxvaule[0];
            break;
        }
        else
        {
            qDebug()<<"in case 2 4";
            ui->textBrowser->setTextColor(QColor(255,0,0));
            ui->textBrowser->append(QStringLiteral("第")+QString::number(boxvaule[0],10)+QStringLiteral("号和")+QString::number(boxvaule[1],10)+QStringLiteral("号箱子已满"));
            ui->textBrowser->setTextColor(QColor(0,0,0));
            break;
        }
        break;
    default:
        break;
    }
    return ret;
}

void MainWindow::initFileandBoxSetting()
{
    m_settingfile = new QFile("./customerSettings.ini");
    if(!m_settingfile->exists())
    {
        qDebug()<<"file is not existing, create it with default setting";
        m_settingfile->open(QIODevice::WriteOnly);
        //write the default setting
        QTextStream txtWrite(m_settingfile);
        for(int i=0; i<MAXBoxNum;i++)
        {
            txtWrite<<QString::number((i+1),10)<<endl;
            txtWrite.flush();
        }
        m_settingfile->close();
    }
    m_boxSetting = new int[MAXBoxNum];
    m_boxContent = new int[MAXBoxNum];

    for(int i=0;i<MAXBoxNum;i++)
    {
        m_boxContent[i]=0;
        m_boxSetting[i]=i+1;
        qDebug()<<"default box setting "<<i+1<<" is "<<m_boxSetting[i]<<endl;
    }
    if(!m_settingfile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"Can't open the file!,use the default setting"<<endl;
    }
    else
    {
        int linenumber = 0;
        while(!m_settingfile->atEnd()) {
            if(linenumber > MAXBoxNum-1)//we only support MAXBoxNum
                break;
            QByteArray line = m_settingfile->readLine();
            QString str(line);
            int keshicode = str.toInt();
            if((keshicode > 0)&&(keshicode < MAXRoomNum))
            {
                m_boxSetting[linenumber]=keshicode;
                qDebug()<<"box setting (line) "<<linenumber+1<<" is "<<m_boxSetting[linenumber]<<endl;
            }
            linenumber++;
            qDebug()<< str<<endl;
        }
        m_settingfile->close();
    }

}

void MainWindow::on_database_merge(int num)
{
    qDebug()<<"num is"<<num<<endl;
    QSqlQuery query;

    query.exec("select * from QRtable");
    while (query.next())
    {
         qDebug() << query.value(0).toString() << query.value(1).toString() << query.value(2).toString();
    }
    query.clear();

    bool isok = false;
    isok=query.exec("merge into QRtable as t\nusing MYTEST.robot.dbo.test as s\non t.string=s.id\nwhen not matched\nthen insert values(s.id,s.room,s.size);");

    //isok=query.exec("merge into QRtable as t\nusing 127.0.0.1\\SQLEXPRESS.robot.dbo.test as s\non t.string=s.id\nwhen not matched\nthen insert values(s.id,s.room,s.size);");
    //isok=query.exec("merge into QRtable as t\nusing test as s\non t.string=s.id\nwhen not matched\nthen insert values(s.id,s.room,s.size);");
    qDebug()<<"is Ok ="<<isok;
    query.exec("select * from QRtable");
    while (query.next())
    {
         qDebug() << query.value(0).toString() << query.value(1).toString() << query.value(2).toString();
    }
}
MainWindow::~MainWindow()
{
    database.close();
    delete ui;
}
void MainWindow::newConnect()
{
    //find one connection to store the new connection
        //if the connection is 10, close the server
        QTcpSocket *newtcpsocket = m_tcpserver->nextPendingConnection();
        if (m_nConnected>=10)
        {

            newtcpsocket->close();
            qDebug()<<"Maximum connection (10) reached, new connection will be refused......";
            return;
        }
        int i=0;
        for (i=0; i<10; i++)
        {
            if (m_pConnection[i].m_nSocketID == -1)
            {
                break;
            }
        }

        //add new connection
        m_mutexConnection.lock();
        (m_pConnection[i]).InitializeConnection(newtcpsocket, m_nID);
        connect(m_pConnection[i].m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
        connect(m_pConnection[i].m_pTcpSocket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
        m_mutexConnection.unlock();
        //set the flags
        m_nID++;
        m_nConnected++;
}
void MainWindow::OnDisconnected()
{
    //get the connection that disconnected from the server
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(this->sender());
    Connection* connection = qobject_cast<Connection*>(socket->parent());

    //delete the connection
    m_mutexConnection.lock();
    connection->DeleteConnection();
    m_mutexConnection.unlock();
    if(m_nConnected>0)
        m_nConnected--;
}
void MainWindow::freshBoxContent(int boxset1, int boxset2, int boxset3)
{
    for(int i=0;i<12;i++)
    {
        if(!(boxset1 & (0x01<<i)))
            m_boxContent[i] = 0;
        if(!(boxset2 & (0x01<<i)))
            m_boxContent[i+12] = 0;
        if(!(boxset3 & (0x01<<i)))
            m_boxContent[i+24] = 0;
    }
}

void MainWindow::readMessage()
{
    QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());
    Connection* connection = qobject_cast<Connection*>(newtcpsocket->parent());

    QString res = connection->m_pTcpSocket->readAll();
    ui->textBrowser->append("receive data:  " + res);
    QString qrid = res.section(',',0,0);
    int boxset1 = res.section(',',1,1).toInt();
    int boxset2 = res.section(',',2,3).toInt();
    int boxset3 = res.section(',',2,3).toInt();
    boxset1 &= 0xFFF;
    boxset2 &= 0xFFF;
    boxset3 &= 0xFFF;

    qDebug()<<res;
    qDebug()<<qrid;
    qDebug()<<boxset1;
    qDebug()<<boxset2;
    qDebug()<<boxset3;

    freshBoxContent(boxset1, boxset2, boxset3);

    QSqlQuery query;
    //QString qstring = "select * from QRtable where QRtable.string like "+res;
    QString qstring = "select * from QRtable where QRtable.string="+qrid;

    qDebug()<<qstring;
    bool value = query.exec(qstring);
    qDebug()<<"bool value is"<<value;
    int quriedRoomNum = -1;
    int quriedSize = -1;
    if(value)
    {
        if(query.next())
        {
             qDebug() << query.value(0).toString() << query.value(1).toString() << query.value(2).toString();
             quriedSize =query.value(2).toInt();
             quriedRoomNum = query.value(1).toInt();
        }
    }
    int returnBoxNum =checkDestinationBoxNumber(quriedRoomNum,boxset1,boxset2,boxset3);
    qDebug()<<"return box number is "<<returnBoxNum;
    if(returnBoxNum>0 && returnBoxNum <37)
    {
        m_boxContent[returnBoxNum]++;
        qDebug()<<"box has "<<m_boxContent[returnBoxNum] << "in it";
    }
    else
    {
        returnBoxNum = -1;
        quriedSize = -1;
    }
    QString sendto =QString::number(returnBoxNum,10)+','+QString::number(quriedSize,10);
    connection->m_pTcpSocket->write(sendto.toStdString().data());


    ui->textBrowser->append("send data:  " + sendto);
}
#if 0
void MainWindow::readMessage()
{

    QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());
    Connection* connection = qobject_cast<Connection*>(newtcpsocket->parent());

    QString res = "";

    res = connection->m_pTcpSocket->readAll();

    QString clientip = connection->m_pTcpSocket->peerAddress().toString();



    qDebug() << "user " << clientip << "sendText: " << res;
    qDebug()<<"readData()"<<"IP"<<newtcpsocket->peerAddress().toString()<<newtcpsocket->peerPort();
    ui->textBrowser->append(newtcpsocket->peerAddress().toString());
    ui->textBrowser->append("received data:");
    ui->textBrowser->append(res);
    qDebug()<<res;

    QSqlQuery query;
    //QString qstring = "select * from QRtable where QRtable.string like "+res;
    QString qstring = "select * from QRtable where QRtable.string="+res;

    qDebug()<<qstring;
    bool value = query.exec(qstring);
    qDebug()<<"bool value is"<<value;
    QString sendto ="-1,-1";
    if(value)
    {
        if(query.next())
        {
             qDebug() << query.value(0).toString() << query.value(1).toString() << query.value(2).toString();
             sendto = query.value(1).toString()+","+query.value(2).toString();
        }
    }
    int returnBoxNum =checkDestinationBoxNumber(query.value(1).toInt(),0,0,0);
    if(returnBoxNum>0 && returnBoxNum <37)
        m_boxContent[returnBoxNum]++;
    ui->textBrowser->append("send data:");
    ui->textBrowser->append(sendto);
    connection->m_pTcpSocket->write(sendto.toStdString().data());
}
#endif
void MainWindow::mergeDataBase()
{

}

void MainWindow::sendMessage()
{

    //get the connection that disconnected from the server
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(this->sender());
    Connection* connection = qobject_cast<Connection*>(socket->parent());

    connection->m_pTcpSocket->write("receive data");

}
