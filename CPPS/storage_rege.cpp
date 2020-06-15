#include "storage_rege.h"
#include "ui_storage_rege.h"

storage_rege::storage_rege(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::storage_rege)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    if(db.open()){

    }
    }
    model = new QSqlQueryModel(this);
    model->setQuery("SELECT name,id FROM storage ",db);
    ui->listView->setModel(model);

    comp = new QCompleter;
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);
}

storage_rege::~storage_rege()
{
    delete ui;
}
void storage_rege::on_listView_clicked(const QModelIndex &index)
{
    ui->lineEdit->setText(model->data(index).toString());

}

void storage_rege::on_lineEdit_returnPressed()
{
    if(!ui->lineEdit->text().isEmpty()){
    QList<QModelIndex> modelIndexList = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(ui->lineEdit->text()), -1, Qt::MatchStartsWith);
    foreach(QModelIndex modelIndex, modelIndexList)
       ui->listView->selectionModel()->select(modelIndex, QItemSelectionModel::Select);}else {
        ui->listView->selectionModel()->clearSelection();
        }
}

void storage_rege::on_pushButton_2_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
    }
    else{
        QString name = ui->lineEdit->text();
        QSqlQuery qry(db);
        qry.prepare("INSERT INTO storage (name) VALUES(:name) ");
        qry.bindValue(":name",name);
        qry.exec();
    }
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM storage ",db);
    ui->listView->setModel(model);
    comp = new QCompleter;
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);
}

void storage_rege::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
    }
    else{
        QString name = ui->lineEdit->text();
        QSqlQuery qry(db);
        qry.prepare("DELETE FROM storage WHERE name = :name");
        qry.bindValue(":name",name);
        if(qry.exec()){}else {
        qDebug() << "Error inserting into the table:\n" << qry.lastError();
        }

    }
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM storage ",db);
    ui->listView->setModel(model);
}

void storage_rege::on_pushButton_3_clicked()
{
    ui->listView->selectionModel()->clearSelection();
    ui->lineEdit->clear();
    db.close();
    db.open();
    model->setQuery("SELECT name,id FROM storage ",db);
    ui->listView->setModel(model);
    comp->setModel(model);
    ui->lineEdit->setCompleter(comp);
}
