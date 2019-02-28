#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "package.h"
#include <QDebug>
#include <QFileDialog>
#include <QDialog>
#include <QMessageBox>
#include <QCompleter>
#include <QStringList>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(QString(":/tcp.ico")));
    ui->openFile->setText(QString("打开文件"));
    ui->closeFile->setText(QString("关闭文件"));
    ui->exit->setText(QString("关闭程序"));
    ui->about->setText(QString("TCP编程成员"));
    //
    ui->progressBar->setValue(100);
    choose = new chooseDevice(this);
    listeners =new Listener;
    thread = new QThread(this);
    //设置过滤输入的提示器
    QStringList listOfProtocal;
    listOfProtocal<<"TCP"<<"WSP"<<"ULP"<<"UDP"<<"TLSv1.2"<<"SSDP"<<"MDNS"<<"LLMNR"<<"IPv6"<<"IGMPv2"<<"ICMPv6"<<
                    "HTTP"<<"DNS"<<"DHCPv6"<<"ARP";
    QCompleter *completer = new QCompleter(listOfProtocal,this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->keyWord->setCompleter(completer);
    /**
    Open
    打开文件并读取数据包
    **/
    connect(ui->openFile,&QAction::triggered,
            [=]()
    {
        qDebug()<<"Open Files.";
        QString path = QFileDialog::getOpenFileName(this,"choose package","/home","txt(*.txt)");
        //成功打开数据包则进行后面的分析工作
        if(path.isEmpty()!=true)
        {
            ui->progressBar->setValue(0);
            file.setFileName(path);
            //文件成功打开，继续
            if(file.open(QIODevice::ReadOnly))
            {
                ui->selectedText->setText(QString(path+"已打开"));
                char buf[1024];
                qint64 lineLength;
                do{

                    lineLength = file.readLine(buf, sizeof(buf));
                if (lineLength != -1)
                {
                    //qDebug()<<buf;//已经过测试
                    QString line(buf);
                    Package *package = new Package(line);
                    /**
                      TEST CODE
                    **/
                    //表中插入一行
                    ui->table->insertRow(TotalLineOfPackage);
                    ui->table->setItem(TotalLineOfPackage,0,new QTableWidgetItem(QString("%1").arg(package->Time)));
                    ui->table->setItem(TotalLineOfPackage,1,new QTableWidgetItem(package->Source));
                    ui->table->setItem(TotalLineOfPackage,2,new QTableWidgetItem(package->Destination));
                    ui->table->setItem(TotalLineOfPackage,3,new QTableWidgetItem(package->Protocal));
                    ui->table->setItem(TotalLineOfPackage,4,new QTableWidgetItem(QString("%1").arg(package->Length)));
                    ui->table->setItem(TotalLineOfPackage,5,new QTableWidgetItem(package->Info));
                    ++TotalLineOfPackage;
                    ui->progressBar->setValue(TotalLineOfPackage);
                }
                }while(lineLength != -1);
                ui->progressBar->setValue(100);
                ui->totalPackage->setText(QString("%1").arg(TotalLineOfPackage));
                ui->filterButton->setEnabled(true);
            }
            else
            {
                ui->selectedText->setText(QString("打开文件失败!"));
            }
        }
    });
    /**
     * 效果等用于按下“筛选按钮”，回车快捷键更方便
     * */
    connect(ui->keyWord,&QLineEdit::returnPressed,this,&MainWindow::on_filterButton_clicked);
    /**
     * 重设表格，恢复上一次过滤的改变,下一次过滤继续使用
     * */
    connect(ui->keyWord,&QLineEdit::textEdited,
            [=]()
    {
        for(int i=0;i<TotalLineOfPackage;++i)
        {
            ui->table->setRowHidden(i,false);
        }
    });
    /**
      About
      关于程序
      **/
    connect(ui->about,&QAction::triggered,
            [=]()
    {
       //
        qDebug()<<"Show help message.";
        QMessageBox::about(this,QString::fromLocal8Bit("TCP小组"),QString::fromLocal8Bit("赵鹏 "));
    });
    /**
      Close
      关闭当前打开的数据包文件，并初始化
    **/
    connect(ui->closeFile,&QAction::triggered,
            [=]()
    {
        qDebug()<<"Close current files and reset window.";
        file.close();
        ui->keyWord->setText("");
        ui->detail->clear();
        ui->totalPackage->setText("0");
        ui->selectedText->setText(" ");
        ui->filterButton->setEnabled(false);
        ui->table->reset();
        ui->progressBar->setValue(0);
        for(int i=TotalLineOfPackage;i!=-1;--i)
        {
           ui->table->removeRow(i);
        }
        //包计数器重置
        TotalLineOfPackage=0;
    });
    /**
      Exit
      关闭并退出程序
    **/
    connect(ui->exit,&QAction::triggered,
            [=]()
    {
        listeners->setFlag(false);
        thread->quit();
        thread->wait();
        this->close();
    });
    connect(this,&MainWindow::destroyed,
            [=]()
    {
        listeners->setFlag(false);
        thread->quit();
        thread->wait();
    });
    /**
      单元格选中触发事件
      **/
    connect(ui->table,&QTableWidget::itemClicked,this,&MainWindow::itemSelectedAndShowDetail);
    /**
      开始监听适配器
      **/
    connect(choose,&chooseDevice::startListener,this,&MainWindow::startListing);
    //

    connect(this,&MainWindow::startInListener,listeners,&Listener::ReceiveInListener);
    //线程已经抓到一个包，把该包发出并填充到Table
    connect(listeners,&Listener::OnePackageIsReade,this,&MainWindow::PutPackageIntoTable);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::PutPackageIntoTable(Package *package)
{
    ui->table->insertRow(TotalLineOfPackage);
    ui->table->setItem(TotalLineOfPackage,0,new QTableWidgetItem(QString("%1").arg(package->Time)));
    ui->table->setItem(TotalLineOfPackage,1,new QTableWidgetItem(package->Source));
    ui->table->setItem(TotalLineOfPackage,2,new QTableWidgetItem(package->Destination));
    ui->table->setItem(TotalLineOfPackage,3,new QTableWidgetItem(package->Protocal));
    ui->table->setItem(TotalLineOfPackage,4,new QTableWidgetItem(QString("%1").arg(package->Length)));
    ui->table->setItem(TotalLineOfPackage,5,new QTableWidgetItem(package->Info));
    ++TotalLineOfPackage;
}

void MainWindow::itemSelectedAndShowDetail(QTableWidgetItem *item)
{
    QString text = item->text();
    //qDebug()<<text;
    ui->selectedText->setText(text);
    int row = item->row();
    ui->detail->setText(QString("No.%1组数据包详细信息:").arg(row+1).toUtf8());
    ui->detail->append(" ");
    ui->detail->append(QString("抓取时间:"+ui->table->item(row,0)->text()));
    ui->detail->append(QString("源地址:"+ui->table->item(row,1)->text()));
    ui->detail->append(QString("目的地址:"+ui->table->item(row,2)->text()));
    ui->detail->append(QString("协议类型:"+ui->table->item(row,3)->text()));
    ui->detail->append(QString("本包长度:"+ui->table->item(row,4)->text()));
    ui->detail->append(QString("相关信息:"+ui->table->item(row,5)->text()));
}
void MainWindow::startListing(int indexOfAdapter)
{
    //开始监听
    /* 跳转到已选中的适配器 */
        for(d=alldevs, i=0; i< indexOfAdapter ;d=d->next, i++);
        qDebug()<<indexOfAdapter+1;
        /* 打开设备 */
        if ( (adhandle= pcap_open_live(d->name,          // 设备名
                                  65536,            // 要捕捉的数据包的部分
                                                    // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
                                  1,             // 远程机器验证
                                  1000,             // 读取超时时间
                                  errbuf            // 错误缓冲池
                                  ) ) == NULL)
        {
            fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
            /* 释放设列表 */
            pcap_freealldevs(alldevs);
            return ;
        }
        qDebug()<<QString("listening on %1").arg(d->description);
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        /**
          抓包操作放在线程中
          **/
        listeners->setFlag(true);
        listeners->setAdHandle(adhandle);
        listeners->moveToThread(thread);
        thread->start();
        //发出信号
        emit startInListener();

}


void MainWindow::on_filterButton_clicked()
{

    if(ui->filterBox->currentIndex()==0)
    {
        //选中按“协议”筛选
        QString keyWord = ui->keyWord->text();
        if(keyWord=="")
        {
            QMessageBox::about(this,QString("警告"),QString("请输入筛选关键字"));
        }
        else
        {
            //"TCP","WSP","ULP","SSL","SSDP","IPv4","HTTP","DNS","Ethertype"
            if(keyWord=="TCP")
            {
               filterExecution(keyWord);
            }
            else if(keyWord=="WSP")
            {
                filterExecution(keyWord);
            }else if(keyWord=="ULP")
            {
               filterExecution(keyWord);
            }else if(keyWord=="SSL")
            {
                filterExecution(keyWord);
            }else if(keyWord=="SSDP")
            {
               filterExecution(keyWord);
            }else if(keyWord=="IPv4")
            {
                filterExecution(keyWord);
            }else if(keyWord=="HTTP")
            {
                filterExecution(keyWord);
            }else if(keyWord=="DNS")
            {
               filterExecution(keyWord);
            }else if(keyWord=="Ethertype")
            {
                filterExecution(keyWord);
            }else if(keyWord=="ARP")
            {
                filterExecution(keyWord);
            }else if(keyWord=="UDP")
            {
                filterExecution(keyWord);
            }else if(keyWord=="MDNS")
            {
                filterExecution(keyWord);
            }else if(keyWord=="TLSv1.2")
            {
                filterExecution(keyWord);
            }else if(keyWord=="LLMNR")
            {
                filterExecution(keyWord);
            }else if(keyWord=="IGMPv2")
            {
                filterExecution(keyWord);
            }else if(keyWord=="ICMPv6")
            {
                filterExecution(keyWord);
            }else if(keyWord=="DHCPv6")
            {
                filterExecution(keyWord);
            }else
            {
                QMessageBox::about(this,QString::fromLocal8Bit("Nothing Found"),QString::fromLocal8Bit("No Such Package"));
            }

        }
    }
    else
    {
        QMessageBox::question(this,QString("抱歉"),QString("暂只支持Protocal过滤条件"),QMessageBox::Close,QMessageBox::Yes);
    }
}

void MainWindow::filterExecution(QString keyWord)
{
    qDebug()<<"过滤Ethertype";
    for(int i=0;i<TotalLineOfPackage;i++)
    {
        QString temp = ui->table->item(i,3)->text();
        qDebug()<<temp;
        if(temp!=keyWord)
        {
            ui->table->setRowHidden(i,true);
        }

    }
}

void MainWindow::on_actionchoose_triggered()
{
    //重置复选框适配器列表
    choose->ResetComoBox();
    //获取本地设备列表
        if (pcap_findalldevs(&alldevs, errbuf) == -1)
        {
            QMessageBox::warning(this,"Warming",QString(errbuf));
            //fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
            exit(1);
        }
        else
        {
            //输出列表
            for (d = alldevs; d != NULL; d = d->next)
            {
                if (d->description)
                {
                    QString deviceDescription=QString("%1:%2").arg(d->name).arg(d->description);
                    qDebug()<<deviceDescription;
                    choose->appendItem(deviceDescription);
                }
                else
                {
                    QString descriptionString = "No description available";
                    QString deviceDescription=QString("%1:%2").arg(d->name).arg(descriptionString);
                    choose->appendItem(deviceDescription);
                }

            }
            choose->show();
        }


}

void MainWindow::on_actionstop_triggered()
{
    //立即停止抓包
    listeners->setFlag(false);
}

void MainWindow::on_actiongoon_triggered()
{
    //继续抓包
    listeners->setFlag(true);
    emit startInListener();
}

void MainWindow::on_actiontishi_triggered()
{
    QMessageBox::information(this,"Package","默认抓取5000组数据包");
}

void MainWindow::on_about_triggered()
{

}
