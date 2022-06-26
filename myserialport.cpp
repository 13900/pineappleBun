#include "myserialport.h"
#include <QMessageBox>

MySerialPort::MySerialPort()
{
    serialPort = new QSerialPort();
    serialPortInfo = new QSerialPortInfo();

}

QSerialPort* MySerialPort::default_init()
{
    serialPort->setBaudRate(4800);
    serialPort->setDataBits(QSerialPort::Data5);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::NoParity);
    return serialPort;
}
QStringList MySerialPort::serial_search()
{
    foreach(const QSerialPortInfo &info, serialPortInfo->availablePorts())
    {
        serialStringList<<info.portName();
    }
    return serialStringList;
}

void MySerialPort::setPortName(const QString &name)
{
    serialPort->setPortName(name);
}

void MySerialPort::setBaudRate(int index)
{
    serialPort->setBaudRate(index);
}
void MySerialPort::setDataBits(int index)
{
    switch (index) {
    case 0:
        serialPort->setDataBits(QSerialPort::Data5);
        break;
     case 1:
        serialPort->setDataBits(QSerialPort::Data6);
        break;
    case 2:
       serialPort->setDataBits(QSerialPort::Data7);
       break;
    default:
       serialPort->setDataBits(QSerialPort::Data8);
       break;
    }
}

void MySerialPort::setStopBits(int index)
{
    switch (index) {
    case 0:
        serialPort->setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        serialPort->setStopBits(QSerialPort::OneAndHalfStop);
        break;
    default:
        serialPort->setStopBits(QSerialPort::TwoStop);
        break;
    }
}

void MySerialPort::setParity(int index)
{
    switch (index) {
    case 0:
        serialPort->setParity(QSerialPort::NoParity);
        break;
    case 1:
        serialPort->setParity(QSerialPort::EvenParity);
        break;
    case 2:
        serialPort->setParity(QSerialPort::OddParity);
        break;
    case 3:
        serialPort->setParity(QSerialPort::SpaceParity);
        break;
    default:
        serialPort->setParity(QSerialPort::MarkParity);
        break;
    }
}

int MySerialPort::openSerialPort()
{
    if(serialPort->isOpen())
    {
        return 0;
    }
    if(serialPort->open(QIODevice::ReadWrite))
    {
        return 1;
     }else
     {
        return 2;
     }
}

bool MySerialPort::closeSerialPort()
{
    serialPort->close();
    if(serialPort->isOpen())
    {
        return false;
    }else
    {
        return true;
    }
}

QByteArray MySerialPort::receive_data()
{
    QByteArray message;
    message.append(serialPort->readAll());
    return message;
}

void MySerialPort::sendSerialData(const QByteArray &data)
{
    serialPort->write(data);
}

bool MySerialPort::sendSerialFileData()
{
    if(!fileName.isEmpty()){
        QFile file(fileName);
        file.open(QIODeviceBase::ReadWrite);
        QByteArray  array ;
        while(!file.atEnd())
        {
             array += file.readLine();
        }
        serialPort->write(array);
        return true;
    }
    return false;
}

