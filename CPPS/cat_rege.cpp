#include "cat_rege.h"
#include "ui_cat_rege.h"
#include <rege.h>
cat_rege::cat_rege(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cat_rege)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    if(db.open()){

    }
    }
    model = new QSqlQueryModel(this);
    model->setQuery("SELECT name,id FROM category ",db);
    ui->listView->setModel(model);

    comp = new QCompleter;
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);

}

cat_rege::~cat_rege()
{
    delete ui;
}

void cat_rege::on_lineEdit_returnPressed()
{
    if(!ui->lineEdit->text().isEmpty()){
    QList<QModelIndex> modelIndexList = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(ui->lineEdit->text()), -1, Qt::MatchStartsWith);
    foreach(QModelIndex modelIndex, modelIndexList)
        ui->listView->selectionModel()->select(modelIndex, QItemSelectionModel::Select);


        }else {
        ui->listView->selectionModel()->clearSelection();
        }
}

void cat_rege::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
    }
    else{
        QString cat_name = ui->lineEdit->text();
        QSqlQuery qry(db);
        qry.prepare("DELETE FROM category WHERE name = :cat");
        qry.bindValue(":cat",cat_name);
        if(qry.exec()){}else {
        qDebug() << "Error inserting into the table:\n" << qry.lastError();
        }

    }
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM category ",db);
    ui->listView->setModel(model);
    emit addeditems();

}

void cat_rege::on_pushButton_2_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
    }
    else{
        QString cat_name = ui->lineEdit->text();
        QSqlQuery qry(db);
        qry.prepare("INSERT INTO category (name) VALUES(:cat) ");
        qry.bindValue(":cat",cat_name);
        qry.exec();
    }
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM category ",db);
    ui->listView->setModel(model);
    comp = new QCompleter;
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);
    emit addeditems();

}

void cat_rege::on_pushButton_3_clicked()
{
   ui->listView->selectionModel()->clearSelection();
   ui->lineEdit->clear();
   db.close();
   db.open();
   model->setQuery("SELECT name,id FROM category ",db);
   ui->listView->setModel(model);
   comp->setModel(model);
   ui->lineEdit->setCompleter(comp);
}

void cat_rege::on_listView_clicked(const QModelIndex &index)
{
    ui->lineEdit->setText(model->data(index).toString());

}
