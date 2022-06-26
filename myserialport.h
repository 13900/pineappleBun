#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>

class MySerialPort
{

public:
    MySerialPort();
    QSerialPort* default_init();
    QStringList serial_search();
    QString fileName;
    void setPortName(const QString &name);
    void setBaudRate(int index=4800);
    void setDataBits(int index=0);
    void setStopBits(int index=0);
    void setParity(int index=0);
    int openSerialPort();
    bool closeSerialPort();
    QByteArray receive_data();
    void sendSerialData(const QByteArray &data);
    bool sendSerialFileData();

private:
    QSerialPort *serialPort;
    QSerialPortInfo *serialPortInfo;
    QStringList serialStringList;
};

#endif // MYSERIALPORT_H
