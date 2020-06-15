#ifndef CUSTOM_REGE_H
#define CUSTOM_REGE_H

#include <QMainWindow>
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
#include<qtrpt.h>

namespace Ui {
class custom_rege;
}

class custom_rege : public QMainWindow
{
    Q_OBJECT

public:
    explicit custom_rege(QWidget *parent = 0);
    ~custom_rege();
signals:
    void addeditems();
private slots:

    void clearall();
    void checkStat();


    void on_pushButton_clicked();

    void on_listView_activated(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::custom_rege *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;
    int type;
    int stop_deal;
    int stop_payment;
    int stop_return;
    int stop_sales;
};

#endif // CUSTOM_REGE_H
