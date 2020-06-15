#ifndef CLASS_REGE_H
#define CLASS_REGE_H

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
class class_rege;
}

class class_rege : public QDialog
{
    Q_OBJECT

public:
    explicit class_rege(QWidget *parent = 0);
    ~class_rege();
signals:
    void addeditems();
private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::class_rege *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QCompleter *comp;
};

#endif // CLASS_REGE_H
