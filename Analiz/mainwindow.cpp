#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "excelreader.h"
#include "tablecreator.h"
#include <QFileDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentOffset(0) // Инициализация сдвига
{
    ui->setupUi(this);
    if (!openDatabase()) {
        qDebug() << "Failed to open database.";
    }

    // Установка заголовков столбцов таблицы
    QStringList headers;
    headers << "Контрагент" << "ИНН" << "Дата операции" << "Сумма операции" << "Назначение платежа" << "Оборот";
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Загрузка первых 15 записей
    loadTurnoverData(currentOffset);
}

MainWindow::~MainWindow()
{
    closeDatabase();
    delete ui;
}

bool MainWindow::openDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("analiz.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    }

    QSqlQuery query;
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS transactions ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "kontagent TEXT, "
                               "inn TEXT, "
                               "date_oper TEXT, "
                               "sum_oper REAL, "
                               "pay_purpose TEXT)";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Failed to create table:" << query.lastError().text();
        return false;
    }

    return true;
}

void MainWindow::closeDatabase()
{
    db.close();
}

void MainWindow::insertDataToDatabase(const QStringList &data)
{
    if (!db.isOpen() && !openDatabase()) {
        qDebug() << "Database is not open and cannot be opened";
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO transactions (kontagent, inn, date_oper, sum_oper, pay_purpose) "
                  "VALUES (?, ?, ?, ?, ?)");

    query.addBindValue(data[0]);
    query.addBindValue(data[1]);
    query.addBindValue(data[2]);
    query.addBindValue(data[3]);
    query.addBindValue(data[4]);

    if (!query.exec()) {
        qDebug() << "Failed to insert data:" << query.lastError().text();
    }
}

void MainWindow::showDatabaseInfo()
{
    if (!db.isOpen() && !openDatabase()) {
        qDebug() << "Database is not open and cannot be opened";
        return;
    }

    QSqlQuery query;
    int totalRows = 0;

    if (query.exec("SELECT COUNT(*) FROM transactions")) {
        if (query.next()) {
            totalRows = query.value(0).toInt();
        }
    } else {
        qDebug() << "Failed to retrieve total row count:" << query.lastError().text();
    }

    QMessageBox::information(this, tr("Database Info"),
                             tr("Total rows in database: %1").arg(totalRows));
}

void MainWindow::loadTurnoverData(int offset)
{
    if (!db.isOpen() && !openDatabase()) {
        qDebug() << "Database is not open and cannot be opened";
        return;
    }

    QSqlQuery query;
    QString turnoverQuery = "SELECT kontagent, inn, date_oper, sum_oper, pay_purpose, "
                            "(SELECT SUM(sum_oper) FROM transactions t2 WHERE t2.inn = t1.inn) as turnover "
                            "FROM transactions t1 ORDER BY id LIMIT 15 OFFSET ?";
    query.prepare(turnoverQuery);
    query.addBindValue(offset);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve turnover data:" << query.lastError().text();
        return;
    }

    ui->tableWidget->setRowCount(0);

    // Установка заголовков столбцов таблицы, включая новый столбец "Оборот"
    QStringList headers;
    headers << "Контрагент" << "ИНН" << "Дата операции" << "Сумма операции" << "Назначение платежа" << "Оборот";
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // Оборот
        ++row;
    }

    // Автоматическая подгонка размера столбцов и строк
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

void MainWindow::on_btnBrowse_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Excel File"), "", tr("Excel Files (*.xlsx)"));
    if (!filePath.isEmpty()) {
        ui->lineEditFilePath->setText(filePath);

        // Создаем или открываем базу данных и таблицу
        TableCreator tableCreator;
        if (!tableCreator.createTableIfNotExists("analiz.db")) {
            qDebug() << "Failed to create or open the database.";
            return;
        }

        // Пример использования ExcelReader для чтения данных из файла
        ExcelReader excelReader;
        if (excelReader.readXlsxFile(filePath)) {
            QList<QList<QVariant>> data = excelReader.getSheetData();

            int startRow = 2; // Начальная строка (третья строка, индекс 2)
            for (int row = startRow; row < data.size(); ++row) {
                QString sumOper = data[row][21].toString();
                if (sumOper.toDouble() == 0) {
                    continue; // Пропустить запись, если значение в столбце V равно 0
                }

                QStringList rowData;
                rowData << data[row][12].toString()  // M - Контрагент
                        << data[row][13].toString()  // N - Инн
                        << data[row][10].toString()  // K - Дата операции
                        << sumOper                    // V - Сумма операции
                        << data[row][23].toString(); // X - Назначение платежа

                insertDataToDatabase(rowData);
            }

            // Показать информацию о базе данных после добавления данных
            showDatabaseInfo();

            // Обновить таблицу после добавления данных
            currentOffset = 0;
            loadTurnoverData(currentOffset);
        } else {
            qDebug() << "Failed to read the XLSX file.";
        }
    }
}

void MainWindow::on_btnClearDatabase_clicked()
{
    if (!db.isOpen() && !openDatabase()) {
        qDebug() << "Database is not open and cannot be opened";
        return;
    }

    QSqlQuery query;
    if (!query.exec("DELETE FROM transactions")) {
        qDebug() << "Failed to clear the table:" << query.lastError().text();
    } else {
        qDebug() << "Table cleared successfully.";
        // Показать информацию о базе данных после очистки данных
        showDatabaseInfo();

        // Обновить таблицу после очистки данных
        currentOffset = 0;
        loadTurnoverData(currentOffset);
    }
}

void MainWindow::on_btnNextData_clicked()
{
    if (!db.isOpen() && !openDatabase()) {
        qDebug() << "Database is not open and cannot be opened";
        return;
    }

    currentOffset += 15;
    loadTurnoverData(currentOffset);
}

void MainWindow::on_btnPreviousData_clicked()
{
    if (!db.isOpen() && !openDatabase()) {
        qDebug() << "Database is not open and cannot be opened";
        return;
    }

    if (currentOffset >= 15) {
        currentOffset -= 15;
    }
    loadTurnoverData(currentOffset);
}

void MainWindow::on_btnCalculateSum_clicked()
{
    if (!db.isOpen() && !openDatabase()) {
        qDebug() << "Database is not open and cannot be opened";
        return;
    }

    QSqlQuery query;
    if (!query.exec("SELECT SUM(sum_oper) FROM transactions")) {
        qDebug() << "Failed to calculate sum:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        double sum = query.value(0).toDouble();
        ui->lineEditSum->setText(QString::number(sum, 'f', 2)); // Форматирование числа с двумя знаками после запятой
    }
}
