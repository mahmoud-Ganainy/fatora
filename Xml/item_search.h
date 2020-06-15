#ifndef ITEM_SEARCH_H
#define ITEM_SEARCH_H
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
class item_search;
}

class item_search : public QDialog
{
    Q_OBJECT

public:
    explicit item_search(QWidget *parent = nullptr);
    ~item_search();
	QString find_in;


private slots:


    void on_search_in_returnPressed();
private:
    Ui::item_search *ui;
	QSqlDatabase db;
    QSqlQueryModel *model;
    QCompleter *comp;
};

#endif // ITEM_SEARCH_H
