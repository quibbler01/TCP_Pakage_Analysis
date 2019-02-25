#ifndef PACKAGE_H
#define PACKAGE_H
#include <QString>
#include <QTextStream>
class Package
{

public:
    Package();
    Package(int no,double time,QString source,QString destination,QString protocal,int length,QString info);
    Package(QString line);
    ~Package();

public :
    int No;
    double Time;
    QString Source;
    QString Destination;
    QString Protocal;
    int Length;
    QString Info;
};

#endif // PACKAGE_H
