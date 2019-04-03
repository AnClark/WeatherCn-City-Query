#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    querier(new WeatherCnCityQuerier)
{
    ui->setupUi(this);

    ui->tblResult->setModel(querier->getModel());

    connect(ui->txtKeyword, SIGNAL(textChanged(QString)), querier, SLOT(startQueryCity(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
