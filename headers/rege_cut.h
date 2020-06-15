#ifndef REGE_CUT_H
#define REGE_CUT_H

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
#include<dialog.h>
#include<dialog1.h>
#include<dialog2.h>
#include<dialog3.h>
#include<dialog4.h>
#include<price_update.h>
#include<QShortcut>
#include <QVector>
#include <buy_win.h>
#include<search_items.h>
namespace Ui {
class rege_cut;
}

class rege_cut : public QDialog
{
    Q_OBJECT

public:
    explicit rege_cut(QWidget *parent = nullptr);
    ~rege_cut();

signals:
    void addeditems();
private slots:


    //void on_item_int_returnPressed();
    void test1();
    void test2();
    void test3();

    void test6();
    void test7();


    void find_items();
    void rec_item();
    void get_sub_det(QString table);
    void update_price();




    void on_item_buy_returnPressed();

    void on_item_profit_returnPressed();

    void on_item_profit_editingFinished();

    void on_item_sell_editingFinished();

    void on_item_sell_returnPressed();

    void on_item_buy_editingFinished();

    void on_add_clicked();

    void clear_sub();
    void clear_all();
    void insert_items();
    void load_table();
    void on_next_clicked();
    void re_con();
    void update_item();
    void update_combo();


    void on_item_name_returnPressed();

    void on_item_id_textChanged(const QString &arg1);

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    //void on_checkBox_stateChanged(int arg1);

    void on_pushButton_6_clicked();


private:
    Ui::rege_cut *ui;
    Dialog *yes;
    Dialog1 *yes1;
    Dialog2 *yes2;
    Dialog3 *yes3;
    Dialog4 *yes4;
    price_update *pri_up;
    search_items *search1;
    buy_Win *buy;
    QSqlDatabase db;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;
    QSqlQueryModel *model4;
    QSqlQueryModel *model5;
    QSqlQueryModel *model6;
    QSqlQueryModel *model7;
    QSqlQueryModel *model8;

    QCompleter *completer;
    QString in_class;
    QString in_sea;
    QString in_cat;
    QString in_clr;
    QString in_size;
    QString in_code;
};

#endif // REGE_CUT_H
