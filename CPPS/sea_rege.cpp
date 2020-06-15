#include "sea_rege.h"
#include "ui_sea_rege.h"
#include <rege.h>
sea_rege::sea_rege(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sea_rege)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    if(db.open()){

    }
    }
    model = new QSqlQueryModel(this);
    model->setQuery("SELECT name FROM season ",db);
    ui->listView->setModel(model);

    comp = new QCompleter;
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);
}

sea_rege::~sea_rege()
{
    delete ui;
}

void sea_rege::on_lineEdit_returnPressed()
{
    if(!ui->lineEdit->text().isEmpty()){
    QList<QModelIndex> modelIndexList = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(ui->lineEdit->text()), -1, Qt::MatchStartsWith);
    foreach(QModelIndex modelIndex, modelIndexList)
       ui->listView->selectionModel()->select(modelIndex, QItemSelectionModel::Select);}else {
        ui->listView->selectionModel()->clearSelection();
        }
}

void sea_rege::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
    }
    else{
        QString sea_name = ui->lineEdit->text();
        QSqlQuery qry (db);
        qry.prepare("DELETE FROM season WHERE name = :sea");
        qry.bindValue(":sea",sea_name);
        qry.exec();
    }
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM season ",db);
    ui->listView->setModel(model);
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);
    emit addeditems();

    
}

void sea_rege::on_pushButton_2_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
    }
    else{
        QString sea_name = ui->lineEdit->text();
        QSqlQuery qry (db);
        qry.prepare("INSERT INTO season (name) VALUES(:sea) ");
        qry.bindValue(":sea",sea_name);
        qry.exec();
    }
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM season ",db);
    ui->listView->setModel(model);
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);
    emit addeditems();

}

void sea_rege::on_pushButton_3_clicked()
{
    ui->listView->selectionModel()->clearSelection();
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM season ",db);
    ui->listView->setModel(model);
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);


}

void sea_rege::on_listView_clicked(const QModelIndex &index)
{
    ui->lineEdit->setText(model->data(index).toString());

}
