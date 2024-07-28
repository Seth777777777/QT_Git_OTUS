#include "jsonhandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariantList>
#include <QVariantMap>
#include <QDebug>

JsonHandler::JsonHandler(QObject *parent) : QObject(parent)
{
    connect(&m_networkManager, &QNetworkAccessManager::finished, this, &JsonHandler::onReplyFinished);
}

void JsonHandler::fetchData()
{
    QNetworkRequest request(QUrl("https://otestserver_1-1-o9142362.deta.app/"));
    m_networkManager.get(request);
}

void JsonHandler::fetchDataWithPath(const QString &path)
{
    QUrl url("https://otestserver_1-1-o9142362.deta.app" + path);
    qDebug() << "Fetching data with URL:" << url.toString();
    QNetworkRequest request(url);
    m_networkManager.get(request);
}

void JsonHandler::onReplyFinished(QNetworkReply *reply)
{
    QString responseText;
    QVariantList servicesList;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        responseText = QString::fromUtf8(response);
        qDebug() << "Response from server:" << responseText;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();

        if (jsonObj.contains("services")) {
            QJsonArray servicesArray = jsonObj["services"].toArray();
            for (const QJsonValue &value : servicesArray) {
                QJsonObject serviceObj = value.toObject();
                QVariantMap serviceMap;
                serviceMap["name"] = serviceObj["name"].toString();
                serviceMap["description"] = serviceObj["description"].toString();
                serviceMap["path"] = serviceObj["path"].toString();
                servicesList.append(serviceMap);
            }
            emit dataFetched(servicesList);
        } else if (jsonObj.contains("goods")) {
            goods = jsonObj["goods"].toArray().toVariantList();
            qDebug() << "Goods data:" << goods;
            emit goodsDataFetched();
        } else {
            coord = jsonObj["coord"].toObject().toVariantMap();
            weather = jsonObj["weather"].toArray()[0].toObject().toVariantMap();
            main = jsonObj["main"].toObject().toVariantMap();
            visibility = jsonObj["visibility"].toInt();
            wind = jsonObj["wind"].toObject().toVariantMap();
            rain = jsonObj["rain"].toObject().toVariantMap();
            clouds = jsonObj["clouds"].toObject().toVariantMap();
            sys = jsonObj["sys"].toObject().toVariantMap();
            timezone = jsonObj["timezone"].toInt();
            id = jsonObj["id"].toInt();
            name = jsonObj["name"].toString();
            cod = jsonObj["cod"].toInt();

            qDebug() << "Coord:" << coord;
            qDebug() << "Weather:" << weather;
            qDebug() << "Main:" << main;
            qDebug() << "Visibility:" << visibility;
            qDebug() << "Wind:" << wind;
            qDebug() << "Rain:" << rain;
            qDebug() << "Clouds:" << clouds;
            qDebug() << "Sys:" << sys;
            qDebug() << "Timezone:" << timezone;
            qDebug() << "ID:" << id;
            qDebug() << "Name:" << name;
            qDebug() << "Cod:" << cod;

            emit serverResponse(responseText);
            emit detailDataFetched();
        }
    } else {
        responseText = "Error: " + reply->errorString();
        qDebug() << "Error in network reply:" << responseText;
        emit serverResponse(responseText);
    }
    reply->deleteLater();
}
