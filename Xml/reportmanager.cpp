#include "reportmanager.h"
#include "ui_reportmanager.h"

reportmanager::reportmanager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reportmanager)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("test");
    db2 = QSqlDatabase::database("setting");
    if(!db.isOpen()){
    if(db.open()){

    }
    else {
    }
    }
    if(!db2.isOpen()){
    if(db2.open()){



    }
    else {
    }
    }
    this->model = new QSqlQueryModel();//
    this->model1 = new QSqlQueryModel();//
    this->model2 = new QSqlQueryModel();//
    this->model3 = new QSqlQueryModel();
    this->model4 = new QSqlQueryModel();
    this->model5 = new QSqlQueryModel();
    this->model6 = new QSqlQueryModel();
    this->model7 = new QSqlQueryModel();
    this->model8 = new QSqlQueryModel();
    this->model10 = new QSqlQueryModel();
    this->model11 = new QSqlQueryModel();
    ui->pur_int->setEnabled(false);
    ui->pur_barcode->setEnabled(false);
    ui->pur_sup_name->setEnabled(false);
    ui->pur_int_check->setEnabled(false);
    ui->pur_item_name->setEnabled(false);
    ui->pur_supp_check->setEnabled(false);
    ui->pur_barcode_check->setEnabled(false);
    ui->pur_item_name_check->setEnabled(false);

    ui->sale_int->setEnabled(false);
    ui->sale_cash_check->setEnabled(false);
    ui->sale_credit_check->setEnabled(false);
    ui->sale_custom_name->setEnabled(false);
    ui->sale_seller_name->setEnabled(false);
    ui->sale_barcode->setEnabled(false);
    ui->sale_cash_depo_name->setEnabled(false);
    ui->sale_item_name->setEnabled(false);
    ui->sale_int_check->setEnabled(false);
    ui->sale_custom_check->setEnabled(false);
    ui->sale_seller_check->setEnabled(false);
    ui->sale_barcode_check->setEnabled(false);
    ui->sale_cash_depo_check->setEnabled(false);
    ui->sale_item_name_check->setEnabled(false);

    ui->start->setDate(QDate::currentDate());
    ui->end->setDate(QDate::currentDate());
    model->setQuery("SELECT name , id FROM storage",db);
    model1->setQuery("SELECT name , id FROM supplier",db);
    model3->setQuery("SELECT name , id FROM customers",db);
    model4->setQuery("SELECT name , id FROM cash_deposit",db);
    model5->setQuery("SELECT name , id FROM employees",db);

    ui->storage->setModel(model);
    ui->pur_sup_name->setModel(model1);
    ui->sale_custom_name->setModel(model3);
    ui->sale_cash_depo_name->setModel(model4);
    ui->sale_seller_name->setModel(model5);
    ui->storage->setCurrentIndex(0);
    ui->pur_sup_name->lineEdit()->clear();
    ui->sale_custom_name->lineEdit()->clear();
    ui->sale_seller_name->lineEdit()->clear();
    ui->sale_cash_depo_name->lineEdit()->clear();

}

reportmanager::~reportmanager()
{
    delete ui;
}

void reportmanager::on_pushButton_clicked()
{

    quan_total = 0;
    buy_total =0;
    sell_total =0;
    QString start = QString::number(ui->start->date().year())+"-"+QString::number(ui->start->date().month())+"-"+QString::number(ui->start->date().day())+" "+"00:00:00";
    QString end = QString::number(ui->end->date().year())+"-"+QString::number(ui->end->date().month())+"-"+QString::number(ui->end->date().day())+" "+"23:59:59";
    QString printer_name;
    QSqlQuery qry2(db2);
    qry2.prepare("SELECT print_name from print_info WHERE print_type = 1");
    if (qry2.exec())
    {while (qry2.next())
        {
           printer_name = qry2.value(0).toString();
        }
    }
    report = new LimeReport::ReportEngine(this);
    LimeReport::ICallbackDatasource *ds =report->dataManager()->createCallbackDatasouce("ds");
    connect(ds,SIGNAL(getCallbackData(const LimeReport::CallbackInfo& , QVariant& )),
            this,SLOT(slotGetCallbackData(const LimeReport::CallbackInfo& , QVariant&)));



    if(ui->pur_radio->isChecked()){
		QString myqry2;
		QString myqry;
		if(ui->pur_purchases_check->isChecked()&&!ui->pur_return_check->isChecked()){
			myqry2 = "SELECT quantity , buy_price , sell_price FROM purchases_items JOIN purchases ON purchases.rec_no = purchases_items.rec_no WHERE purchases.create_date BETWEEN '"+start+"'  AND '"+end+"'";
			myqry = "SELECT * FROM purchases_items JOIN purchases ON purchases.rec_no = purchases_items.rec_no WHERE purchases.create_date BETWEEN '"+start+"'  AND '"+end+"'";
			if(ui->pur_supp_check->isChecked()&&!ui->pur_sup_name->currentText().isEmpty()){
				myqry += "AND purchases.supplier_id = '"+model1->index(ui->pur_sup_name->currentIndex(),1).data().toString()+"' ";
				myqry2 += "AND purchases.supplier_id = '"+model1->index(ui->pur_sup_name->currentIndex(),1).data().toString()+"' ";

			}
			if(ui->pur_barcode_check->isChecked()&&!ui->pur_barcode->text().isEmpty()){
				myqry += "AND purchases_items.exp_barcode = '"+ui->pur_barcode->text()+"' ";
				myqry2 += "AND purchases_items.exp_barcode = '"+ui->pur_barcode->text()+"' ";

			}
			if(ui->pur_int_check->isChecked()&&!ui->pur_int->text().isEmpty()){

				myqry += "AND purchases_items.item_int = '"+ui->pur_int->text()+"' ";
				myqry2 += "AND purchases_items.item_int = '"+ui->pur_int->text()+"' ";

			}
			QSqlQuery qry(db);
			qry.prepare(myqry2);
			if( qry.exec() ){
				while (qry.next())
				{
					quan_total += qry.value(0).toDouble();
					buy_total += qry.value(0).toDouble()*qry.value(1).toDouble();
					sell_total += qry.value(0).toDouble()*qry.value(2).toDouble();

				}
			}
			else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}				
		}
		else if(ui->pur_return_check->isChecked()&&!ui->pur_purchases_check->isChecked()){
			myqry2 = "SELECT quantity , buy_price , sell_price FROM purchases_return_items JOIN purchases_return ON purchases_return.rec_no = purchases_return_items.rec_no WHERE purchases_return.create_date BETWEEN '"+start+"'  AND '"+end+"'";
			myqry = "SELECT * FROM purchases_return_items JOIN purchases ON purchases_return.rec_no = purchases_items.rec_no WHERE purchases_return.create_date BETWEEN '"+start+"'  AND '"+end+"'";
			if(ui->pur_supp_check->isChecked()&&!ui->pur_sup_name->currentText().isEmpty()){
				myqry += "AND purchases_return.supplier_id = '"+model1->index(ui->pur_sup_name->currentIndex(),1).data().toString()+"' ";
				myqry2 += "AND purchases_return.supplier_id = '"+model1->index(ui->pur_sup_name->currentIndex(),1).data().toString()+"' ";

			}
			if(ui->pur_barcode_check->isChecked()&&!ui->pur_barcode->text().isEmpty()){
				myqry += "AND purchases_return_items.exp_barcode = '"+ui->pur_barcode->text()+"' ";
				myqry2 += "AND purchases_return_items.exp_barcode = '"+ui->pur_barcode->text()+"' ";

			}
			if(ui->pur_int_check->isChecked()&&!ui->pur_int->text().isEmpty()){

				myqry += "AND purchases_return_items.item_int = '"+ui->pur_int->text()+"' ";
				myqry2 += "AND purchases_return_items.item_int = '"+ui->pur_int->text()+"' ";

			}
			QSqlQuery qry(db);
			qry.prepare(myqry2);
			if( qry.exec() ){
				while (qry.next())
				{
					quan_total += qry.value(0).toDouble();
					buy_total += qry.value(0).toDouble()*qry.value(1).toDouble();
					sell_total += qry.value(0).toDouble()*qry.value(2).toDouble();

				}
			}
			else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}		
		}else if(ui->pur_return_check->isChecked()&&ui->pur_purchases_check->isChecked()){
			myqry2 = "SELECT quantity , buy_price , sell_price FROM purchases_return_items JOIN purchases_return ON purchases_return.rec_no = purchases_return_items.rec_no WHERE purchases_return.create_date BETWEEN '"+start+"'  AND '"+end+"' ";
			QString un_myqry2 = "SELECT quantity , buy_price , sell_price FROM purchases_items JOIN purchases ON purchases.rec_no = purchases_items.rec_no WHERE purchases.create_date BETWEEN '"+start+"'  AND '"+end+"'";
			myqry = "SELECT * FROM purchases_return_items JOIN purchases ON purchases_return.rec_no = purchases_items.rec_no WHERE purchases_return.create_date BETWEEN '"+start+"'  AND '"+end+"'";
			QString un_myqry ="UNION SELECT * FROM purchases_items JOIN purchases ON purchases.rec_no = purchases_items.rec_no WHERE purchases.create_date BETWEEN '"+start+"'  AND '"+end+"'";			
			if(ui->pur_supp_check->isChecked()&&!ui->pur_sup_name->currentText().isEmpty()){
				myqry += "AND purchases_return.supplier_id = '"+model1->index(ui->pur_sup_name->currentIndex(),1).data().toString()+"' ";
				myqry2 += "AND purchases_return.supplier_id = '"+model1->index(ui->pur_sup_name->currentIndex(),1).data().toString()+"' ";
				un_myqry += "AND purchases.supplier_id = '"+model1->index(ui->pur_sup_name->currentIndex(),1).data().toString()+"' ";
				un_myqry2 += "AND purchases.supplier_id = '"+model1->index(ui->pur_sup_name->currentIndex(),1).data().toString()+"' ";
				
			}
			if(ui->pur_barcode_check->isChecked()&&!ui->pur_barcode->text().isEmpty()){
				myqry += "AND purchases_return_items.exp_barcode = '"+ui->pur_barcode->text()+"' ";
				myqry2 += "AND purchases_return_items.exp_barcode = '"+ui->pur_barcode->text()+"' ";
				un_myqry += "AND purchases_items.exp_barcode = '"+ui->pur_barcode->text()+"' ";
				un_myqry2 += "AND purchases_items.exp_barcode = '"+ui->pur_barcode->text()+"' ";

			}
			if(ui->pur_int_check->isChecked()&&!ui->pur_int->text().isEmpty()){

				myqry += "AND purchases_return_items.item_int = '"+ui->pur_int->text()+"' ";
				myqry2 += "AND purchases_return_items.item_int = '"+ui->pur_int->text()+"' ";
				un_myqry += "AND purchases_items.item_int = '"+ui->pur_int->text()+"' ";
				un_myqry2 += "AND purchases_items.item_int = '"+ui->pur_int->text()+"' ";

			}
			QSqlQuery qry(db);
			qry.prepare(un_myqry2);
			if( qry.exec() ){
				while (qry.next())
				{
					quan_total += qry.value(0).toDouble();
					buy_total += qry.value(0).toDouble()*qry.value(1).toDouble();
					sell_total += qry.value(0).toDouble()*qry.value(2).toDouble();

				}
			}
			else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}
			qry.prepare(myqry2);
			if( qry.exec() ){
				while (qry.next())
				{
					quan_total -= qry.value(0).toDouble();
					buy_total -= qry.value(0).toDouble()*qry.value(1).toDouble();
					sell_total -= qry.value(0).toDouble()*qry.value(2).toDouble();

				}
			}
			else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}			
			myqry = myqry + un_myqry;
		}



        model2->setQuery(myqry,db);
        qDebug()<<myqry;
        report->dataManager()->addModel("bcode",model2,false);
        report->loadFromFile(":/Rc/Xml/test.lrxml");
        report->previewReport();
    }else if (ui->sales_radio->isChecked()) {


        QString myqry = "SELECT * FROM sales_items JOIN sales ON sales.rec_no = sales_items.rec_no WHERE sales.create_date BETWEEN '"+start+"'  AND '"+end+"'";
        QString myqry2 = "SELECT quantity , price  FROM sales_items JOIN sales ON sales.rec_no = sales_items.rec_no WHERE sales.create_date BETWEEN '"+start+"'  AND '"+end+"'";
        if(ui->sale_custom_check->isChecked()&&!ui->sale_custom_name->currentText().isEmpty()){
            myqry += "AND sales.customer_id = '"+model3->index(ui->sale_custom_name->currentIndex(),1).data().toString()+"' ";
            myqry2 += "AND sales.customer_id = '"+model3->index(ui->sale_custom_name->currentIndex(),1).data().toString()+"' ";

        }
        if(ui->sale_barcode_check->isChecked()&&!ui->sale_barcode->text().isEmpty()){
            myqry += "AND sales_items.exp_barcode = '"+ui->sale_barcode->text()+"' ";
            myqry2 += "AND sales_items.exp_barcode = '"+ui->sale_barcode->text()+"' ";

        }
        if(ui->sale_int_check->isChecked()&&!ui->sale_int->text().isEmpty()){

            myqry += "AND sales_items.item_int = '"+ui->sale_int->text()+"' ";
            myqry2 += "AND sales_items.item_int = '"+ui->sale_int->text()+"' ";

        }
        if(ui->sale_cash_check->isChecked()){
            if(ui->sale_credit_check->isChecked()){
                myqry += "AND (sales.payment_method_id = 0";
                myqry2 += "AND (sales.payment_method_id = 0";
            }else {
                myqry += "AND sales.payment_method_id = 0";
                myqry2 += "AND sales.payment_method_id = 0";
            }
        }
        if(ui->sale_credit_check->isChecked()){
            if(ui->sale_cash_check->isChecked()){
                myqry += " OR sales.payment_method_id = 1)";
                myqry2 += " OR sales.payment_method_id = 1)";
            }else {
                myqry += " AND sales.payment_method_id = 1";
                myqry2 += " AND sales.payment_method_id = 1";
            }
        }
        if(ui->sale_cash_depo_check->isChecked()&&!ui->sale_cash_depo_name->currentText().isEmpty()){
            myqry += "AND sales.cash_deposit_id = '"+model4->index(ui->sale_cash_depo_name->currentIndex(),1).data().toString()+"' ";
            myqry2 += "AND sales.cash_deposit_id = '"+model4->index(ui->sale_cash_depo_name->currentIndex(),1).data().toString()+"' ";
        }
        if(ui->sale_seller_check->isChecked()&&!ui->sale_seller_name->currentText().isEmpty()){
            myqry += "AND sales.seller_id = '"+model5->index(ui->sale_seller_name->currentIndex(),1).data().toString()+"' ";
            myqry2 += "AND sales.seller_id = '"+model5->index(ui->sale_seller_name->currentIndex(),1).data().toString()+"' ";
        }

        QSqlQuery qry(db);
        qry.prepare(myqry2);
        if( qry.exec() ){
            while (qry.next())
            {
                quan_total += qry.value(0).toDouble();
                sell_total += qry.value(0).toDouble()*qry.value(1).toDouble();
            }
        }
        else{qDebug() << "Error inserting into the table:\n" << qry.lastQuery();}

        model2->setQuery(myqry,db);
        report->dataManager()->addModel("bcode",model2,false);
        report->loadFromFile(":/Rc/Xml/sales.lrxml");
        report->previewReport();
        //report->designReport();
    }





}
void reportmanager::slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    QString start = QString::number(ui->start->date().year())+"-"+QString::number(ui->start->date().month())+"-"+QString::number(ui->start->date().day())+" "+"00:00:00";
    QString end = QString::number(ui->end->date().year())+"-"+QString::number(ui->end->date().month())+"-"+QString::number(ui->end->date().day())+" "+"23:59:59";
    QStringList columns;
    columns << "start" << "end" << "supplier" << "storage"<<"buy_total"<<"sell_total"<<"quan_total"<<"customer";
    switch (info.dataType) {
    case LimeReport::CallbackInfo::RowCount:
        data = 1;
        break;
    case LimeReport::CallbackInfo::ColumnCount:
        data = columns.size();
        break;
    case LimeReport::CallbackInfo::ColumnHeaderData:
        data = columns.at(info.index);
        break;
    case LimeReport::CallbackInfo::ColumnData:
        if(info.columnName == "start"){
            data = start;
        }else if (info.columnName == "end") {
            data = end;
        }else if (info.columnName == "supplier") {
            if(ui->pur_supp_check->isChecked()&&!ui->pur_sup_name->currentText().isEmpty()){
            data = ui->pur_sup_name->currentText();}else {
            data = "جميع الموردين";
            }
        }else if (info.columnName == "storage") {
            data = ui->storage->currentText();
        }else if (info.columnName == "buy_total") {
            data = buy_total;
        }else if (info.columnName == "sell_total") {
            data = sell_total;
        }else if (info.columnName == "quan_total") {
            data = quan_total;
        }else if (info.columnName == "customer") {
            if(ui->sale_custom_check->isChecked()&&!ui->sale_custom_name->currentText().isEmpty()){
            data = ui->sale_custom_name->currentText();}else {
            data = "جميع العملاء";
            }
        }

        break;
    default: break;
    }
}

void reportmanager::on_pur_supp_check_toggled(bool checked)
{
    if(checked == true){
        ui->pur_sup_name->setEnabled(true);
    }else {
        ui->pur_sup_name->setEnabled(false);
        ui->pur_sup_name->lineEdit()->clear();

    }
}


void reportmanager::on_pur_barcode_check_toggled(bool checked)
{
    if(checked == true){
        ui->pur_barcode->setEnabled(true);

    }else {
        ui->pur_barcode->setEnabled(false);
        ui->pur_barcode->clear();

    }
}

void reportmanager::on_pur_int_check_toggled(bool checked)
{
    if(checked == true){
        ui->pur_int->setEnabled(true);

    }else {
        ui->pur_int->setEnabled(false);
        ui->pur_int->clear();

    }
}

void reportmanager::on_pur_radio_toggled(bool checked)
{
    if(checked == true){
        ui->pur_int_check->setEnabled(true);
        ui->pur_supp_check->setEnabled(true);
        ui->pur_barcode_check->setEnabled(true);
        ui->pur_item_name_check->setEnabled(true);
    }
    else if (checked == false) {
        ui->pur_int_check->setEnabled(false);
        ui->pur_supp_check->setEnabled(false);
        ui->pur_barcode_check->setEnabled(false);
        ui->pur_item_name_check->setEnabled(false);
        ui->pur_int_check->setChecked(false);
        ui->pur_supp_check->setChecked(false);
        ui->pur_barcode_check->setChecked(false);
        ui->pur_item_name_check->setChecked(false);
        ui->pur_int->setEnabled(false);
        ui->pur_barcode->setEnabled(false);
        ui->pur_item_name->setEnabled(false);
    }
}

void reportmanager::on_pur_item_name_check_toggled(bool checked)
{
    if(checked == true){
        ui->pur_item_name->setEnabled(true);

    }else {
        ui->pur_item_name->setEnabled(false);
        ui->pur_item_name->lineEdit()->clear();

    }
}



void reportmanager::on_sales_radio_toggled(bool checked)
{
    if(checked == true){
    ui->sale_int_check->setEnabled(true);
    ui->sale_cash_check->setEnabled(true);
    ui->sale_credit_check->setEnabled(true);
    ui->sale_custom_check->setEnabled(true);
    ui->sale_seller_check->setEnabled(true);
    ui->sale_barcode_check->setEnabled(true);
    ui->sale_cash_depo_check->setEnabled(true);
    ui->sale_item_name_check->setEnabled(true);

    }
    else if (checked == false) {
        ui->sale_int_check->setEnabled(false);
        ui->sale_cash_check->setEnabled(false);
        ui->sale_credit_check->setEnabled(false);
        ui->sale_custom_check->setEnabled(false);
        ui->sale_seller_check->setEnabled(false);
        ui->sale_barcode_check->setEnabled(false);
        ui->sale_cash_depo_check->setEnabled(false);
        ui->sale_item_name_check->setEnabled(false);
        ui->sale_int_check->setChecked(false);
        ui->sale_cash_check->setChecked(false);
        ui->sale_credit_check->setChecked(false);
        ui->sale_custom_check->setChecked(false);
        ui->sale_seller_check->setChecked(false);
        ui->sale_barcode_check->setChecked(false);
        ui->sale_cash_depo_check->setChecked(false);
        ui->sale_item_name_check->setChecked(false);
        ui->sale_int->setEnabled(false);
        ui->sale_barcode->setEnabled(false);
        ui->sale_item_name->setEnabled(false);
    }
}

void reportmanager::on_sale_custom_check_toggled(bool checked)
{
    if(checked == true && ui->sales_radio->isChecked()){
        ui->sale_custom_name->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_custom_name->lineEdit()->clear();
        ui->sale_custom_name->setEnabled(false);
    }
}

void reportmanager::on_sale_cash_depo_check_toggled(bool checked)
{
    if(checked == true && ui->sales_radio->isChecked()){
        ui->sale_cash_depo_name->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_cash_depo_name->setEnabled(false);
        ui->sale_cash_depo_name->lineEdit()->clear();

    }
}

void reportmanager::on_sale_seller_check_toggled(bool checked)
{
    if(checked == true && ui->sales_radio->isChecked()){
        ui->sale_seller_name->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_seller_name->setEnabled(false);
        ui->sale_seller_name->lineEdit()->clear();
    }
}



void reportmanager::on_sale_barcode_check_toggled(bool checked)
{
    if(checked == true && ui->sales_radio->isChecked()){
        ui->sale_barcode->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_barcode->setEnabled(false);
        ui->sale_barcode->clear();
    }
}

void reportmanager::on_sale_int_check_toggled(bool checked)
{
    if(checked == true && ui->sales_radio->isChecked()){
        ui->sale_int->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_int->setEnabled(false);
        ui->sale_int->clear();

    }
}

void reportmanager::on_sale_item_name_check_toggled(bool checked)
{
    if(checked == true && ui->sales_radio->isChecked()){
        ui->sale_item_name->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_item_name->setEnabled(false);
        ui->sale_item_name->lineEdit()->clear();
    }
}
