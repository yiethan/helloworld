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
        number++;
        sleep(600);
        emit mergeDatabase(number);
    }
}
