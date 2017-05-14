#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <pcap.h>
#include <package.h>
/* 4字节的IP地址 */
typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;
/* IPv4 首部 */
typedef struct ip_header{
    u_char  ver_ihl;        // 版本 (4 bits) + 首部长度 (4 bits)
    u_char  tos;            // 服务类型(Type of service)
    u_short tlen;           // 总长(Total length)
    u_short identification; // 标识(Identification)
    u_short flags_fo;       // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    u_char  ttl;            // 存活时间(Time to live)
    u_char  proto;          // 协议(Protocol)
    u_short crc;            // 首部校验和(Header checksum)
    ip_address  saddr;      // 源地址(Source address)
    ip_address  daddr;      // 目的地址(Destination address)
    u_int   op_pad;         // 选项与填充(Option + Padding)
}ip_header;

class Listener : public QObject
{
    Q_OBJECT
public:
    explicit Listener(QObject *parent = 0);
    pcap_t *adhandle;
    void setAdHandle(pcap_t *adhandle_);
    const u_char *pkt_data;
    struct pcap_pkthdr *header;
    time_t local_tv_sec;
    struct tm *ltime;
    char timestr[16];
    ip_header *ih;
    Package *package;
    void ExecuteInListener();
    void setFlag(bool flag);
    bool FLAG=false;
signals:
    void OnePackageIsReade(Package *package);
public slots:
    void ReceiveInListener();

};

#endif // LISTENER_H
