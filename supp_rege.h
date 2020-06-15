#ifndef SUPP_REGE_H
#define SUPP_REGE_H

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
class supp_rege;
}

class supp_rege : public QMainWindow
{
    Q_OBJECT

public:
    explicit supp_rege(QWidget *parent = 0);
    ~supp_rege();

private slots:
    void on_pushButton_clicked();
    void clearall();
    void checkStat();

    void on_listView_activated(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
signals:
    void addeditems();
private:
    Ui::supp_rege *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;
    int type;
    int stop_deal;
    int stop_payment;
    int stop_return;
};

#endif // SUPP_REGE_H
