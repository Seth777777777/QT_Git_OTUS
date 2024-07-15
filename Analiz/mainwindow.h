#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnBrowse_clicked();
    void on_btnClearDatabase_clicked();
    void on_btnNextData_clicked();
    void on_btnPreviousData_clicked();
    void on_btnCalculateSum_clicked();
    void showDatabaseInfo();
    void updateStatusLabel(int currentRow, int totalRows);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    int currentOffset; // Текущий сдвиг для выборки данных
    int totalRows; // Общее количество строк

    bool openDatabase();
    void closeDatabase();
    void insertDataToDatabase(const QStringList &data, int currentRow, int totalRows);
    void loadTurnoverData(int offset);
    void clearTables();
};

#endif // MAINWINDOW_H
