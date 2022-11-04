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

void MainWindow::on_send_clicked()
{
    QString txt_code=ui->lineEdit->text();
    //    AA BB 02 0D 14 07 D1 00 00

    txt_code = QString::number(txt_code.toInt(), 16).toUpper();
    QByteArray ba = QByteArray::fromHex("AABB020D14")+QByteArray::fromHex(txt_code.toLatin1())+QByteArray::fromHex("0000");
    port.write(ba);
    qDebug()<<"txt_code:"<<ba;

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
