#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTableWidgetItem>
#include <choosedevice.h>
#include <pcap.h>
#include <QThread>
#include <listener.h>

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void filterExecution(QString keyWord);
signals:
    //在线程中启动，对应Slots:void ReceiveInListener();
    void startInListener();
public slots:
    void itemSelectedAndShowDetail(QTableWidgetItem *item);
    void on_filterButton_clicked();
    void on_actionchoose_triggered();
    void startListing(int indexOfAdapter);
    void PutPackageIntoTable(Package *package);
public:
    Ui::MainWindow *ui;
    QFile file;
    int TotalLineOfPackage=0;
    chooseDevice *choose;
    pcap_if_t *alldevs;	//适配器列表，它是一个链表的数据结构
    pcap_if_t *d;		//保存某个适配器
    int i=0;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *adhandle;

//    const u_char *pkt_data;
//    struct pcap_pkthdr *header;
//    time_t local_tv_sec;
//    struct tm *ltime;
//    char timestr[16];
//    ip_header *ih;

    int res;
    QThread *thread;
    Listener *listeners;
    //QString allProtocal[9] = {"TCP","WSP","ULP","SSL","SSDP","IPv4","HTTP","DNS","Ethertype"};
private slots:
    void on_actionstop_triggered();
    void on_actiongoon_triggered();
    void on_actiontishi_triggered();
};

#endif // MAINWINDOW_H
