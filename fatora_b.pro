#-------------------------------------------------
#
# Project created by QtCreator 2018-03-05T18:08:04
#
#-------------------------------------------------

QT       += core gui printsupport sql network quickwidgets
QTPLUGIN += QSQLMYSQL

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fatora_b
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    rege.cpp \
    dialog.cpp \
    dialog1.cpp \
    class_rege.cpp \
    sea_rege.cpp \
    cat_rege.cpp \
    dialog2.cpp \
    dialog3.cpp \
    dialog4.cpp \
    buy_win.cpp \
    supp_rege.cpp \
    custom_rege.cpp \
    search_items.cpp \
    setting_window.cpp \
    mysql_set.cpp \
    sqlite_set.cpp \
    price_update.cpp \
    payments.cpp \
    cash_depo_rege.cpp \
    storage_rege.cpp \
    expenses.cpp \
    monitor.cpp \
    item_search.cpp \
    reportmanager.cpp \
    invoice_search.cpp \
    supplier_report.cpp \
    rege_cut.cpp \
    buy_win_cut.cpp \
    sell_win_cut.cpp \
    sell_win.cpp \
    employee_rege.cpp \
    employee_man.cpp \
    data_sheet.cpp \
    customer_report.cpp \
    navigation.cpp

HEADERS += \
    ../testt/reciver.h \
        mainwindow.h \
    rege.h \
    dialog.h \
    dialog1.h \
    class_rege.h \
    sea_rege.h \
    cat_rege.h \
    dialog2.h \
    dialog3.h \
    dialog4.h \
    buy_win.h \
    supp_rege.h \
    custom_rege.h \
    search_items.h \
    setting_window.h \
    mysql_set.h \
    sqlite_set.h \
    price_update.h \
    payments.h \
    cash_depo_rege.h \
    storage_rege.h \
    expenses.h \
    monitor.h \
    item_search.h \
    reportmanager.h \
    invoice_search.h \
    supplier_report.h \
    rege_cut.h \
    buy_win_cut.h \
    sell_win_cut.h \
    sell_win.h \
    employee_rege.h \
    employee_man.h \
    data_sheet.h \
    customer_report.h \
    navigation.h

FORMS += \
        mainwindow.ui \
    rege.ui \
    dialog.ui \
    dialog1.ui \
    class_rege.ui \
    sea_rege.ui \
    cat_rege.ui \
    dialog2.ui \
    dialog3.ui \
    dialog4.ui \
    buy_win.ui \
    supp_rege.ui \
    custom_rege.ui \
    search_items.ui \
    setting_window.ui \
    mysql_set.ui \
    sqlite_set.ui \
    price_update.ui \
    rege_test.ui \
    payments.ui \
    cash_depo_rege.ui \
    storage_rege.ui \
    expenses.ui \
    monitor.ui \
    item_search.ui \
    reportmanager.ui \
    invoice_search.ui \
    supplier_report.ui \
    rege_cut.ui \
    buy_win_cut.ui \
    sell_win_cut.ui \
    sell_win.ui \
    employee_rege.ui \
    employee_man.ui \
    data_sheet.ui \
    customer_report.ui \
    navigation.ui
include($$PWD/QtRptProject/QtRPT/QtRPT.pri)
include($$PWD/LimeReport-1.4.7/limereport.pri)

DISTFILES += \
    MonitorForm.ui.qml \
    Monitor.qml
RESOURCES += \
    rc.qrc

RC_ICONS = logo_blank_BKI_icon.ico
