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

    QSqlQuery query;

    QString createCounterpartiesTable = R"(
        CREATE TABLE IF NOT EXISTS counterparties (
            counterparty_id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL
        );
    )";

    QString createInnsTable = R"(
        CREATE TABLE IF NOT EXISTS inns (
            counterparty_id INTEGER,
            inn TEXT NOT NULL,
            satellite INTEGER,
            PRIMARY KEY(counterparty_id, inn)
        );
    )";

    QString createOperationDatesTable = R"(
        CREATE TABLE IF NOT EXISTS operation_dates (
            counterparty_id INTEGER,
            date_oper TEXT NOT NULL,
            operation_id INTEGER PRIMARY KEY AUTOINCREMENT
        );
    )";

    QString createIncomingAmountsTable = R"(
        CREATE TABLE IF NOT EXISTS incoming_amounts (
            operation_id INTEGER,
            amount REAL NOT NULL,
            PRIMARY KEY(operation_id, amount)
        );
    )";

    QString createPaymentPurposesTable = R"(
        CREATE TABLE IF NOT EXISTS payment_purposes (
            operation_id INTEGER,
            purpose TEXT NOT NULL,
            PRIMARY KEY(operation_id, purpose)
        );
    )";

    QString createSuppliersTable = R"(
        CREATE TABLE IF NOT EXISTS suppliers (
            operation_id INTEGER,
            supplier_inn TEXT NOT NULL,
            PRIMARY KEY(operation_id, supplier_inn)
        );
    )";

    if (!query.exec(createCounterpartiesTable)) {
        qDebug() << "Failed to create table counterparties:" << query.lastError().text();
        return false;
    }

    if (!query.exec(createInnsTable)) {
        qDebug() << "Failed to create table inns:" << query.lastError().text();
        return false;
    }

    if (!query.exec(createOperationDatesTable)) {
        qDebug() << "Failed to create table operation_dates:" << query.lastError().text();
        return false;
    }

    if (!query.exec(createIncomingAmountsTable)) {
        qDebug() << "Failed to create table incoming_amounts:" << query.lastError().text();
        return false;
    }

    if (!query.exec(createPaymentPurposesTable)) {
        qDebug() << "Failed to create table payment_purposes:" << query.lastError().text();
        return false;
    }

    if (!query.exec(createSuppliersTable)) {
        qDebug() << "Failed to create table suppliers:" << query.lastError().text();
        return false;
    }

    return true;
}
