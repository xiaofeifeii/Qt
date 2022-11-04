#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    QSerialPort port;


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_checkSerial_clicked();

    void on_openSerial_clicked();

    void on_send_clicked();

    void log();

    void on_clean_clicked();

private:
    Ui::MainWindow *ui;


signals:
    void logAppend(QString str);
};

#endif // MAINWINDOW_H
