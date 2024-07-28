#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVariantMap>
#include <QVariantList>

class JsonHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap coord MEMBER coord NOTIFY dataChanged)
    Q_PROPERTY(QVariantMap weather MEMBER weather NOTIFY dataChanged)
    Q_PROPERTY(QVariantMap main MEMBER main NOTIFY dataChanged)
    Q_PROPERTY(int visibility MEMBER visibility NOTIFY dataChanged)
    Q_PROPERTY(QVariantMap wind MEMBER wind NOTIFY dataChanged)
    Q_PROPERTY(QVariantMap rain MEMBER rain NOTIFY dataChanged)
    Q_PROPERTY(QVariantMap clouds MEMBER clouds NOTIFY dataChanged)
    Q_PROPERTY(QVariantMap sys MEMBER sys NOTIFY dataChanged)
    Q_PROPERTY(int timezone MEMBER timezone NOTIFY dataChanged)
    Q_PROPERTY(int id MEMBER id NOTIFY dataChanged)
    Q_PROPERTY(QString name MEMBER name NOTIFY dataChanged)
    Q_PROPERTY(int cod MEMBER cod NOTIFY dataChanged)
    Q_PROPERTY(QVariantList goods MEMBER goods NOTIFY dataChanged)

public:
    explicit JsonHandler(QObject *parent = nullptr);
    Q_INVOKABLE void fetchData();
    Q_INVOKABLE void fetchDataWithPath(const QString &path);

signals:
    void dataFetched(const QVariantList &services);
    void serverResponse(const QString &response);
    void detailDataFetched();
    void goodsDataFetched();
    void dataChanged();

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager m_networkManager;
    QVariantList servicesList;
    QVariantMap coord;
    QVariantMap weather;
    QVariantMap main;
    int visibility;
    QVariantMap wind;
    QVariantMap rain;
    QVariantMap clouds;
    QVariantMap sys;
    int timezone;
    int id;
    QString name;
    int cod;
    QVariantList goods;
};

#endif // JSONHANDLER_H
