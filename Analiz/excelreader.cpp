#include "excelreader.h"
#include "xlsxdocument.h"
#include <QDebug>

ExcelReader::ExcelReader(QObject *parent) : QObject(parent) {
}

ExcelReader::~ExcelReader() {
}

bool ExcelReader::readXlsxFile(const QString &filePath) {
    QXlsx::Document xlsx(filePath);
    if (!xlsx.isLoadPackage()) {
        qDebug() << "Failed to load the xlsx file:" << filePath;
        return false;
    }

    sheetData.clear();

    QStringList sheetNames = xlsx.sheetNames();
    if (sheetNames.isEmpty()) {
        qDebug() << "No sheets found in the xlsx file.";
        return false;
    }

    qDebug() << "Sheets available in the xlsx file:" << sheetNames;

    QXlsx::Worksheet *sheet = dynamic_cast<QXlsx::Worksheet *>(xlsx.sheet(sheetNames.first()));
    if (!sheet) {
        qDebug() << "Failed to access the first sheet in the xlsx file.";
        return false;
    }

    int rowCount = sheet->dimension().rowCount();
    int colCount = sheet->dimension().columnCount();

    for (int row = 1; row <= rowCount; ++row) {
        QList<QVariant> rowData;
        for (int col = 1; col <= colCount; ++col) {
            rowData.append(sheet->cellAt(row, col) ? sheet->cellAt(row, col)->value() : QVariant());
        }
        sheetData.append(rowData);
    }

    return true;
}

QList<QList<QVariant>> ExcelReader::getSheetData(int sheetIndex) const {
    Q_UNUSED(sheetIndex); // We are only handling the first sheet in this example.
    return sheetData;
}
