#ifndef DATA_SHEET_H
#define DATA_SHEET_H

#include <QWidget>
#include <QtSql>
#include<QCompleter>
#include<QMenuBar>
#include<QMenu>
#include<QDate>
#include<QSqlQuery>
#include<QMessageBox>
#include<QByteArray>
#include<QtNetwork/QNetworkInterface>
#include<QBuffer>
#include<QFile>
#include<QIODevice>
#include<QPixmap>
#include<QComboBox>
#include<QFontComboBox>
#include<QTableWidgetItem>
#include<QTimer>
#include<qtrpt.h>
#include<qshortcut.h>
#include<LimeReport>
#include <QtConcurrent/QtConcurrent>
namespace Ui {
class data_sheet;
}

class data_sheet : public QWidget
{
    Q_OBJECT

public:
    explicit data_sheet(QWidget *parent = nullptr);
    ~data_sheet();

private:
    Ui::data_sheet *ui;
    QSqlDatabase db;
    QSqlDatabase db2;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;
    QSqlQueryModel *model4;
    QSqlQueryModel *model5;
    QSqlQueryModel *model6;
    QSqlQueryModel *model7;
    QSqlQueryModel *model8;
    QSqlQueryModel *model10;
    QSqlQueryModel *model11;
};

#endif // DATA_SHEET_H
