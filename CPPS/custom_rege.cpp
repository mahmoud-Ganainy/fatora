#include "custom_rege.h"
#include "ui_custom_rege.h"

custom_rege::custom_rege(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::custom_rege)
{
    ui->setupUi(this);
    this->setStyleSheet(QString::fromUtf8("font: 12pt FreeMono;"));
    db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    if(db.open()){

        ui->statusbar->showMessage("DataBase Connected!",2000);
    }
    else {
        ui->statusbar->showMessage("Error Error",2000);
    }
    }
    model = new QSqlQueryModel(this);
    clearall();
}

custom_rege::~custom_rege()
{
    delete ui;
}

void custom_rege::clearall()
{
    ui->custom_name->clear();
    ui->custom_id->clear();
    ui->phone->clear();
    ui->address->clear();
    ui->first_time->clear();
    ui->bala->clear();
    ui->limit->clear();
    ui->discount_perc->clear();
    ui->stop_deal->setChecked(false);
    ui->stop_pay->setChecked(false);
    ui->stop_return->setChecked(false);
    ui->stop_sales->setChecked(false);
    ui->indi->setAutoExclusive(false);
    ui->indi->setChecked(false);
    ui->indi->setAutoExclusive(true);
    ui->shop->setAutoExclusive(false);
    ui->shop->setChecked(false);
    ui->shop->setAutoExclusive(true);
    ui->comp->setAutoExclusive(false);
    ui->comp->setChecked(false);
    ui->comp->setAutoExclusive(true);
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM customers ",db);
    ui->listView->setModel(model);
    QSqlQuery qry(db);
    qry.prepare("SELECT MAX(id) from customers");
    if (qry.exec())
    {
        while (qry.next())
        {
            int test7;
            test7 = qry.value(0).toInt();
            test7+=1;
            QString test6 = QString::number(test7);
            ui->custom_id->setText(test6);

        }
    }
    else{
          qDebug() << "Error inserting into the table:\n" << qry.lastError();
   }
    ui->bala->setText("0");
    ui->first_time->setText("0");
    ui->discount_perc->setText("0");
    ui->limit->setText("0");

}
void custom_rege::checkStat()
{
  if(ui->stop_deal->isChecked())
  {
      stop_deal = 1;
  }
  else{
      stop_deal = 0;
  }
  if(ui->stop_pay->isChecked()){
      stop_payment = 1;

  }
  else {
      stop_payment = 0;
  }
  if(ui->stop_return->isChecked()){
      stop_return = 1;

  }
  else {
      stop_return = 0;
  }
  if(ui->stop_sales->isChecked()){
      stop_sales = 1;

  }
  else {
      stop_sales = 0;
  }
  if(ui->indi->isChecked()){
      type = 1;
  }
  else if (ui->comp->isChecked()) {
      type = 2;

  }
  else{
      type = 3;
  }
}

void custom_rege::on_pushButton_clicked()
{
    QString name = ui->custom_name->text();
    QString address = ui->address->text();
    QString phone = ui->phone->text();
    QString id = ui->custom_id->text();
    QString first = ui->first_time->text();
    QString limit = ui->limit->text();
    QString discount = ui->discount_perc->text();
    checkStat();
    int recCount = 0;
    QSqlQuery qry(db);
    qry.prepare("SELECT * FROM customers WHERE id = :custom");
    qry.bindValue(":custom",id);
    if( qry.exec() ){
        while (qry.next())
        {
            recCount++;
        }
    }

    else{
          qDebug() << "Error inserting into the table:\n" << qry.lastError();
   }
    if(recCount==0){
        QSqlQuery qry(db);
        qry.prepare("INSERT INTO customers(balance,name,id,phone,address,old_balance,type,stop_deal,stop_payments,stop_return,stop_sales,customer_limit,discount_perc)VALUES(:bala,:custom_name,:custom_id,:phone,:address,:first_time,:type,:stop_deal,:stop_payment,:stop_return,:stop_sales,:custom_limit,:discount_perc)");
        qry.bindValue(":custom_id",id);
        qry.bindValue(":bala",first);
        qry.bindValue(":custom_name",name);
        qry.bindValue(":phone",phone);
        qry.bindValue(":address",address);
        qry.bindValue(":first_time",first);
        qry.bindValue(":custom_limit",limit);
        qry.bindValue(":type",type);
        qry.bindValue(":stop_deal",stop_deal);
        qry.bindValue(":stop_payment",stop_payment);
        qry.bindValue(":stop_return",stop_return);
        qry.bindValue(":stop_sales",stop_sales);
        qry.bindValue(":discount_perc",discount);
        if(!qry.exec())
         qDebug() << "Error inserting into the table:\n" << qry.lastError();
    }
    else{
        QSqlQuery qry(db);
        qry.prepare("UPDATE customers SET name = :custom_name ,phone = :phone,address = :address,type = :type,stop_deal = :stop_deal,stop_payments = :stop_payment,stop_return = :stop_return,stop_sales = :stop_sales,customer_limit = :custom_limit,discount_perc = :discount_perc,old_balance = old_balance + :balance,balance = balance + :balance WHERE id = :custom_id");
        qry.bindValue(":custom_id",id);
        qry.bindValue(":custom_name",name);
        qry.bindValue(":phone",phone);
        qry.bindValue(":address",address);
        qry.bindValue(":custom_limit",limit);
        qry.bindValue(":type",type);
        qry.bindValue(":stop_deal",stop_deal);
        qry.bindValue(":stop_payment",stop_payment);
        qry.bindValue(":stop_return",stop_return);
        qry.bindValue(":stop_sales",stop_sales);
        qry.bindValue(":discount_perc",discount);
		qry.bindValue(":balance",first);
        if(!qry.exec())
         qDebug() << "Error inserting into the table:\n" << qry.lastError();
    }
    clearall();
    emit addeditems();
}

void custom_rege::on_listView_activated(const QModelIndex &index)
{
    QString name = ui->listView->model()->data(index).toString();
    qDebug()<<name;
    QSqlQuery qry(db);
    qry.prepare("SELECT id,balance,address,phone,type,stop_deal,stop_payments,stop_return,stop_sales,old_balance,customer_limit,discount_perc FROM customers WHERE name ='"+name+"' ");
    if (qry.exec())
    {
        while (qry.next())
        {
            ui->custom_name->setText(name);
            ui->custom_id->setText(qry.value(0).toString());
            ui->bala->setText(qry.value(1).toString());
            ui->address->setText(qry.value(2).toString());
            ui->phone->setText(qry.value(3).toString());
             type = qry.value(4).toInt();
             stop_deal = qry.value(5).toInt();
             stop_payment = qry.value(6).toInt();
             stop_return = qry.value(7).toInt();
             stop_sales = qry.value(8).toInt();
            //ui->first_time->setText(qry.value(9).toString());
            ui->limit->setText(qry.value(10).toString());
            ui->discount_perc->setText(qry.value(11).toString());
             if(type == 1){
                 ui->indi->setChecked(true);
             }
             else if(type == 2){
                 ui->comp->setChecked(true);
             }
             else if(type == 3){
                 ui->shop->setChecked(true);
             }
             if(stop_deal == 0){
                 ui->stop_deal->setChecked(false);
             }
             else{
                 ui->stop_deal->setChecked(true);
             }
             if(stop_payment == 0){
                 ui->stop_pay->setChecked(false);
             }
             else{
                 ui->stop_pay->setChecked(true);
             }
             if(stop_return == 0){
                 ui->stop_return->setChecked(false);
             }
             else{
                 ui->stop_return->setChecked(true);
             }
             if(stop_sales == 0){
                 ui->stop_sales->setChecked(false);
             }
             else{
                 ui->stop_sales->setChecked(true);
             }


        }
    }
    else{
          qDebug() << "Error inserting into the table:\n" << qry.lastError();
   }
}

void custom_rege::on_pushButton_2_clicked()
{
    clearall();
}

void custom_rege::on_pushButton_3_clicked()
{
    if (QMessageBox::question(this, tr("delete?!!!"),
        tr("Are You Sure You WANT TO delete!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
                QString id = ui->custom_id->text();
                QSqlQuery qry(db);
                qry.prepare("DELETE FROM customers WHERE id = "+id+"");
                if (qry.exec())
                {
                }
                else{
                      qDebug() << "Error inserting into the table:\n" << qry.lastError();
                }
                clearall();
                emit addeditems();

       }

}
