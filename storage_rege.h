#ifndef STORAGE_REGE_H
#define STORAGE_REGE_H

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
namespace Ui {
class storage_rege;
}

class storage_rege : public QDialog
{
    Q_OBJECT

public:
    explicit storage_rege(QWidget *parent = nullptr);
    ~storage_rege();
private slots:
    void on_listView_clicked(const QModelIndex &index);

    void on_lineEdit_returnPressed();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();
private:
    Ui::storage_rege *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QCompleter *comp;
};

#endif // STORAGE_REGE_H
