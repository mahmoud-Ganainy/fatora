#ifndef SELL_WIN_H
#define SELL_WIN_H

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
#include<QTimer>
#include<QTableWidgetItem>
#include<qtrpt.h>
#include<qshortcut.h>
#include<qicon.h>
#include<QElapsedTimer>
#include<LimeReport>
#include<item_search.h>
#include<invoice_search.h>

namespace Ui {
class sell_win;
}

class sell_win : public QMainWindow
{
    Q_OBJECT

public:
    explicit sell_win(QWidget *parent = 0);
    ~sell_win();
    void clear_test();

private slots:
    void setData();
    void clearAll();
    void clearLines();
    void getTotal_items();
    void getTotal();
    void enterName();
    void enterInt();
    void enterBcode();
    void setTable();
    void save();
    void print_recp();
    void print_save();
    void set_edit();
    void set_unedit();
    void delete_row();
    void test();
    void update_time();
    void update_itemsList();
    void update_List();
    void search_invoice();
    void show_search();
    void showEvent(QShowEvent *event);




    void on_expire_currentIndexChanged(int index);

    void on_sell_price_textChanged(const QString &arg1);

    void on_quan_textChanged(const QString &arg1);

    void on_discount_per_textChanged(const QString &arg1);

    void on_quan_returnPressed();

    void on_sell_price_returnPressed();

    void on_discount_per_returnPressed();

    void on_save_clicked();

    void on_rec_stat_currentIndexChanged(int index);

    void on_dicount_returnPressed();

    void on_discount_perc_editingFinished();

    void on_discount_perc_returnPressed();

    void on_total_before_textChanged(const QString &arg1);

    void on_pushButton_6_clicked();

    void on_ex_bcode_activated(const QString &arg1);

    void on_item_int_activated(const QString &arg1);

    void on_saved_rec_clicked();

    void on_rec_no_textChanged(const QString &arg1);

    void on_back_rec_clicked();

    void on_next_rec_clicked();

    void on_new_rec_clicked();

    void on_dicount_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_tableWidget_2_clicked(const QModelIndex &index);

    void on_unsaved_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_custom_name_currentIndexChanged(int index);


    void on_pushButton_3_clicked();

    void on_tableWidget_2_itemChanged(QTableWidgetItem *item);


    void slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
    void on_search_clicked();
	void on_item_name_currentIndexChanged(int index);


private:
    Ui::sell_win *ui;
    QSqlDatabase db;
    QSqlDatabase db2;
    bool first_open = true;
	bool saved;
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
    QSqlQueryModel *model12;
    QSqlQueryModel *model13;
    QSqlQueryModel *model14;
    QSqlQueryModel *model15;
    QSqlQueryModel *model16;
    LimeReport::ReportEngine *report;
    QCompleter *completer;
    QSqlQuery *qry_in;
    QTableView *tv;
    item_search *search;
    invoice_search *inv_search;
    int code_id;
    double item_bala;
    int cur_row;
    bool searching;
    int cur_search_index;
    QVector<int> search_list;
    int max_search_index;


};

#endif // SELL_WIN_H
