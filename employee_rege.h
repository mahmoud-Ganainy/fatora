#ifndef EMPLOYEE_REGE_H
#define EMPLOYEE_REGE_H

#include <QDialog>
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
class employee_rege;
}

class employee_rege : public QDialog
{
    Q_OBJECT

public:
    explicit employee_rege(QWidget *parent = nullptr);
    ~employee_rege();



private slots:

    void clearall();
    void checkStat();


    void on_listView_activated(const QModelIndex &index);

    void on_save_clicked();

    void on_clear_clicked();

    void on_delete_2_clicked();

private:
    Ui::employee_rege *ui;
    int cashier,seller,army,insurance;
    QSqlDatabase db;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;
};

#endif // EMPLOYEE_REGE_H
