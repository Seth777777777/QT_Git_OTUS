#include "tablecreator.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

TableCreator::TableCreator() {
}

TableCreator::~TableCreator() {
    if (db.isOpen()) {
        db.close();
    }
}

bool TableCreator::createTableIfNotExists(const QString &dbName) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "Error: unable to open database:" << db.lastError();
        return false;
    }

    QSqlQuery query(db);
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS transactions ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "kontagent TEXT, "
                               "inn TEXT, "
                               "date_oper TEXT, "
                               "sum_oper REAL, "
                               "pay_purpose TEXT);";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error: unable to create table:" << query.lastError();
        return false;
    }

    return true;
}

bool TableCreator::insertData(const QStringList &data) {
    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO transactions (kontagent, inn, date_oper, sum_oper, pay_purpose) "
                  "VALUES (?, ?, ?, ?, ?)");

    query.addBindValue(data[0]);
    query.addBindValue(data[1]);
    query.addBindValue(data[2]);
    query.addBindValue(data[3]);
    query.addBindValue(data[4]);

    if (!query.exec()) {
        qDebug() << "Failed to insert data:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlDatabase TableCreator::getDatabase() {
    return db;
}
