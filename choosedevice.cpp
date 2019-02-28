#include "choosedevice.h"
#include "ui_choosedevice.h"
#include <QDebug>

chooseDevice::chooseDevice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chooseDevice)
{
    qDebug()<<"constructor chooseDevice";
    ui->setupUi(this);
    connect(this,&chooseDevice::destroyed,
            [=]()
    {
        ResetComoBox();
    });
}

chooseDevice::~chooseDevice()
{
    delete ui;
}

void chooseDevice::appendItem(QString string)
{
    ui->devicecomboBox->addItem(string);
}
void chooseDevice::ResetComoBox()
{
    int tempCount = this->ui->devicecomboBox->count();
    for(int i=tempCount;i>=0;--i)
    {
        this->ui->devicecomboBox->removeItem(i);
    }
}

void chooseDevice::on_devicepushButton_clicked()
{
    ResetComoBox();
    this->close();
}

void chooseDevice::on_pushButton_clicked()
{
    //选定适配器并向主窗口返回所选适配器
    int indexOfChooseAdapter = ui->devicecomboBox->currentIndex();
    //发出监听信号，让主窗口开始监听
    qDebug()<<indexOfChooseAdapter;
    emit startListener(indexOfChooseAdapter);

    this->close();
}
