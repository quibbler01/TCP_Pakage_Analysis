#include "package.h"
#include <QDebug>
Package::Package()
{
    //  默认构造函数
}

Package::Package(int no, double time, QString source, QString destination,
                 QString protocal, int length, QString info)
{
    No = no;
    Time = time;
    Source =source;
    Destination = destination;
    Protocal = protocal;
    Length = length;
    Info = info;
}

Package::Package(QString line)
{
   //No = line.section(' ',1,1);
    QTextStream *stream=new QTextStream(&line,QIODevice::ReadOnly);
    *stream>>No>>Time>>Source>>Destination>>Protocal>>Length;
    Info = stream->readLine();
    Info.remove(0,4);
    /**
     * qDebug()<<No<<" | "<<Time<<" | "<<Source<<" | "<<Destination<<" | "<<Protocal
           <<" | "<<Length<<" | "<<Info;
    **/
}
