#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

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
#include<QTimer>
#include<qtrpt.h>
#include<qshortcut.h>
#include<LimeReport>

namespace Ui {
class reportmanager;
}

class reportmanager : public QDialog
{
    Q_OBJECT

public:
    explicit reportmanager(QWidget *parent = nullptr);
    ~reportmanager();

private slots:
    void enable_buy();
    void disable_buy();
    void enable_sell();
    void disable_sell();
    void enable_inv();
    void disable_inv();

    void on_pushButton_clicked();
    void on_pur_supp_check_toggled(bool checked);

    void on_pur_barcode_check_toggled(bool checked);

    void on_pur_int_check_toggled(bool checked);

    void on_pur_item_name_check_toggled(bool checked);
    void slotGetCallbackData(const LimeReport::CallbackInfo& info, QVariant& data);

    void on_sale_custom_check_toggled(bool checked);

    void on_sale_cash_depo_check_toggled(bool checked);

    void on_sale_seller_check_toggled(bool checked);

    void on_sale_barcode_check_toggled(bool checked);

    void on_sale_int_check_toggled(bool checked);

    void on_sale_item_name_check_toggled(bool checked);

    void on_pushButton_2_clicked();

    void on_report_type_currentIndexChanged(int index);

private:
    Ui::reportmanager *ui;
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
	QSqlQueryModel *model9;
    QSqlQueryModel *model10;
    QSqlQueryModel *model11;
    LimeReport::ReportEngine *report;
    double buy_total,sell_total,quan_total;
    QString shop_name,shop_phone,trade,tax;

};

#endif // REPORTMANAGER_H
