#include "search_items.h"
#include "ui_search_items.h"

search_items::search_items(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::search_items)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    db.open();
    }
    model = new QSqlQueryModel(this);
    comp = new QCompleter(this);
    model->setQuery("SELECT name FROM items ",db);
    comp->setModel(model);
    ui->search_in->setCompleter(comp);
}

search_items::~search_items()
{
    delete ui;
}


void search_items::on_search_in_returnPressed()
{
   find_in = ui->search_in->text();
   this->accept();

}
