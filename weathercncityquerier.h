/**
  * weathercncityqueirer.h - 中国天气网城市查询工具类
  *
  * 提供API的调用、返回之JSON的解析，以及为TableView准备的model对象。
  *
  * 注：TableView需要model来显示内容，这类似于Android中ListView的用法。
  */

#ifndef WEATHERCNCITYQUERIER_H
#define WEATHERCNCITYQUERIER_H

#include <QObject>
#include <QMainWindow>
#include <QStandardItemModel>

// 中国天气网城市查询API地址
#define QUERY_URL "http://toy1.weather.com.cn/search?cityname=%1"
// 用于解析API返回结果的正则表达式
#define RESULT_REGEX "(\\d+)~(\\w+)~([\u4E00-\u9FA5]+)~(\\w+)~([\u4E00-\u9FA5]+)~(\\w+)~(\\d+)~(\\d+)~(\\w+)~([\u4E00-\u9FA5]+)"

typedef QMap<QString, QVariant> QMAP_CANDIDATE;     // 让代码更优雅一些～

class WeatherCnCityQuerier : public QObject
{
    Q_OBJECT

public:
    WeatherCnCityQuerier();
    QStandardItemModel *getModel();     // 返回model，可直接供TableView使用

public slots:
    // 槽函数，提供天气查询。我的预期是在文本框中键入的同时，能实时返回检索结果
    void startQueryCity(QString keyword);

private:
    QStandardItemModel *model;          // 供TableView使用的model
    // 获取并解析中国天气网API返回的JSON
    QList<QMAP_CANDIDATE> getAndParseJson(QString keyword);

    // 互斥锁，防止因输入过快或网络延迟导致TableView中出现重复项
    bool mutexLock = false;
};

#endif // WEATHERCNCITYQUERIER_H
