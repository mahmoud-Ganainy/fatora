#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<rege.h>
#include<rege_cut.h>
#include <QMainWindow>
#include <buy_win.h>
#include <buy_win_cut.h>
#include<sell_win_cut.h>
#include<sell_win.h>
#include<supp_rege.h>
#include<custom_rege.h>
#include<payments.h>
#include<cash_depo_rege.h>
#include<storage_rege.h>
#include<expenses.h>
#include<monitor.h>
#include<setting_window.h>
#include<QCloseEvent>
#include<QFile>
#include<QFileDialog>
#include <QtSql>
#include <QProcess>
#include <QProgressBar>
#include <reportmanager.h>
#include <supplier_report.h>
#include<employee_rege.h>
#include<employee_man.h>
#include<data_sheet.h>
#include<customer_report.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionCustomer_triggered();

    void on_actionSupplier_triggered();

    void on_actionSeason_triggered();

    void on_actionSales_Return_triggered();

    void on_actionBuy_Return_triggered();

    void on_actionCategory_triggered();

    void on_actionClass_triggered();

    void on_actionItems_triggered();

    void on_actionsetting_triggered();
    void closeEvent(QCloseEvent *e);



    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_tabWidget_tabCloseRequested(int index);


    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_actiontest_triggered();

    void on_action_6_triggered();

    void on_action_7_triggered();

    void on_action_8_triggered();

    void on_action_9_triggered();

    void on_action_10_triggered();

private:
    Ui::MainWindow *ui;
    rege *reges;
    buy_Win *buy_wind;
    rege_cut *reges_cut;
    buy_win_cut *buy_wind_cut;
    sell_win *sell_wind;
    sell_win_cut *sell_wind_cut;
    supp_rege *supp;
    custom_rege *custom;
    cat_rege *categ;
    class_rege *class_regi;
    custom_rege *custom_regi;
    sea_rege *sea_regi;
    setting_window *sett;
    payments *payment;
    cash_depo_rege *cash_rege;
    storage_rege *storage_reg;
    supplier_report *supp_rep;
    customer_report *custom_rep;
    employee_rege *emp_rege;
    employee_man *emp_man;
    data_sheet * data_shit;


    expenses *expen;
    monitor * moni;
    reportmanager *repo;
    QSqlDatabase db;
    QSqlDatabase db1;
    QSqlDatabase db2;
    QString type;
    QString loc_data;
    QString set_data;
    QString server_host;
    QString server_user;
    QString server_pass;
    int version =0;

};

#endif // MAINWINDOW_H
