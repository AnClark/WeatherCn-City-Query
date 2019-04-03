#ifndef WEATHERCNCITYQUERIER_H
#define WEATHERCNCITYQUERIER_H

#include <QObject>
#include <QMainWindow>
#include <QStandardItemModel>

class WeatherCnCityQuerier : public QObject
{
    Q_OBJECT

public:
    WeatherCnCityQuerier();
    QStandardItemModel *getModel();

public slots:
    void startQueryCity(QString keyword);

private:
    QStandardItemModel *model;
};

#endif // WEATHERCNCITYQUERIER_H
