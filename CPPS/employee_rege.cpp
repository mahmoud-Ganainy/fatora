#include "employee_rege.h"
#include "ui_employee_rege.h"

employee_rege::employee_rege(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::employee_rege)
{
    ui->setupUi(this);
    //this->setStyleSheet(QString::fromUtf8("font: 12pt FreeMono;"));
    db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    if(db.open()){

    }
    else {

    }
    }
    model = new QSqlQueryModel(this);
    clearall();
}

employee_rege::~employee_rege()
{
    delete ui;
}

void employee_rege::clearall()
{
     ui->employee_name->clear();
     ui->employee_id->clear();
     ui->address->clear();
     ui->phone->clear();
     ui->blood->setCurrentIndex(0);
     ui->national_id->clear();
     ui->salary->clear();
     ui->emp_date->setDate(QDate::currentDate());
     ui->DOB->setDate(QDate::currentDate());
     ui->POB->clear();
     ui->army_stat->setChecked(false);
     ui->insurance_stat->setChecked(false);
     ui->appear_as_seller->setChecked(false);
     ui->appear_as_cashier->setChecked(false);

    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM employees ",db);
    ui->listView->setModel(model);
    QSqlQuery qry(db);
    qry.prepare("SELECT MAX(id) from employees");
    if (qry.exec())
    {
        while (qry.next())
        {
            int test7;
            test7 = qry.value(0).toInt();
            test7+=1;
            QString test6 = QString::number(test7);
            ui->employee_id->setText(test6);

        }
    }
    else{
          qDebug() << "Error inserting into the table:\n" << qry.lastError();
    }
    ui->salary->setText("0");
}

void employee_rege::checkStat()
{
    if(ui->insurance_stat->isChecked() == 1){

        insurance = 1;
    }else {
        insurance = 0;
    }
    if(ui->appear_as_cashier->isChecked() == 1){
        cashier = 1;
    }else {
        cashier = 0;
    }
    if(ui->army_stat->isChecked() == 1){
            army = 1;
    }else {
        army = 0;
    }
    if(ui->appear_as_seller->isChecked() == 1){
       seller = 1;

    }else {
       seller = 0;
    }
}

void employee_rege::on_listView_activated(const QModelIndex &index)
{
    QString id = ui->listView->model()->index(index.row(),1).data().toString();
    qDebug()<<id;
    QSqlQuery qry(db);
    qry.prepare("SELECT name,address,phone,date_of_birth,place_of_birth,insurance,appear_as_cashier,blood_type,army_status,national_id,salary,date_of_emp,seller FROM employees WHERE id ='"+id+"' ");
    if (qry.exec())
    {
        while (qry.next())
        {
            ui->employee_id->setText(id);
            ui->employee_name->setText(qry.value(0).toString());
            ui->address->setText(qry.value(1).toString());
            ui->phone->setText(qry.value(2).toString());
            ui->DOB->setDate(qry.value(3).toDate());
            ui->POB->setText(qry.value(4).toString());
            if(qry.value(5).toInt() == 1){
                ui->insurance_stat->setChecked(true);

            }else {
                ui->insurance_stat->setChecked(false);
            }
            if(qry.value(6).toInt() == 1){
                ui->appear_as_cashier->setChecked(true);

            }else {
                ui->appear_as_cashier->setChecked(false);
            }
            ui->blood->setCurrentText(qry.value(7).toString());
            if(qry.value(8).toInt() == 1){
                ui->army_stat->setChecked(true);

            }else {
                ui->army_stat->setChecked(false);
            }
            ui->national_id->setText(qry.value(9).toString());
            ui->salary->setText(qry.value(10).toString());
            ui->emp_date->setDate(qry.value(11).toDate());
            if(qry.value(12).toInt() == 1){
                ui->appear_as_seller->setChecked(true);

            }else {
                ui->appear_as_seller->setChecked(false);
            }
        }
    }
    else {
        qDebug() << "Error inserting into the table:\n" << qry.lastError();
    }
}

void employee_rege::on_save_clicked()
{
    QString name = ui->employee_name->text();
    QString id = ui->employee_id->text();
    QString address = ui->address->text();
    QString phone = ui->phone->text();
    QString blood = ui->blood->currentText();
    QString national_id = ui->national_id->text();
    QString salary = ui->salary->text();
    QDate emp_date= ui->emp_date->date();
    QDate DOB = ui->DOB->date();
    QString POB= ui->POB->text();
    checkStat();
    int recCount = 0;
    QSqlQuery qry(db);
    qry.prepare("SELECT * FROM employees WHERE id = :id");
    qry.bindValue(":id",id);
    if( qry.exec() ){
        while (qry.next())
        {
            recCount++;
        }
    }

    else{
          qDebug() << "Error :\n" << qry.lastError();
   }
    if(recCount==0){
        QSqlQuery qry(db);
        qry.prepare("INSERT INTO employees (name,address,phone,date_of_birth,place_of_birth,insurance,appear_as_cashier,blood_type,army_status,national_id,salary,date_of_emp,seller)VALUES(:name,:address,:phone,:date_of_birth,:place_of_birth,:insurance,:appear_as_cashier,:blood_type,:army_status,:national_id,:salary,:date_of_emp,:seller)");
        qry.bindValue(":name",name);
        qry.bindValue(":address",address);
        qry.bindValue(":phone",phone);
        qry.bindValue(":date_of_birth",DOB);
        qry.bindValue(":place_of_birth",POB);
        qry.bindValue(":insurance",insurance);
        qry.bindValue(":appear_as_cashier",cashier);
        qry.bindValue(":blood_type",blood);
        qry.bindValue(":army_status",army);
        qry.bindValue(":national_id",national_id);
        qry.bindValue(":salary",salary);
        qry.bindValue(":date_of_emp",emp_date);
        qry.bindValue(":seller",seller);

        if(!qry.exec())
         QMessageBox::warning(this,tr("FATEL ERROR"),qry.lastError().text(),QMessageBox::Ok);

    }else {
        QSqlQuery qry(db);
        qry.prepare("UPDATE employees SET name = :name,address = :address,phone = :phone,date_of_birth = :date_of_birth,place_of_birth = :place_of_birth,insurance = :insurance,appear_as_cashier = :appear_as_cashier,blood_type = :blood_type,army_status = :army_status,national_id = :national_id,salary = :salary,date_of_emp = :date_of_emp,seller = :seller WHERE id ='"+id+"'");

        qry.bindValue(":name",name);
        qry.bindValue(":address",address);
        qry.bindValue(":phone",phone);
        qry.bindValue(":date_of_birth",DOB);
        qry.bindValue(":place_of_birth",POB);
        qry.bindValue(":insurance",insurance);
        qry.bindValue(":appear_as_cashier",cashier);
        qry.bindValue(":blood_type",blood);
        qry.bindValue(":army_status",army);
        qry.bindValue(":national_id",national_id);
        qry.bindValue(":salary",salary);
        qry.bindValue(":date_of_emp",emp_date);
        qry.bindValue(":seller",seller);
        if(!qry.exec())
         QMessageBox::warning(this,tr("FATEL ERROR"),qry.lastError().text(),QMessageBox::Ok);
    }
    clearall();
}

void employee_rege::on_clear_clicked()
{
    clearall();
}

void employee_rege::on_delete_2_clicked()
{
    if (QMessageBox::question(this, tr("delete?!!!"),
        tr("Are You Sure You WANT TO delete!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
                QString id = ui->employee_id->text();
                QSqlQuery qry(db);
                qry.prepare("DELETE FROM employees WHERE id = "+id+"");
                if (qry.exec())
                {
                }
                else{
                      qDebug() << "Error inserting into the table:\n" << qry.lastError();
                }
                clearall();


       }
}
