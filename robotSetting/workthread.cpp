#include "workthread.h"

WorkThread::WorkThread(QObject *parent) : QThread(parent)
{
    number = 0;
}
WorkThread::~WorkThread()
{

}
void WorkThread::run()
{
    while(1)
    {
        //开启一个死循环，让number每秒自增1，并通过发送UpdateSignal信号通知主界面更新
        emit updateUI(10*number);
        number++;
        if(number>9)
            number = 0;
        sleep(1);
    }
}

void WorkThread::ResetSlot()
{
    number = 0;
    emit updateUI(number);
}
