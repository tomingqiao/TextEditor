

greaterThan(QT_MAJOR_VERSION,4):QT+=widgets


QT       += core gui

TARGET = myMdi
TEMPLATE = app


SOURCES += main.cpp\
    aboutdialog.cpp \
    codeeditor.cpp \
        mainwindow.cpp \
    mdichild.cpp \
    replacedialog.cpp \
    searchdialog.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    codeeditor.h \
    mdichild.h \
    replacedialog.h \
    searchdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    replacedialog.ui \
    searchdialog.ui

RESOURCES += \
     myImage.qrc

RC_ICONS = myico4.ico
