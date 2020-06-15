#ifndef SUPPLIER_REPORT_H
#define SUPPLIER_REPORT_H

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
#include<LimeReport>
namespace Ui {
class supplier_report;
}

class supplier_report : public QDialog
{
    Q_OBJECT

public:
    explicit supplier_report(QWidget *parent = nullptr);
    ~supplier_report();
private slots:

    void clearall();
    void on_supplier_currentIndexChanged(int index);



    void on_checkBox_toggled(bool checked);

    void on_pushButton_clicked();
    void slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);


private:
    Ui::supplier_report *ui;
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

#endif // SUPPLIER_REPORT_H
