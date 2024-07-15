#ifndef EXCELREADER_H
#define EXCELREADER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariant>

class ExcelReader : public QObject {
    Q_OBJECT

public:
    explicit ExcelReader(QObject *parent = nullptr);
    ~ExcelReader();
    bool readXlsxFile(const QString &filePath);
    QList<QList<QVariant>> getSheetData(int sheetIndex = 0) const;

private:
    QList<QList<QVariant>> sheetData;
};

#endif // EXCELREADER_H
