#include "weathercncityquerier.h"

WeatherCnCityQuerier::WeatherCnCityQuerier()
{
    model = new QStandardItemModel;

    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, "Id");
    model->setHeaderData(1, Qt::Horizontal, "City Name");
    model->setHeaderData(2, Qt::Horizontal, "Belongs to");
}

QStandardItemModel *WeatherCnCityQuerier::getModel()
{
    return model;
}


void WeatherCnCityQuerier::startQueryCity(QString keyword)
{
    QList<QStandardItem*> item_test;
    item_test << new QStandardItem("10101010") << new QStandardItem(keyword) << new QStandardItem("China");

    model->appendRow(item_test);
}
