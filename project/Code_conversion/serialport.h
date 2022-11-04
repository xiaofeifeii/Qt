#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class serialport
{
public:
    serialport();
    QSerialPort serial;
};

#endif // SERIALPORT_H
