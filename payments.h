#ifndef PAYMENTS_H
#define PAYMENTS_H

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
namespace Ui {
class payments;
}

class payments : public QWidget
{
    Q_OBJECT

public:
    explicit payments(QWidget *parent = nullptr);
    ~payments();

private slots:
  void ClearAll();
  void set_edit();
  void set_unedit();
  void save();
  void save_print();
  void update_time();
  void update_List();



  void on_rec_stat_currentIndexChanged(int index);

  void on_saved_clicked();

  void on_delete_rec_clicked();

  void on_next_clicked();

  void on_back_clicked();

  void on_customer_name_currentIndexChanged(int index);

  void on_supp_name_currentIndexChanged(int index);

  void on_save_clicked();

  void on_rec_no_textChanged(const QString &arg1);

  void on_pushButton_4_clicked();

private:
    Ui::payments *ui;
    int cur_row;
    bool saved;
    QSqlDatabase db;
    QSqlDatabase db2;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;
    QSqlQueryModel *model4;
    QSqlQueryModel *model5;
};

#endif // PAYMENTS_H
