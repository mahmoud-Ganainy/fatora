#ifndef CUSTOMER_REPORT_H
#define CUSTOMER_REPORT_H

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
#include<LimeReport>
namespace Ui {
class customer_report;
}

class customer_report : public QWidget
{
    Q_OBJECT

public:
    explicit customer_report(QWidget *parent = nullptr);
    ~customer_report();
private slots:

    void clearall();



    void on_checkBox_toggled(bool checked);

    void on_customer_currentIndexChanged(int index);


    void on_checkBox_6_clicked(bool checked);

    void on_pushButton_clicked();
    void slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);

private:
    Ui::customer_report *ui;
    QSqlDatabase db;
    QSqlDatabase db2;

    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;
    QSqlQueryModel *model4;
    QSqlQueryModel *model5;
    QSqlQueryModel *model6;
    LimeReport::ReportEngine *report;
    QString shop_name,shop_phone,trade,tax;
};

#endif // CUSTOMER_REPORT_H
