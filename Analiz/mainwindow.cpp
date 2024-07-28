#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "excelreader.h"
#include "tablecreator.h"
#include <QFileDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QApplication>  // Для processEvents

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentOffset(0) // Инициализация сдвига
    , totalRows(0)
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
    if (db.isOpen()) {
        return true;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("analiz.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    }

    return true;
}

void MainWindow::closeDatabase()
{
    if (db.isOpen()) {
        db.close();
    }
}

void MainWindow::insertDataToDatabase(const QStringList &data, int currentRow, int totalRows)
{
    if (!db.isOpen() && !openDatabase()) {
        qDebug() << "Database is not open and cannot be opened";
        return;
    }

    QSqlQuery query;

    // Вставка в таблицу контрагентов
    query.prepare("INSERT INTO counterparties (name) VALUES (?)");
    query.addBindValue(data[0]);
    if (!query.exec()) {
        qDebug() << "Failed to insert data into counterparties:" << query.lastError().text();
        return;
    }
    int counterparty_id = query.lastInsertId().toInt();

    // Вставка в таблицу ИНН
    query.prepare("INSERT INTO inns (counterparty_id, inn) VALUES (?, ?)");
    query.addBindValue(counterparty_id);
    query.addBindValue(data[1]);
    if (!query.exec()) {
        qDebug() << "Failed to insert data into inns:" << query.lastError().text();
        return;
    }

    // Вставка в таблицу дат операций
    query.prepare("INSERT INTO operation_dates (counterparty_id, date_oper) VALUES (?, ?)");
    query.addBindValue(counterparty_id);
    query.addBindValue(data[2]);
    if (!query.exec()) {
        qDebug() << "Failed to insert data into operation_dates:" << query.lastError().text();
        return;
    }
    int operation_id = query.lastInsertId().toInt();

    // Вставка в таблицу приходов в рублях
    query.prepare("INSERT INTO incoming_amounts (operation_id, amount) VALUES (?, ?)");
    query.addBindValue(operation_id);
    query.addBindValue(data[3]);
    if (!query.exec()) {
        qDebug() << "Failed to insert data into incoming_amounts:" << query.lastError().text();
        return;
    }

    // Вставка в таблицу назначений платежа
    query.prepare("INSERT INTO payment_purposes (operation_id, purpose) VALUES (?, ?)");
    query.addBindValue(operation_id);
    query.addBindValue(data[4]);
    if (!query.exec()) {
        qDebug() << "Failed to insert data into payment_purposes:" << query.lastError().text();
        return;
    }

    // Вставка в таблицу поставщиков
    query.prepare("INSERT INTO suppliers (operation_id, supplier_inn) VALUES (?, ?)");
    query.addBindValue(operation_id);
    query.addBindValue(data[1]);
    if (!query.exec()) {
        qDebug() << "Failed to insert data into suppliers:" << query.lastError().text();
        return;
    }

    updateStatusLabel(currentRow, totalRows); // Обновление QLabel
    QApplication::processEvents(); // Обновление пользовательского интерфейса
}

void MainWindow::showDatabaseInfo()
{
    if (!db.isOpen() && !openDatabase()) {
        qDebug() << "Database is not open and cannot be opened";
        return;
    }

    QSqlQuery query;
    int totalRows = 0;

    if (query.exec("SELECT COUNT(*) FROM counterparties")) {
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
    QString turnoverQuery = R"(
        SELECT c.name, i.inn, o.date_oper, ia.amount, pp.purpose,
        (SELECT SUM(ia2.amount) FROM incoming_amounts ia2
         JOIN operation_dates o2 ON ia2.operation_id = o2.operation_id
         WHERE o2.counterparty_id = o.counterparty_id) as turnover
        FROM counterparties c
        JOIN inns i ON c.counterparty_id = i.counterparty_id
        JOIN operation_dates o ON c.counterparty_id = o.counterparty_id
        JOIN incoming_amounts ia ON o.operation_id = ia.operation_id
        JOIN payment_purposes pp ON o.operation_id = pp.operation_id
        ORDER BY i.inn, o.date_oper
        LIMIT 15 OFFSET ?
    )";
    query.prepare(turnoverQuery);
    query.addBindValue(offset);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve turnover data:" << query.lastError().text();
        return;
    }

    ui->tableWidget->setRowCount(0);

    QStringList headers;
    headers << "Контрагент" << "ИНН" << "Дата операции" << "Сумма операции" << "Назначение платежа" << "Оборот";
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    QString lastInn;
    QString lastCounterpartyName;
    double totalTurnover = 0.0;

    while (query.next()) {
        QString currentCounterpartyName = query.value(0).toString();
        QString currentInn = query.value(1).toString();

        if (currentInn != lastInn) {
            lastInn = currentInn;
            QSqlQuery sumQuery;
            sumQuery.prepare(R"(
                SELECT SUM(ia.amount)
                FROM incoming_amounts ia
                JOIN operation_dates o ON ia.operation_id = o.operation_id
                JOIN counterparties c ON o.counterparty_id = c.counterparty_id
                JOIN inns i ON c.counterparty_id = i.counterparty_id
                WHERE i.inn = ?
            )");
            sumQuery.addBindValue(lastInn);
            if (!sumQuery.exec()) {
                qDebug() << "Failed to calculate turnover:" << sumQuery.lastError().text();
                return;
            }
            if (sumQuery.next()) {
                totalTurnover = sumQuery.value(0).toDouble();
            }
        }

        ui->tableWidget->insertRow(row);

        QTableWidgetItem *item0 = new QTableWidgetItem();
        QTableWidgetItem *item1 = new QTableWidgetItem();
        QTableWidgetItem *item5 = new QTableWidgetItem();

        if (currentCounterpartyName != lastCounterpartyName) {
            item0->setText(currentCounterpartyName);
            item1->setText(query.value(1).toString());
            lastCounterpartyName = currentCounterpartyName;
            qDebug() << "Вывожу имя контрагента: " << currentCounterpartyName;
        } else {
            item0->setText("");
            item1->setText("");
            qDebug() << "Вставляю пустую строчку для контрагента: " << currentCounterpartyName;
        }

        item5->setText(QString::number(totalTurnover, 'f', 2));

        item0->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 0, item0);

        item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(query.value(2).toString());
        item2->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 2, item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(query.value(3).toString());
        item3->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item3->setFlags(item3->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 3, item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(query.value(4).toString());
        item4->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item4->setText(query.value(4).toString().replace(QRegularExpression("(.{70})"), "\\1\n"));
        item4->setFlags(item4->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 4, item4);

        item5->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item5->setFlags(item5->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 5, item5);

        ++row;
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

void MainWindow::updateStatusLabel(int currentRow, int totalRows)
{
    ui->labelStatus->setText(tr("Processing row %1 of %2").arg(currentRow).arg(totalRows));
    QApplication::processEvents(); // Обновление пользовательского интерфейса
}

void MainWindow::on_btnBrowse_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Excel File"), "", tr("Excel Files (*.xlsx)"));
    if (!filePath.isEmpty()) {
        ui->lineEditFilePath->setText(filePath);

        // Создаем или открываем базу данных и таблицы
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
            totalRows = data.size() - startRow;
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

                insertDataToDatabase(rowData, row - startRow + 1, totalRows);
            }

            showDatabaseInfo();

            currentOffset = 0;
            loadTurnoverData(currentOffset);
            ui->labelStatus->setText(tr("Processing completed."));
        } else {
            qDebug() << "Failed to read the XLSX file.";
        }
    }
}

void MainWindow::clearTables()
{
    if (!db.isOpen() && !openDatabase()) {
        qDebug() << "Database is not open and cannot be opened";
        return;
    }

    QSqlQuery query;
    QStringList tableNames = {"counterparties", "inns", "operation_dates", "incoming_amounts", "payment_purposes", "suppliers"};

    for (const QString &tableName : tableNames) {
        if (!query.exec("DELETE FROM " + tableName)) {
            qDebug() << "Failed to clear table " << tableName << ":" << query.lastError().text();
        }
    }
}

void MainWindow::on_btnClearDatabase_clicked()
{
    clearTables();
    showDatabaseInfo();
    currentOffset = 0;
    loadTurnoverData(currentOffset);
    ui->labelStatus->setText(tr("Tables cleared."));
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
    QString sumQuery = R"(
        SELECT SUM(amount)
        FROM incoming_amounts
    )";

    if (!query.exec(sumQuery)) {
        qDebug() << "Failed to calculate sum:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        double sum = query.value(0).toDouble();
        ui->lineEditSum->setText(QString::number(sum, 'f', 2)); // Форматирование числа с двумя знаками после запятой
    }
}
