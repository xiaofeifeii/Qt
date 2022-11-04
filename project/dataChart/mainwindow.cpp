#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    customPlot=ui->widget;
    customPlot->clearGraphs();

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    customPlot->addGraph(ui->widget->xAxis, ui->widget->yAxis);
    customPlot->graph(0)->setPen(QPen(Qt::red));

    customPlot->addGraph(ui->widget->xAxis, ui->widget->yAxis);
    customPlot->graph(1)->setPen(QPen(Qt::black));

    customPlot->addGraph(ui->widget->xAxis, ui->widget->yAxis2);
    customPlot->graph(2)->setPen(QPen(Qt::blue));

    connect( &timer, SIGNAL(timeout()),this, SLOT(Graph()));

    timer.start(1000);

    //  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);//日期做X轴
    ui->widget->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->widget->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->widget->xAxis->setDateTimeSpec(Qt::LocalTime);
    ui->widget->xAxis->setTickLabels(true);//显示刻度标签
    customPlot->xAxis->setLabel("时间");
    customPlot->yAxis->setLabel("气压值");
    customPlot->yAxis2->setLabel("压力值");

    customPlot->graph(0)->setName("目标气压");
    customPlot->graph(1)->setName("实际气压");
    customPlot->graph(2)->setName("压力");
    customPlot->legend->setVisible(true); // 显示图例

    //配置右轴和上轴以显示刻度线，但不显示标签：
   //（请参阅 QCPAxisRect：：setupFullAxesBox 以获取更快的方法来执行此操作）
    ui->widget->xAxis2->setVisible(true);
    ui->widget->xAxis2->setTickLabels(false);
    ui->widget->yAxis2->setVisible(true);
    ui->widget->yAxis2->setTickLabels(true);

    //使左轴和下轴始终将其范围传输到右轴和顶轴：
    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->xAxis2, SLOT(setRange(QCPRange)));
    //connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->yAxis2, SLOT(setRange(QCPRange)));
    customPlot->yAxis2->setRange(0,100);
    customPlot->yAxis->setRange(0,800);

}

double MainWindow::getNow()
{
    return (double)(QDateTime::currentMSecsSinceEpoch()) / 1000.0;
    QDateTime::currentDateTime();
}


void MainWindow::Graph()
{
    double now = getNow();

   //接收数据值
    static int number1=50,number2=10,number3=100,n=0;

    value.push_back(number1);
    value2.push_back(number2);
    value3.push_back(number3);


    if(n>3)
    {
        number1=0;
        number3=0;
        n--;
    }else{
        number1=50;
        number3=100;
        n++;
    }
//  value3.push_back((serial.regs.fAimPressure*2));
    key.push_back(now);

    //将数据点传递到图形：
    ui->widget->graph(0)->addData(key,value);
    ui->widget->graph(1)->addData(key,value2);
    ui->widget->graph(2)->addData(key,value3);

     if (1) {
         double max = 0;
         double max2 = 0;
         foreach (double val, value) {
             if (val > max) {
                 max = val;
             }
         }
         foreach (double val, value2) {
             if (val > max) {
                 max = val;
             }
         }
         foreach (double val, value3) {
             if (val > max2) {
                 max2 = val;
             }
         }
         max += 50;
         max2 += 500;


         customPlot->yAxis->setRange(-10, max);
         customPlot->yAxis2->setRange(-10, max2);
         if (key.last() > key.first() + 60) {
             customPlot->xAxis->setRange(now - 60, now + 5);
         } else {
             customPlot->xAxis->setRange(key.first(), key.last() + 5);
         }
     }

    if(1){ui->widget->replot();}//刷新

    ui->widget->xAxis2->setVisible(true);
    ui->widget->xAxis2->setTickLabels(false);
    ui->widget->yAxis2->setVisible(true);
    ui->widget->yAxis2->setTickLabels(true);
    qDebug()<<"now:"<<now<<"number1:"<<number1;
    QDateTime curtime = QDateTime::currentDateTime();

    qDebug()<<"time:"<<curtime.toString("yyyy-MM-dd hh:mm:ss");

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_xRange_clicked()
{
    if(ui->xRange->text() == "x轴缩放"){
        ui->widget->axisRect()->setRangeZoomFactor(0.8,1);
        ui->xRange->setText("y轴缩放");
    }else {
        ui->widget->axisRect()->setRangeZoomFactor(1,0.8);
        ui->xRange->setText("x轴缩放");
    }
}

void MainWindow::on_pause_clicked()
{
    if(ui->pause->text() == "暂停"){
        timer.stop();
        ui->pause->setText("开始");
    }else {
        timer.start(1000);
        ui->pause->setText("暂停");
    }
}

void MainWindow::on_xyRange_clicked()
{
    ui->widget->axisRect()->setRangeZoomFactor(0.8,0.8);
}
