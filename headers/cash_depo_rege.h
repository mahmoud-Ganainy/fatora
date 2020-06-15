#ifndef CASH_DEPO_REGE_H
#define CASH_DEPO_REGE_H

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
namespace Ui {
class cash_depo_rege;
}

class cash_depo_rege : public QDialog
{
    Q_OBJECT

public:
    explicit cash_depo_rege(QWidget *parent = nullptr);
    ~cash_depo_rege();

private slots:
    void on_listView_clicked(const QModelIndex &index);

    void on_lineEdit_returnPressed();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::cash_depo_rege *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QCompleter *comp;
};

#endif // CASH_DEPO_REGE_H
