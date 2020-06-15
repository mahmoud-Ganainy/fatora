#ifndef EMPLOYEE_MAN_H
#define EMPLOYEE_MAN_H

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
#include<qtrpt.h>
namespace Ui {
class employee_man;
}

class employee_man : public QWidget
{
    Q_OBJECT

public:
    explicit employee_man(QWidget *parent = nullptr);
    ~employee_man();
private slots:

    void clearall();
    void checkStat();


    void on_listView_activated(const QModelIndex &index);

    void on_save_clicked();
private:
    Ui::employee_man *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;
};

#endif // EMPLOYEE_MAN_H
