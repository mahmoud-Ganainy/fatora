#include "sqlite_set.h"
#include "ui_sqlite_set.h"

sqlite_set::sqlite_set(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sqlite_set)
{
    ui->setupUi(this);

}

sqlite_set::~sqlite_set()
{
    delete ui;
}



void sqlite_set::on_toolButton_clicked()
{


}

void sqlite_set::on_toolButton_2_clicked()
{
    QString s2 =
        QFileDialog::getOpenFileName(this, "Open a file", "directoryToOpen",
            "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;DB files (*.db *.sqlite)");
    ui->lineEdit_2->setText(s2);
    set_data = s2;
}
