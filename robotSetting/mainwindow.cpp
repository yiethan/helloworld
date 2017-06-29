#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myThread = new WorkThread();
    connect(myThread, SIGNAL(updateUI(int)),
            this, SLOT(on_work_thread(int)));

    myThread->start();

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
    for(int i=0;i<MAXBoxNum;i++)
    {
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
    initBoxPointer();
    setComboxBoxValue();
}

MainWindow::~MainWindow()
{
    delete m_boxSetting;
    delete m_settingfile;
    delete ui;

}
void MainWindow::initBoxPointer()
{
    boxPointer[0] = ui->comboBox_1;
    boxPointer[1] = ui->comboBox_2;
    boxPointer[2] = ui->comboBox_3;
    boxPointer[3] = ui->comboBox_4;
    boxPointer[4] = ui->comboBox_5;
    boxPointer[5] = ui->comboBox_6;
    boxPointer[6] = ui->comboBox_7;
    boxPointer[7] = ui->comboBox_8;
    boxPointer[8] = ui->comboBox_9;
    boxPointer[9] = ui->comboBox_10;
    boxPointer[10] = ui->comboBox_11;
    boxPointer[11] = ui->comboBox_12;
    boxPointer[12] = ui->comboBox_13;
    boxPointer[13] = ui->comboBox_14;
    boxPointer[14] = ui->comboBox_15;
    boxPointer[15] = ui->comboBox_16;
    boxPointer[16] = ui->comboBox_17;
    boxPointer[17] = ui->comboBox_18;
    boxPointer[18] = ui->comboBox_19;
    boxPointer[19] = ui->comboBox_20;
    boxPointer[20] = ui->comboBox_21;
    boxPointer[21] = ui->comboBox_22;
    boxPointer[22] = ui->comboBox_23;
    boxPointer[23] = ui->comboBox_24;
    boxPointer[24] = ui->comboBox_25;
    boxPointer[25] = ui->comboBox_26;
    boxPointer[26] = ui->comboBox_27;
    boxPointer[27] = ui->comboBox_28;
    boxPointer[28] = ui->comboBox_29;
    boxPointer[29] = ui->comboBox_30;
    boxPointer[30] = ui->comboBox_31;
    boxPointer[31] = ui->comboBox_32;
    boxPointer[32] = ui->comboBox_33;
    boxPointer[33] = ui->comboBox_34;
    boxPointer[34] = ui->comboBox_35;
    boxPointer[35] = ui->comboBox_36;
}

void MainWindow::setComboxBoxValue()
{
    initBoxPointer();
    for(int i=0;i<MAXBoxNum;i++)
    {
        boxPointer[i]->setCurrentIndex(m_boxSetting[i]-1);
    }
#if 0
    ui->comboBox_1->setCurrentIndex(m_boxSetting[0]);
    ui->comboBox_2->setCurrentIndex(m_boxSetting[1]);
    ui->comboBox_3->setCurrentIndex(m_boxSetting[2]);
    ui->comboBox_4->setCurrentIndex(m_boxSetting[3]);
    ui->comboBox_5->setCurrentIndex(m_boxSetting[4]);
    ui->comboBox_6->setCurrentIndex(m_boxSetting[5]);
    ui->comboBox_7->setCurrentIndex(m_boxSetting[6]);
    ui->comboBox_8->setCurrentIndex(m_boxSetting[7]);
    ui->comboBox_9->setCurrentIndex(m_boxSetting[8]);
    ui->comboBox_10->setCurrentIndex(m_boxSetting[9]);
    ui->comboBox_11->setCurrentIndex(m_boxSetting[10]);
    ui->comboBox_12->setCurrentIndex(m_boxSetting[11]);
    ui->comboBox_13->setCurrentIndex(m_boxSetting[12]);
    ui->comboBox_14->setCurrentIndex(m_boxSetting[13]);
    ui->comboBox_15->setCurrentIndex(m_boxSetting[14]);
    ui->comboBox_16->setCurrentIndex(m_boxSetting[15]);
    ui->comboBox_17->setCurrentIndex(m_boxSetting[16]);
    ui->comboBox_18->setCurrentIndex(m_boxSetting[17]);
    ui->comboBox_19->setCurrentIndex(m_boxSetting[18]);
    ui->comboBox_20->setCurrentIndex(m_boxSetting[19]);
    ui->comboBox_21->setCurrentIndex(m_boxSetting[20]);
    ui->comboBox_22->setCurrentIndex(m_boxSetting[21]);
    ui->comboBox_23->setCurrentIndex(m_boxSetting[22]);
    ui->comboBox_24->setCurrentIndex(m_boxSetting[23]);
    ui->comboBox_25->setCurrentIndex(m_boxSetting[24]);
    ui->comboBox_26->setCurrentIndex(m_boxSetting[25]);
    ui->comboBox_27->setCurrentIndex(m_boxSetting[26]);
    ui->comboBox_28->setCurrentIndex(m_boxSetting[27]);
    ui->comboBox_29->setCurrentIndex(m_boxSetting[28]);
    ui->comboBox_30->setCurrentIndex(m_boxSetting[29]);
#endif
}

void MainWindow::on_work_thread(int num)
{
    //qDebug()<<"recieve message"<<num<<endl;
    ui->verticalSlider->setValue(num);
    return;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug()<<"check setting";
    int samevalue = 0;
    int roomselect = 0;
    for(int j=0;j<MAXBoxNum;j++)
        qDebug()<<"box setting is "<<j<< "value "<<m_boxSetting[j];
    for(roomselect=1;roomselect<MAXRoomNum+1;roomselect++)
    {
        for(int j=0;j<MAXBoxNum;j++)
        {
            if(m_boxSetting[j] == roomselect)
                samevalue++;
            if(samevalue > allowMaxSameBox)
                break;
        }
        if(samevalue > allowMaxSameBox)
            break;
        else
            samevalue = 0;
    }
    if(samevalue > allowMaxSameBox)
    {
        qDebug()<<"room selected is"<<roomselect;
        QString warningstr;
        warningstr = QStringLiteral("科室")+ QString::number((roomselect),10) + QStringLiteral("选择的箱子数超过了2个,请重新设置");
        QMessageBox::warning(NULL,"warning",warningstr);
        return;
    }
    qDebug()<<"saving file";
    m_settingfile->open(QIODevice::WriteOnly);
    m_settingfile->resize(0);
    //write the default setting
    QTextStream txtWrite(m_settingfile);
    for(int i=0; i<MAXBoxNum;i++)
    {
        qDebug()<<"write "<<QString::number(m_boxSetting[i],10)<<"to file";
        txtWrite<<QString::number(m_boxSetting[i],10)<<endl;
        txtWrite.flush();
    }
    m_settingfile->close();
    QCoreApplication::quit();
}

void MainWindow::on_comboBox_1_currentIndexChanged(int index)
{
    m_boxSetting[0] = index+1;
    qDebug()<<"box 1 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    m_boxSetting[1] = index+1;
    qDebug()<<"box 2 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    m_boxSetting[2] = index+1;
    qDebug()<<"box 3 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    m_boxSetting[3] = index+1;
    qDebug()<<"box 4 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_5_currentIndexChanged(int index)
{
    m_boxSetting[4] = index+1;
    qDebug()<<"box 5 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_6_currentIndexChanged(int index)
{
    m_boxSetting[5] = index+1;
    qDebug()<<"box 6 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_7_currentIndexChanged(int index)
{
    m_boxSetting[6] = index+1;
    qDebug()<<"box 7 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_8_currentIndexChanged(int index)
{
    m_boxSetting[7] = index+1;
    qDebug()<<"box 8 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_9_currentIndexChanged(int index)
{
    m_boxSetting[8] = index+1;
    qDebug()<<"box 9 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_10_currentIndexChanged(int index)
{
    m_boxSetting[9] = index+1;
    qDebug()<<"box 10 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_11_currentIndexChanged(int index)
{
    m_boxSetting[10] = index+1;
    qDebug()<<"box 11 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_12_currentIndexChanged(int index)
{
    m_boxSetting[11] = index+1;
    qDebug()<<"box 12 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_13_currentIndexChanged(int index)
{
    m_boxSetting[12] = index+1;
    qDebug()<<"box 13 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_14_currentIndexChanged(int index)
{
    m_boxSetting[13] = index+1;
    qDebug()<<"box 14 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_15_currentIndexChanged(int index)
{
    m_boxSetting[14] = index+1;
    qDebug()<<"box 15 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_16_currentIndexChanged(int index)
{
    m_boxSetting[15] = index+1;
    qDebug()<<"box 16 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_17_currentIndexChanged(int index)
{
    m_boxSetting[16] = index+1;
    qDebug()<<"box 17 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_18_currentIndexChanged(int index)
{
    m_boxSetting[17] = index+1;
    qDebug()<<"box 18 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_19_currentIndexChanged(int index)
{
    m_boxSetting[18] = index+1;
    qDebug()<<"box 19 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_20_currentIndexChanged(int index)
{
    m_boxSetting[19] = index+1;
    qDebug()<<"box 20 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_21_currentIndexChanged(int index)
{
    m_boxSetting[20] = index+1;
    qDebug()<<"box 21 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_22_currentIndexChanged(int index)
{
    m_boxSetting[21] = index+1;
    qDebug()<<"box 22 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_23_currentIndexChanged(int index)
{
    m_boxSetting[22] = index+1;
    qDebug()<<"box 23 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_24_currentIndexChanged(int index)
{
    m_boxSetting[23] = index+1;
    qDebug()<<"box 24 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_25_currentIndexChanged(int index)
{
    m_boxSetting[24] = index+1;
    qDebug()<<"box 25 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_26_currentIndexChanged(int index)
{
    m_boxSetting[25] = index+1;
    qDebug()<<"box 26 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_27_currentIndexChanged(int index)
{
    m_boxSetting[26] = index+1;
    qDebug()<<"box 27 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_28_currentIndexChanged(int index)
{
    m_boxSetting[27] = index+1;
    qDebug()<<"box 28 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_29_currentIndexChanged(int index)
{
    m_boxSetting[28] = index+1;
    qDebug()<<"box 29 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_30_currentIndexChanged(int index)
{
    m_boxSetting[29] = index+1;
    qDebug()<<"box 30 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_31_currentIndexChanged(int index)
{
    m_boxSetting[30] = index+1;
    qDebug()<<"box 31 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_32_currentIndexChanged(int index)
{
    m_boxSetting[31] = index+1;
    qDebug()<<"box 32 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_33_currentIndexChanged(int index)
{
    m_boxSetting[32] = index+1;
    qDebug()<<"box 33 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_34_currentIndexChanged(int index)
{
    m_boxSetting[33] = index+1;
    qDebug()<<"box 34 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_35_currentIndexChanged(int index)
{
    m_boxSetting[34] = index+1;
    qDebug()<<"box 35 setting to keshi "<<index<<endl;
}

void MainWindow::on_comboBox_36_currentIndexChanged(int index)
{
    m_boxSetting[35] = index+1;
    qDebug()<<"box 36 setting to keshi "<<index<<endl;
}

void MainWindow::on_pushButton_2_clicked()
{
    for(int i=0;i<MAXBoxNum;i++)
    {
        m_boxSetting[i] = i+1;
    }
    for(int j=0;j<MAXBoxNum;j++)
        qDebug()<<"box setting is "<<j<< "value "<<m_boxSetting[j];
    setComboxBoxValue();
}
