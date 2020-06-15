#ifndef SEARCH_ITEMS_H
#define SEARCH_ITEMS_H

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
class search_items;
}

class search_items : public QDialog
{
    Q_OBJECT

public:
    explicit search_items(QWidget *parent = 0);
    ~search_items();
    QString find_in;

private slots:


    void on_search_in_returnPressed();

private:
    Ui::search_items *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    QCompleter *comp;
};

#endif // SEARCH_ITEMS_H
