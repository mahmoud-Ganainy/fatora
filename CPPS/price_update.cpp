#include "price_update.h"
#include "ui_price_update.h"

price_update::price_update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::price_update)
{
    ui->setupUi(this);
    update_state = 0;
}

price_update::~price_update()
{
    delete ui;
}

void price_update::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked()){
        update_state = 1;
    }
    else if (!ui->checkBox->isChecked()) {
        update_state = 0;
    }

}

void price_update::on_buy_price_editingFinished()
{

}

void price_update::on_profit_editingFinished()
{
    double buy_price = ui->buy_price->text().toDouble();
    double prof_num = ui->profit->text().toDouble();
    if(!ui->profit->text().isEmpty()){
        double profit_perc_lo = (prof_num/buy_price)*100;
        double sell_price = buy_price + prof_num;
        ui->sell_price->setText(QString::number(sell_price));
        ui->profit_perc->setText(QString::number(profit_perc_lo));
        sell = ui->sell_price->text().toDouble();
        buy = ui->buy_price->text().toDouble();
        profit = ui->profit->text().toDouble();
        profit_perc = ui->profit_perc->text().toDouble();
    }
}

void price_update::on_profit_perc_editingFinished()
{
    double buy_price = ui->buy_price->text().toDouble();
    double prof_num = ui->profit_perc->text().toDouble();
    if(!ui->profit_perc->text().isEmpty()){
    double prof_inc = buy_price * prof_num/100;
    double sell_price = buy_price + prof_inc;
    ui->sell_price->setText(QString::number(sell_price));
    ui->profit->setText(QString::number(prof_inc));
    sell = ui->sell_price->text().toDouble();
    buy = ui->buy_price->text().toDouble();
    profit = ui->profit->text().toDouble();
    profit_perc = ui->profit_perc->text().toDouble();

    }
}

void price_update::on_sell_price_editingFinished()
{
    if(!ui->buy_price->text().isEmpty()){
 double buy_price = ui->buy_price->text().toDouble();
 double sell_price = ui->sell_price->text().toDouble();
 double profit_lo = sell_price - buy_price;
 double profit_perc_lo = (profit_lo/buy_price)*100;
 ui->profit->setText(QString::number(profit_lo));
 ui->profit_perc->setText(QString::number(profit_perc_lo));
 sell = ui->sell_price->text().toDouble();
 buy = ui->buy_price->text().toDouble();
 profit = ui->profit->text().toDouble();
 profit_perc = ui->profit_perc->text().toDouble();}
}

void price_update::on_pushButton_clicked()
{
    this->accept();
    ui->profit->clear();
    ui->buy_price->clear();
    ui->sell_price->clear();
    ui->profit_perc->clear();
    ui->checkBox->setChecked(false);
    update_state = 0;
}

void price_update::on_pushButton_2_clicked()
{
    ui->profit->clear();
    ui->buy_price->clear();
    ui->sell_price->clear();
    ui->profit_perc->clear();
    ui->checkBox->setChecked(false);
    update_state = 0;
    this->reject();
}




void price_update::on_buy_price_returnPressed()
{
  ui->profit->setFocus();
}

void price_update::on_profit_returnPressed()
{
    ui->profit_perc->setFocus();
}

void price_update::on_profit_perc_returnPressed()
{
   ui->sell_price->setFocus();
}

void price_update::on_sell_price_returnPressed()
{
    ui->pushButton->click();
}
