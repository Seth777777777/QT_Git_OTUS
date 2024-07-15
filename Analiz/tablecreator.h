#ifndef TABLECREATOR_H
#define TABLECREATOR_H

#include <QtSql/QSqlDatabase>
#include <QStringList>

class TableCreator {
public:
    TableCreator();
    ~TableCreator();
    bool createTableIfNotExists(const QString &dbName);
    bool insertData(const QStringList &data);
    QSqlDatabase getDatabase(); // Метод для получения соединения с базой данных

private:
    QSqlDatabase db;
};

#endif // TABLECREATOR_H
