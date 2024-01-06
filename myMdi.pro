

greaterThan(QT_MAJOR_VERSION,4):QT+=widgets


QT       += core gui

TARGET = myMdi
TEMPLATE = app


SOURCES += main.cpp\
    aboutdialog.cpp \
    codeeditor.cpp \
    highlighter.cpp \
        mainwindow.cpp \
    mdichild.cpp \
    recenthistorydialog.cpp \
    replacedialog.cpp \
    searchdialog.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    codeeditor.h \
    highlighter.h \
    mdichild.h \
    recenthistorydialog.h \
    replacedialog.h \
    searchdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    recenthistorydialog.ui \
    replacedialog.ui \
    searchdialog.ui

RESOURCES += \
     myImage.qrc

RC_ICONS = myico4.ico
