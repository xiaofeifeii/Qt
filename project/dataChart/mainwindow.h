#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTimer timer;
    QVector<double> key;
    QVector<double> value;
    QVector<double> value2;
    QVector<double> value3;

    Ui::MainWindow *ui;
    QCustomPlot *customPlot;

private slots:
   void Graph();
   double getNow();
   void on_xRange_clicked();
   void on_pause_clicked();
   void on_xyRange_clicked();
};

#endif // MAINWINDOW_H
