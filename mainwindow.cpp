#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    querier(new WeatherCnCityQuerier)
{
    ui->setupUi(this);

    ui->tblResult->setModel(querier->getModel());

    // 绑定关键字文本框的信号与城市查询函数的槽，以实现输入关键字即时得到检索结果
    // 注意Qt的信号与槽机制，要求信号的参数大于或等于槽的参数，否则会报编译错误！
    connect(ui->txtKeyword, SIGNAL(textChanged(QString)), querier, SLOT(startQueryCity(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
