#include "class_rege.h"
#include "ui_class_rege.h"
#include <rege.h>
class_rege::class_rege(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::class_rege)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    if(db.open()){
    }
    }
    model = new QSqlQueryModel(this);
    model->setQuery("SELECT name,id FROM class ",db);
    ui->listView->setModel(model);

    comp = new QCompleter;
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);
}

class_rege::~class_rege()
{
    delete ui;
}

void class_rege::on_lineEdit_returnPressed()
{
    if(!ui->lineEdit->text().isEmpty()){
    QList<QModelIndex> modelIndexList = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(ui->lineEdit->text()), -1, Qt::MatchStartsWith);
    foreach(QModelIndex modelIndex, modelIndexList)
       ui->listView->selectionModel()->select(modelIndex, QItemSelectionModel::Select);}else {
        ui->listView->selectionModel()->clearSelection();
        }

}

void class_rege::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
    }
    else{
        QString class_name = ui->lineEdit->text();
        QSqlQuery qry(db);
        qry.prepare("DELETE FROM class WHERE name = :class");
        qry.bindValue(":class",class_name);
        qry.exec();
    }
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM class ",db);
    ui->listView->setModel(model);
    comp = new QCompleter;
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);
    emit addeditems();


}

void class_rege::on_pushButton_2_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
    }
    else{
        QString class_name = ui->lineEdit->text();
        QSqlQuery qry(db);
        qry.prepare("INSERT INTO class (name) VALUES(:class) ");
        qry.bindValue(":class",class_name);
        qry.exec();
    }
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM class ",db);
    ui->listView->setModel(model);
    comp = new QCompleter;
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);
    emit addeditems();

}

void class_rege::on_pushButton_3_clicked()
{
    ui->listView->selectionModel()->clearSelection();
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM class ",db);
    ui->listView->setModel(model);
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);

}

void class_rege::on_listView_clicked(const QModelIndex &index)
{
    ui->lineEdit->setText(model->data(index).toString());

}
