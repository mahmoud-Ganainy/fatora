#ifndef BUY_WIN_H
#define BUY_WIN_H

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
#include<QTimer>
#include<qtrpt.h>
#include<qshortcut.h>
#include<LimeReport>
#include <QtConcurrent/QtConcurrent>
#include<item_search.h>
#include<invoice_search.h>

namespace Ui {
class buy_Win;
}

class buy_Win : public QMainWindow
{
    Q_OBJECT

public:
    explicit buy_Win(QWidget *parent = 0);
    ~buy_Win();
public slots:
    void clearAll();


private slots:
    void setData();
    void setData_int();
    void setData_bcode();
    void clearLines();
    void setTable();
    void getTotal();
    void getTotal_items();
    void enter_pressed_1();
    void enter_pressed_2();
    void enter_pressed_3();
    void enter_pressed_4();
    void print_save();
    void print_bcode();
    void save_items();
    void set_edit();
    void set_unedit();
    void delete_row();
    void update_time();
    void update_itemsList();
    void update_List();
	void search_invoice();
	void show_search();
    void showEvent(QShowEvent *event);



    void on_quantity_returnPressed();

    void on_price_buy_returnPressed();

    void on_price_sell_returnPressed();

    void on_expire_check_stateChanged(int arg1);

    void on_discount_editingFinished();

    void on_discount_returnPressed();

    void on_dicount_perc_editingFinished();

    void on_dicount_perc_returnPressed();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_save_clicked();

    void on_total_rec_textChanged(const QString &arg1);

    void on_saved_clicked();

    void on_rec_no_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_dateEdit_dateChanged(const QDate &date);

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_rec_stat_currentIndexChanged(int index);

    void on_item_bcode_activated(const QString &arg1);

    void on_item_int_activated(const QString &arg1);

    void on_un_save_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_pushButton_3_clicked();

    void on_supp_name_currentIndexChanged(int index);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_tableWidget_2_clicked(const QModelIndex &index);

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_item_name_currentIndexChanged(int index);

    void slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
	void slotGetPrintCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);

private:
    Ui::buy_Win *ui;
    bool first_open = true;
	bool saved;
    bool unsaved;
	bool searching;
	int cur_search_index;
    QVector<int> search_list;
    int max_search_index;
	QString print_item_name , print_barcode , print_sell;

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
    QSqlQuery *qry_in;
    QTableView *tv;
    item_search *search;
	invoice_search *inv_search;

    LimeReport::ReportEngine *report;
    int cur_row;
    QString shop_name,shop_phone,trade,tax;


};

#endif // BUY_WIN_H
