#ifndef SETTING_WINDOW_H
#define SETTING_WINDOW_H

#include <QMainWindow>
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
#include<qtrpt.h>
#include<qshortcut.h>
#include<qprinterinfo.h>
#include<qfiledialog.h>
namespace Ui {
class setting_window;
}

class setting_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit setting_window(QWidget *parent = 0);
    ~setting_window();


private slots:

    void on_pushButton_clicked();

    void on_toolButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::setting_window *ui;
    QPrinterInfo *info_print;
    QSqlDatabase db;
    int type;
    QString loc_data;

};

#endif // SETTING_WINDOW_H
