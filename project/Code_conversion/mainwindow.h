#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>


#define CRC_DEFAULT_VALUE  0xFFFF
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    QSerialPort port;
    quint16 crc16CheckValue(quint8 *pucBuf, quint16 usLen)
    {
        quint16 usCrc = CRC_DEFAULT_VALUE;
        quint8 ucTmp = 0;
        int i = 0,j = 0;
        for (i = 0; i < usLen; i++) {
            ucTmp = ((*pucBuf) & 0X00FF);
            ++pucBuf;
            usCrc ^= ucTmp;
            for (j = 0; j < 8; j++) {
                if (usCrc & 0X0001) {
                    usCrc >>= 1;
                    usCrc ^= 0XA001;
                } else {
                    usCrc >>= 1;
                }
            }
        }
        return usCrc;
    }



public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_checkSerial_clicked();

    void on_openSerial_clicked();

    void log();

    void on_clean_clicked();

    void on_send_sign_NID_clicked();

    void on_read_sign_NID_clicked();

    void on_send_clicked();

private:
    Ui::MainWindow *ui;


signals:
    void logAppend(QString str);
};

#endif // MAINWINDOW_H
