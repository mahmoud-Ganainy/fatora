#ifndef INVOICE_SEARCH_H
#define INVOICE_SEARCH_H

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
class invoice_search;
}

class invoice_search : public QDialog
{
    Q_OBJECT

public:
    explicit invoice_search(QWidget *parent = nullptr);
    ~invoice_search();
	QString id_out;
	int type;
    int search_type = 0;
    void clear_all();

private slots:

    void on_type_currentIndexChanged(int index);

    void on_pushButton_clicked();

private:
    Ui::invoice_search *ui;
	QSqlDatabase db;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
};

#endif // INVOICE_SEARCH_H
