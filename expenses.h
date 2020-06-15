#ifndef EXPENSES_H
#define EXPENSES_H

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
class expenses;
}

class expenses : public QWidget
{
    Q_OBJECT
private slots:
  void ClearAll();
  void set_edit();
  void set_unedit();
  void save();
  void save_print();
  void update_time();
  void on_tableWidget_cellChanged(int row, int column);

  void on_save_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

  void on_saved_clicked();

  void on_delete_rec_clicked();

  void on_rec_no_textChanged(const QString &arg1);

  void on_next_rec_clicked();

  void on_back_rec_clicked();

public:
    explicit expenses(QWidget *parent = nullptr);
    ~expenses();

private:
    Ui::expenses *ui;
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

#endif // EXPENSES_H
