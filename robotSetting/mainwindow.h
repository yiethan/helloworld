#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<Qdebug>
#include<Qfile>
#include<QComboBox>
#include "workthread.h"

namespace Ui {
class MainWindow;
}

const int MAXBoxNum=36;
const int MAXRoomNum=36;
const int allowMaxSameBox=2;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void initBoxPointer();
    void setComboxBoxValue();
private slots:
    void on_work_thread(int num);

    void on_pushButton_clicked();

    void on_comboBox_1_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

    void on_comboBox_5_currentIndexChanged(int index);

    void on_comboBox_6_currentIndexChanged(int index);

    void on_comboBox_7_currentIndexChanged(int index);

    void on_comboBox_8_currentIndexChanged(int index);

    void on_comboBox_9_currentIndexChanged(int index);

    void on_comboBox_10_currentIndexChanged(int index);

    void on_comboBox_11_currentIndexChanged(int index);

    void on_comboBox_12_currentIndexChanged(int index);

    void on_comboBox_13_currentIndexChanged(int index);

    void on_comboBox_14_currentIndexChanged(int index);

    void on_comboBox_15_currentIndexChanged(int index);

    void on_comboBox_16_currentIndexChanged(int index);

    void on_comboBox_17_currentIndexChanged(int index);

    void on_comboBox_18_currentIndexChanged(int index);

    void on_comboBox_19_currentIndexChanged(int index);

    void on_comboBox_20_currentIndexChanged(int index);

    void on_comboBox_21_currentIndexChanged(int index);

    void on_comboBox_22_currentIndexChanged(int index);

    void on_comboBox_23_currentIndexChanged(int index);

    void on_comboBox_24_currentIndexChanged(int index);

    void on_comboBox_25_currentIndexChanged(int index);

    void on_comboBox_26_currentIndexChanged(int index);

    void on_comboBox_27_currentIndexChanged(int index);

    void on_comboBox_28_currentIndexChanged(int index);

    void on_comboBox_29_currentIndexChanged(int index);

    void on_comboBox_30_currentIndexChanged(int index);

    void on_comboBox_31_currentIndexChanged(int index);

    void on_comboBox_32_currentIndexChanged(int index);

    void on_comboBox_33_currentIndexChanged(int index);

    void on_comboBox_34_currentIndexChanged(int index);

    void on_comboBox_35_currentIndexChanged(int index);

    void on_comboBox_36_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    QFile *m_settingfile;

    QComboBox *boxPointer[MAXBoxNum];

    int *m_boxSetting;

    WorkThread *myThread;
};

#endif // MAINWINDOW_H
