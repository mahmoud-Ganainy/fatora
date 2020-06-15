#include "supp_rege.h"
#include "ui_supp_rege.h"

supp_rege::supp_rege(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::supp_rege)
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

supp_rege::~supp_rege()
{
    delete ui;
}

void supp_rege::on_pushButton_clicked()
{
 QString name = ui->supp_name->text();
 QString address = ui->address->text();
 QString phone = ui->phone->text();
 QString id = ui->supp_id->text();
 QString first = ui->first_time->text();
 checkStat();
 int recCount = 0;
 QSqlQuery qry(db);
 qry.prepare("SELECT * FROM supplier WHERE id = :supp");
 qry.bindValue(":supp",id);
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
     qry.prepare("INSERT INTO supplier(balance,name,id,phone,address,old_balance,type,stop_deal,stop_payments,stop_return)VALUES(:bala,:supp_name,:supp_id,:phone,:address,:first_time,:type,:stop_deal,:stop_payment,:stop_return)");
     qry.bindValue(":supp_id",id);
     qry.bindValue(":bala",first);
     qry.bindValue(":supp_name",name);
     qry.bindValue(":phone",phone);
     qry.bindValue(":address",address);
     qry.bindValue(":first_time",first);
     qry.bindValue(":type",type);
     qry.bindValue(":stop_deal",stop_deal);
     qry.bindValue(":stop_payment",stop_payment);
     qry.bindValue(":stop_return",stop_return);
     if(!qry.exec())
      qDebug() << "Error inserting into the table:\n" << qry.lastError();
 }
 else{
		QSqlQuery qry(db);
        qry.prepare("UPDATE supplier SET name = :name,phone = :phone,address = :address,type = :type,stop_deal  = :stop_deal,stop_payments = :stop_payment,stop_return = :stop_return,old_balance= old_balance+ :balance,balance = balance + :balance WHERE id = :id");
		qry.bindValue(":id",id);
		qry.bindValue(":balance",first);
		qry.bindValue(":name",name);
		qry.bindValue(":phone",phone);
		qry.bindValue(":address",address);
		qry.bindValue(":type",type);
		qry.bindValue(":stop_deal",stop_deal);
		qry.bindValue(":stop_payment",stop_payment);
		qry.bindValue(":stop_return",stop_return);
        if(!qry.exec())
         qDebug() << "Error inserting into the table:\n" << qry.lastError();
 }
 clearall();
 emit addeditems();

}
void supp_rege::clearall()
{
    ui->supp_name->clear();
    ui->supp_id->clear();
    ui->phone->clear();
    ui->address->clear();
    ui->first_time->clear();
    ui->bala->clear();
    ui->stop_deal->setChecked(false);
    ui->stop_pay->setChecked(false);
    ui->stop_return->setChecked(false);
    ui->item->setAutoExclusive(false);
    ui->item->setChecked(false);
    ui->item->setAutoExclusive(true);
    ui->serv->setAutoExclusive(false);
    ui->serv->setChecked(false);
    ui->serv->setAutoExclusive(true);
    ui->mix->setAutoExclusive(false);
    ui->mix->setChecked(false);
    ui->mix->setAutoExclusive(true);
    db.close();
    db.open();
    model->setQuery("SELECT name FROM supplier ",db);
    ui->listView->setModel(model);
    QSqlQuery qry(db);
    qry.prepare("SELECT MAX(id) from supplier");
    if (qry.exec())
    {
        while (qry.next())
        {
            int test7;
            test7 = qry.value(0).toInt();
            test7+=1;
            QString test6 = QString::number(test7);
            ui->supp_id->setText(test6);

        }
    }
    else{
          qDebug() << "Error inserting into the table:\n" << qry.lastError();
   }
    ui->bala->setText("0");
    ui->first_time->setText("0");

}
void supp_rege::checkStat()
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
  if(ui->item->isChecked()){
      type = 1;
  }
  else if (ui->serv->isChecked()) {
      type = 2;

  }
  else{
      type = 3;
  }
}

void supp_rege::on_listView_activated(const QModelIndex &index)
{
    QString name = ui->listView->model()->data(index).toString();
    qDebug()<<name;
    QSqlQuery qry(db);
    qry.prepare("SELECT id,balance,address,phone,type,stop_deal,stop_payments,stop_return,old_balance FROM supplier WHERE name ='"+name+"' ");
    if (qry.exec())
    {
        while (qry.next())
        {
            ui->supp_name->setText(name);
            ui->supp_id->setText(qry.value(0).toString());
            ui->bala->setText(qry.value(1).toString());
            ui->address->setText(qry.value(2).toString());
            ui->phone->setText(qry.value(3).toString());
             type = qry.value(4).toInt();
             stop_deal = qry.value(5).toInt();
             stop_payment = qry.value(6).toInt();
             stop_return = qry.value(7).toInt();
             //ui->first_time->setText(qry.value(8).toString());
             if(type == 1){
                 ui->item->setChecked(true);
             }
             else if(type == 2){
                 ui->serv->setChecked(true);
             }
             else if(type == 3){
                 ui->mix->setChecked(true);
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


        }
    }
    else{
          qDebug() << "Error inserting into the table:\n" << qry.lastError();
   }

}

void supp_rege::on_pushButton_2_clicked()
{
    clearall();
}

void supp_rege::on_pushButton_3_clicked()
{
    if (QMessageBox::question(this, tr("delete?!!!"),
        tr("Are You Sure You WANT TO delete!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
                QString id = ui->supp_id->text();
                QSqlQuery qry(db);
                qry.prepare("DELETE FROM supplier WHERE id = "+id+"");
                if (qry.exec())
                {
                }
                else{
                      qDebug() << "Error inserting into the table:\n" << qry.lastError();
                }
                clearall();
       }
    emit addeditems();

}
