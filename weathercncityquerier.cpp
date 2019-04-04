#include "weathercncityquerier.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QRegExp>

WeatherCnCityQuerier::WeatherCnCityQuerier()
{
    // 初始化模型
    model = new QStandardItemModel;

    // 设置列数和表头
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, "Id");
    model->setHeaderData(1, Qt::Horizontal, "City Name");
    model->setHeaderData(2, Qt::Horizontal, "Belongs to");
}

QStandardItemModel *WeatherCnCityQuerier::getModel()
{
    // 返回类中已配置好的模型
    // 只要模型有更新，TableView显示的内容也会更新
    return model;
}


void WeatherCnCityQuerier::startQueryCity(QString keyword)
{
    // 先清空模型中的数据
    model->clear();

    // 获得解析结果
    QList<QMAP_CANDIDATE> candidates = getAndParseJson(keyword);

    // 将解析结果显示到模型当中
    for(int i=0; i < candidates.length(); i++)
    {
        QList<QStandardItem*> currentCity;      // 用一个QList装下一行的所有列
        currentCity << new QStandardItem(candidates.at(i).value("Id").toString())
                    << new QStandardItem(candidates.at(i).value("name").toString())
                    << new QStandardItem(candidates.at(i).value("superior").toString());
        model->appendRow(currentCity);
    }
}

QList<QMAP_CANDIDATE> WeatherCnCityQuerier::getAndParseJson(QString keyword)
{
    // 存放解析结果的列表
    QList<QMAP_CANDIDATE> candidates;

    // 互斥锁检查。一旦上了互斥锁，就说明有另一个过程正在进行，则当前过程就不应再继续。
    if(mutexLock)
        return candidates;

    /**
     * 【第一步】
     * 访问API，取得JSON。
     */

    mutexLock = true;                       // 上互斥锁

    QString URLSTR = QUERY_URL;             // 载入预先定义好的API地址

    // 生成URL对象
    // 地址中预留了参数位（%1），在这里填上
    QUrl url(URLSTR.arg(keyword));

    // 创建网络访问管理器
    QNetworkAccessManager manager;

    // 网络请求的过程在事件循环中进行。
    QEventLoop loop;                // 建立子事件循环
    qDebug() << "获取城市列表中..." << "  API为"<< url.toString();
    // 发出网络请求，并将请求结果记录下来
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    // 请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();                    // 开启子事件循环

    // 读取网络请求中的文本。从API返回的文本是JSON格式的。
    QString rawData = reply->readAll();

    qDebug() << "已完成";

    /**
      * 【第二步】
      * 解析JSON。
      *
      * 中国天气网城市查询API的JSON格式为：
      * (
      *   [
      *     {"ref": "城市编号~省拼音~城市名~拼音~城市名~拼音~不带零的区号~邮编~首字母简拼~省份"},
      *     ...
      *   ]
      * )
      * 可见格式都是固定的，因此可直接用正则表达式匹配。
      */

    // 建立正则表达式对象
    QRegExp re(RESULT_REGEX);
    int pos(0);         // 记录当前解析字符串的位置

    /* 开始用正则表达式解析字符串 */
    // re.indexIn(str, pos)返回pos处匹配的索引，同时也用于接收字符串给re.cap()使用
    // 找不到匹配时，下面的表达式返回-1（实际上就是pos的值）
    while((pos = re.indexIn(rawData, pos)) != -1)
    {
        QMAP_CANDIDATE cityItem;        // 用QMap存储捕获得到的信息
        // 开始捕获。re.cap()的参数用于指定捕获哪一个子串，即第几对圆括号里的内容
        // 为0则捕获整个字符串
        cityItem["Id"]             = re.cap(1);
        cityItem["pinyinProvince"] = re.cap(2);
        cityItem["name"]           = re.cap(3);
        cityItem["pinyin"]         = re.cap(4);
        cityItem["name2"]          = re.cap(5);
        cityItem["pinyin2"]        = re.cap(6);
        cityItem["zoneDesc"]       = re.cap(7);
        cityItem["zipcode"]        = re.cap(8);
        cityItem["firstLetter"]    = re.cap(9);
        cityItem["superior"]       = re.cap(10);

        // 向前继续解析
        pos += re.matchedLength();
        // 将解析结果存入列表中
        candidates.push_back(cityItem);
    }

    mutexLock = false;          // 解互斥锁
    return candidates;
}
