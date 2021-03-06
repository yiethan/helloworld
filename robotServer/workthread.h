#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>

class WorkThread : public QThread
{
    Q_OBJECT
private:
    int number;
public:
    explicit WorkThread(QObject *parent = 0);
    ~WorkThread();
protected:
    void run();
signals:
    void mergeDatabase(int number);

};

#endif // WORKTHREAD_H
