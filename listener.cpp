#include "listener.h"
#include <mainwindow.h>
#include <QDebug>

Listener::Listener(QObject *parent) : QObject(parent)
{

}

//Slots
void Listener::ReceiveInListener()
{
    qDebug()<<"Receive!";
    ExecuteInListener();
}
void Listener::setAdHandle(pcap_t *adhandle_)
{
    this->adhandle = adhandle_;
}
void Listener::setFlag(bool flag)
{
    FLAG = flag;
}

void Listener::ExecuteInListener()
{
    int temp=0;

    while(++temp<5000&&pcap_next_ex( adhandle, &header, &pkt_data)>=0&&FLAG==true)
    {
        package = new Package();
        local_tv_sec=header->ts.tv_sec;
        ltime=localtime(&local_tv_sec);
        strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
//        qDebug()<<QString("%1 %2 len:%3").arg(timestr).arg(header->ts.tv_usec).arg(header->len);
        ih = (ip_header *) (pkt_data + 14);
//        qDebug()<<"源地址"<<ih->saddr.byte1<<"."<<ih->saddr.byte2<<"."<<ih->saddr.byte3<<"."<<ih->saddr.byte4;
//        qDebug()<<"目的地址"<<ih->daddr.byte1<<"."<<ih->daddr.byte2<<"."<<ih->daddr.byte3<<"."<<ih->daddr.byte4;
//        qDebug()<<"协议"<<ih->proto;
//        qDebug()<<"选项填充"<<ih->op_pad;
//        qDebug()<<header->ts.tv_usec;
        package->Time =header->ts.tv_usec;

        package->Source = QString("%1.%2.%3.%4")
                .arg(ih->saddr.byte1)
                .arg(ih->saddr.byte2)
                .arg(ih->saddr.byte3)
                .arg(ih->saddr.byte4);
        package->Destination = QString("%1.%2.%3.%4")
                .arg(ih->daddr.byte1)
                .arg(ih->daddr.byte2)
                .arg(ih->daddr.byte3)
                .arg(ih->daddr.byte4);
        package->Length = header->len;

        package->Protocal = QString("协议号:%1").arg(ih->proto);

        package->Info = QString("Type of service:%1,Time to live:%2,Header checksum:%3,Option+Padding:%4,Identification:%5")
                .arg(ih->tos)
                .arg(ih->ttl)
                .arg(ih->crc)
                .arg(ih->op_pad)
                .arg(ih->identification);
        emit OnePackageIsReade(package);
    }
}
