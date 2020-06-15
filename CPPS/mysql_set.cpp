#include "mysql_set.h"
#include "ui_mysql_set.h"

mysql_set::mysql_set(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mysql_set)
{
    ui->setupUi(this);
}

mysql_set::~mysql_set()
{
    delete ui;
}
