#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QMessageBox"
#include "QByteArray"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

   connect(&port, &QSerialPort::readyRead , this ,MainWindow::log);

    ui->setupUi(this);
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(info.portName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_checkSerial_clicked()
{
    ui->comboBox->clear();
    //检索端口号
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(info.portName());
    }

}

void MainWindow::on_openSerial_clicked()
{
    if(ui->openSerial->text() == QString("打开串口"))
    {
        QString portName = ui->comboBox->currentText();
        port.setPortName(portName);
        port.setBaudRate(115200);
        port.setDataBits(QSerialPort::Data8);
        port.setParity(QSerialPort::NoParity);
        port.setStopBits(QSerialPort::OneStop);
        port.setFlowControl(QSerialPort::NoFlowControl);


        if (!port.open(QIODevice::ReadWrite)) {
            qDebug()<<"open port" << portName << "failed";
            QMessageBox::warning(NULL , "提示", "串口打开失败！");
                        return;
        }

        qDebug()<<"open port" << portName << "success!";
        ui->openSerial->setText("关闭串口");
    }else{
        port.close();
        ui->openSerial->setText("打开串口");
    }

}



void MainWindow::log()
{
    QTextCursor tc = ui->textBrowser_log->textCursor();
    QByteArray buffer = port.readAll();
    QString receive = QString(buffer);
    tc.movePosition(QTextCursor::End);
    tc.insertText(receive);
    tc.movePosition(QTextCursor::End);
    ui->textBrowser_log->setTextCursor(tc);
}

void MainWindow::on_clean_clicked()
{
    ui->textBrowser_log->clear();
}



void MainWindow::on_send_clicked()
{
    QString nid_code=ui->lineEdit_NID->text();
    QString aid_code=ui->lineEdit_AID->text();
    QString did_code=ui->lineEdit_DID->text();
    //AA BB 02 0D(NID) 14(AID) 07 D1(DID) 00 00
    nid_code = QString::number(nid_code.toInt(), 16).toUpper();
    aid_code = QString::number(aid_code.toInt(), 16).toUpper();
    did_code = QString::number(did_code.toInt(), 16).toUpper();

    QByteArray ba = QByteArray::fromHex("AABB02")+
                    QByteArray::fromHex(nid_code.toLatin1())+ QByteArray::fromHex(aid_code.toLatin1()) +QByteArray::fromHex(did_code.toLatin1()) +
                    QByteArray::fromHex("0000");
    port.write(ba);
    qDebug()<<"NID_code:"<<ba;
}


void MainWindow::on_send_sign_NID_clicked()
{
//    AA BB 18 00 01 0D 00 00
    QString txt_code=ui->lineEdit_sign_NID->text();

    qDebug()<<txt_code.toInt()+1;
    if((txt_code.toInt()>0)&&(txt_code.toInt()<16))
    {

        txt_code = QString::number(txt_code.toInt(), 16).toUpper();
//        QByteArray ba = QByteArray::fromHex("AABB180001")+QByteArray::fromHex(txt_code.toLatin1())+QByteArray::fromHex("D1C6");
        QByteArray ba = QByteArray::fromHex("180001")+QByteArray::fromHex(txt_code.toLatin1());
        quint8 temp[20];
        memcpy(&temp, ba.data(), ba.length());

        temp[20]=crc16CheckValue(temp,ba.length());
        memcpy( ba.data(),&temp, ba.length());
        qDebug()<<"crc:"<<ba;
        port.write(ba);
        qDebug()<<"sign_NID_code:"<<ba;
    }else {
        QMessageBox::warning(NULL , "数据不合法", "NID范围:0~15");
    }


}

void MainWindow::on_read_sign_NID_clicked()
{
//    AABB1800010D0000
    QByteArray ba = QByteArray::fromHex("AABB1900010D0000");
    port.write(ba);
    qDebug()<<"read_NID_code:"<<ba;
}


