#ifndef CAT_REGE_H
#define CAT_REGE_H

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
class cat_rege;
}

class cat_rege : public QDialog
{
    Q_OBJECT

public:
    explicit cat_rege(QWidget *parent = 0);
    ~cat_rege();
signals:
    void addeditems();
private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::cat_rege *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    QSqlQueryModel *model2;
    QCompleter *comp;
};

#endif // CAT_REGE_H
