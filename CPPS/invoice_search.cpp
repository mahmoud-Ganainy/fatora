#include "invoice_search.h"
#include "ui_invoice_search.h"

invoice_search::invoice_search(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::invoice_search)
{
    ui->setupUi(this);
	db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    db.open();
    }
    model = new QSqlQueryModel(this);
	model1 = new QSqlQueryModel(this);
    clear_all();


}

invoice_search::~invoice_search()
{
    delete ui;
}

void invoice_search::clear_all()
{
    if(search_type == 0){
        model->setQuery("SELECT name,id FROM supplier ",db);
        ui->filter_name->setText("المورد");
        ui->type->setItemText(1,"بحث بالمورد");
    }else if (search_type == 1) {
        model->setQuery("SELECT name,id FROM customers ",db);
        ui->filter_name->setText("العميل");
        ui->type->setItemText(1,"بحث بالعميل");


    }
    ui->supplier->setModel(model);
    ui->supplier->setEnabled(false);
    ui->type->setCurrentIndex(0);
    ui->rec_no->clear();
    id_out.clear();
    type = 0;
}

void invoice_search::on_type_currentIndexChanged(int index)
{
    if(index == 0){
        ui->supplier->setEnabled(false);
        ui->rec_no->setEnabled(true);
    }else if (index == 1) {
        ui->supplier->setEnabled(true);
        ui->supplier->setCurrentIndex(0);
        ui->rec_no->setEnabled(false);
    }
}

void invoice_search::on_pushButton_clicked()
{
    if(ui->type->currentIndex() == 0){
        if(ui->rec_no->text().isEmpty()){
            QMessageBox::warning(this,"please fill the missing field","رجاء ادخل رقم فاتورة");
        }else {
        id_out = ui->rec_no->text();
        type = 0;
        }
    }else if (ui->type->currentIndex() == 1) {
        QModelIndex myindex = model->index(ui->supplier->currentIndex(),1);
        id_out = model->data(myindex).toString();
        type = 1;
    }
    this->accept();
}
