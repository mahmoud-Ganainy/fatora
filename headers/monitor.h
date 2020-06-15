#ifndef MONITOR_H
#define MONITOR_H

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
#include<QStandardItemModel>
#include<LimeReport>

namespace Ui {
class monitor;
}

class monitor : public QWidget
{
    Q_OBJECT
private slots:

  void on_comboBox_currentIndexChanged(int index);

  void on_pushButton_2_clicked();

  void on_pushButton_clicked();
  void slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
public slots:
  void getTable();
  void getstat(int id);

public:
    explicit monitor(QWidget *parent = nullptr);
    ~monitor();

private:
    Ui::monitor *ui;
    QSqlDatabase db;
    QSqlDatabase db2;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    double sales_cash=0;
    double sales_credit=0;
    double sales_return_cash=0;
    double sales_return_credit=0;
    double purchases_cash=0;
    double purchases_credit=0;
    double purchases_return_cash=0;
    double purchases_return_credit=0;
    double supplier_payment_out=0;
    double supplier_payment_in=0;
    double customer_payment_in=0;
    double customer_payment_out=0;
    double expenses_cash=0;
    double expenses_credit=0;
    double sales_total=0;
    double purchases_total=0;
    double purchases_return_total=0;
    double cash_deposit_total=0;
    LimeReport::ReportEngine *report;
    QString shop_name,shop_phone,trade,tax;

};

#endif // MONITOR_H
