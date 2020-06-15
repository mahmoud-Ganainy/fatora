#include "data_sheet.h"
#include "ui_data_sheet.h"

data_sheet::data_sheet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::data_sheet)
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
    //QSqlTableModel *GetTable = new QSqlTableModel(this,db);
    /*GetTable->setTable("sales_items");
    GetTable->setFilter("rec_no = "+test8+" ");*/
    QSqlQuery qry(db);
    qry.prepare("SELECT * FROM `item_balance` join sub_items on sub_items.id=item_balance.sub_item_id");
    //GetTable->setQuery(qry);
    model->setQuery(qry);
    ui->tableView->setModel(model);


}

data_sheet::~data_sheet()
{
    delete ui;
}
