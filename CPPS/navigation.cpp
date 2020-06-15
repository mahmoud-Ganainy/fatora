#include "navigation.h"
#include "ui_navigation.h"

navigation::navigation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::navigation)
{
    ui->setupUi(this);
}

navigation::~navigation()
{
    delete ui;
}
