QT += qml core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    excelreader.cpp \
    main.cpp \
    mainwindow.cpp \
    tablecreator.cpp \


HEADERS += \
    excelreader.h \
    mainwindow.h \
    tablecreator.h \

FORMS += \
    mainwindow.ui
    # QXlsx code for Application Qt project
    QXLSX_PARENTPATH=./         # current QXlsx path is . (. means curret directory)
    QXLSX_HEADERPATH=./header/  # current QXlsx header path is ./header/
    QXLSX_SOURCEPATH=./source/  # current QXlsx source path is ./source/
    include(./QXlsx.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Добавьте путь к QXlsx
INCLUDEPATH += C:/QT_Programming/QT_Git_OTUS/QXlsx_source/

