#ifndef SQLITE_SET_H
#define SQLITE_SET_H

#include <QDialog>
#include<QFileDialog>
namespace Ui {
class sqlite_set;
}

class sqlite_set : public QDialog
{
    Q_OBJECT

public:
    explicit sqlite_set(QWidget *parent = 0);
    ~sqlite_set();
    QString loc_data;
    QString set_data;

private slots:


    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::sqlite_set *ui;

};

#endif // SQLITE_SET_H
