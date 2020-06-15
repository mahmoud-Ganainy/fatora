#include "employee_man.h"
#include "ui_employee_man.h"

employee_man::employee_man(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::employee_man)
{
    ui->setupUi(this);
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

employee_man::~employee_man()
{
    delete ui;
}

void employee_man::clearall()
{
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM employees ",db);
    ui->listView->setModel(model);
}

void employee_man::checkStat()
{

}

void employee_man::on_listView_activated(const QModelIndex &index)
{
    QString id = ui->listView->model()->index(index.row(),1).data().toString();
    QString name = ui->listView->model()->index(index.row(),0).data().toString();
    QSqlQuery qry(db);
    qry.prepare("SELECT id,name,password FROM users WHERE employee_id ='"+id+"' ");
    if (qry.exec())
    {
        while (qry.next())
        {

        }
    }
    else {
        qDebug() << "Error inserting into the table:\n" << qry.lastError();
    }
    QSqlQuery qry2(db);
    qry2.prepare("SELECT can_rege_items,can_rege_supp,can_rege_custom,can_buy,can_supp_pay,can_custom_pay,can_expense,_can_get_rep FROM roles WHERE user_id ='"+id+"' ");
    if (qry2.exec())
    {
        while (qry2.next())
        {

        }
    }
    else {
        qDebug() << "Error inserting into the table:\n" << qry2.lastError();
    }
}

void employee_man::on_save_clicked()
{

}
