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
    this->model9 = new QSqlQueryModel();
    this->model10 = new QSqlQueryModel();
    this->model11 = new QSqlQueryModel();
    ui->stackedWidget->setCurrentIndex(0);
    enable_buy();
    disable_sell();

    ui->start->setDate(QDate::currentDate());
    ui->end->setDate(QDate::currentDate());
    model->setQuery("SELECT name , id FROM storage",db);
    model1->setQuery("SELECT name , id FROM supplier",db);
    model3->setQuery("SELECT name , id FROM customers",db);
    model4->setQuery("SELECT name , id FROM cash_deposit",db);
    model5->setQuery("SELECT name , id FROM employees",db);
    model6->setQuery("SELECT name , id FROM category",db);
    model7->setQuery("SELECT name , id FROM class",db);
    model8->setQuery("SELECT name , id FROM season",db);
	model9->setQuery("SELECT name , id FROM items",db);
	model10->setQuery("SELECT id FROM sub_items order by id ASC",db);
    model11->setQuery("SELECT int_barcode FROM sub_items order by id ASC",db);
	
    ui->storage->setModel(model);
    ui->pur_sup_name->setModel(model1);
    ui->sale_custom_name->setModel(model3);
    ui->sale_cash_depo_name->setModel(model4);
    ui->sale_seller_name->setModel(model5);
	ui->category_name->setModel(model6);
	ui->class_name->setModel(model7);
	ui->season_name->setModel(model8);
	ui->inv_item_name->setModel(model9);
	ui->inv_item_bcode->setModel(model10);
	ui->inv_item_int->setModel(model11);
    ui->storage->setCurrentIndex(0);
    ui->inv_supplier_name->setModel(model1);
    ui->pur_sup_name->lineEdit()->clear();
    ui->sale_custom_name->lineEdit()->clear();
    ui->sale_seller_name->lineEdit()->clear();
    ui->sale_cash_depo_name->lineEdit()->clear();

}

reportmanager::~reportmanager()
{
    delete ui;
}
void reportmanager::on_report_type_currentIndexChanged(int index)
{
    if(index == 0){
        ui->stackedWidget->setCurrentIndex(0);
        enable_buy();
        disable_inv();
        disable_sell();

    }else if (index == 1) {
        ui->stackedWidget->setCurrentIndex(1);
        enable_sell();
        disable_buy();
        disable_inv();


    }else if (index == 2) {
        ui->stackedWidget->setCurrentIndex(2);
        enable_inv();
        disable_buy();
        disable_sell();
    }
    else if (index == 3) {
        ui->stackedWidget->setCurrentIndex(3);
    }
}
void reportmanager::enable_buy()
{
    ui->pur_int_check->setEnabled(true);
    ui->pur_supp_check->setEnabled(true);
    ui->pur_barcode_check->setEnabled(true);
    ui->pur_item_name_check->setEnabled(true);
}

void reportmanager::disable_buy()
{
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

void reportmanager::enable_sell()
{
    ui->sale_int_check->setEnabled(true);
    ui->sale_cash_check->setEnabled(true);
    ui->sale_credit_check->setEnabled(true);
    ui->sale_custom_check->setEnabled(true);
    ui->sale_seller_check->setEnabled(true);
    ui->sale_barcode_check->setEnabled(true);
    ui->sale_cash_depo_check->setEnabled(true);
    ui->sale_item_name_check->setEnabled(true);
}

void reportmanager::disable_sell()
{
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

void reportmanager::enable_inv()
{

}

void reportmanager::disable_inv()
{

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



    if(ui->report_type->currentIndex() == 0){
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
            myqry2 = "SELECT quantity ,price AS 'buy_price'  FROM purchases_return_items JOIN purchases_return ON purchases_return.rec_no = purchases_return_items.rec_no WHERE purchases_return.create_date BETWEEN '"+start+"'  AND '"+end+"'";
            myqry = "SELECT *,price AS 'buy_price' FROM purchases_return_items JOIN purchases_return ON purchases_return.rec_no = purchases_return_items.rec_no WHERE purchases_return.create_date BETWEEN '"+start+"'  AND '"+end+"'";
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
				}
			}
			else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}		
		}
		else if(ui->pur_return_check->isChecked()&&ui->pur_purchases_check->isChecked()){
            myqry2 = "SELECT quantity ,price AS 'buy_price' FROM purchases_return_items JOIN purchases_return ON purchases_return.rec_no = purchases_return_items.rec_no WHERE purchases_return.create_date BETWEEN '"+start+"'  AND '"+end+"' ";
			QString un_myqry2 = "SELECT quantity , buy_price , sell_price FROM purchases_items JOIN purchases ON purchases.rec_no = purchases_items.rec_no WHERE purchases.create_date BETWEEN '"+start+"'  AND '"+end+"'";

            myqry = "SELECT purchases_return_items.id,purchases_return_items.rec_no,purchases_return_items.item_name,purchases_return_items.item_int,purchases_return_items.exp_barcode,purchases_return_items.price as 'buy_price',purchases_return_items.quantity,null AS 'sell_price',purchases_return_items.total,purchases_return_items.create FROM purchases_return_items JOIN purchases_return ON purchases_return.rec_no = purchases_return_items.rec_no WHERE purchases_return.create_date BETWEEN '"+start+"'  AND '"+end+"'";
            QString un_myqry ="UNION SELECT purchases_items.id,purchases_items.rec_no,purchases_items.item_name,purchases_items.item_int,purchases_items.exp_barcode,purchases_items.buy_price,purchases_items.quantity,purchases_items.sell_price,purchases_items.total,purchases_items.create FROM purchases_items JOIN purchases ON purchases.rec_no = purchases_items.rec_no WHERE purchases.create_date BETWEEN '"+start+"'  AND '"+end+"'";




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

				}
			}
			else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}
			qry.prepare(myqry2);
			if( qry.exec() ){
				while (qry.next())
				{
					quan_total -= qry.value(0).toDouble();
					buy_total -= qry.value(0).toDouble()*qry.value(1).toDouble();

				}
			}
			else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}			
			myqry = myqry + un_myqry;
		}



        model2->setQuery(myqry,db);
        qDebug()<<myqry;
        report->dataManager()->addModel("bcode",model2,false);
        report->loadFromFile(":/Rc/Xml/test.lrxml");
        //report->designReport();
        report->designReport();
    }
	else if (ui->report_type->currentIndex() == 1) {
        QString myqry;
        QString myqry2;
        if(ui->sale_sales_check->isChecked()&&!ui->sale_return_check->isChecked()){
             myqry = "SELECT * FROM sales_items JOIN sales ON sales.rec_no = sales_items.rec_no WHERE sales.create_date BETWEEN '"+start+"'  AND '"+end+"'";
             myqry2 = "SELECT quantity , price  FROM sales_items JOIN sales ON sales.rec_no = sales_items.rec_no WHERE sales.create_date BETWEEN '"+start+"'  AND '"+end+"'";


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
        }else if (!ui->sale_sales_check->isChecked()&&ui->sale_return_check->isChecked()) {
            myqry = "SELECT * FROM sales_return_items JOIN sales_return ON sales_return.rec_no = sales_return_items.rec_no WHERE sales_return.create_date BETWEEN '"+start+"'  AND '"+end+"'";
            myqry2 = "SELECT quantity , price  FROM sales_return_items JOIN sales_return ON sales_return.rec_no = sales_return_items.rec_no WHERE sales_return.create_date BETWEEN '"+start+"'  AND '"+end+"'";


           if(ui->sale_custom_check->isChecked()&&!ui->sale_custom_name->currentText().isEmpty()){
               myqry += "AND sales_return.customer_id = '"+model3->index(ui->sale_custom_name->currentIndex(),1).data().toString()+"' ";
               myqry2 += "AND sales_return.customer_id = '"+model3->index(ui->sale_custom_name->currentIndex(),1).data().toString()+"' ";

           }
           if(ui->sale_barcode_check->isChecked()&&!ui->sale_barcode->text().isEmpty()){
               myqry += "AND sales_return_items.exp_barcode = '"+ui->sale_barcode->text()+"' ";
               myqry2 += "AND sales_return_items.exp_barcode = '"+ui->sale_barcode->text()+"' ";

           }
           if(ui->sale_int_check->isChecked()&&!ui->sale_int->text().isEmpty()){

               myqry += "AND sales_return_items.item_int = '"+ui->sale_int->text()+"' ";
               myqry2 += "AND sales_return_items.item_int = '"+ui->sale_int->text()+"' ";

           }
           if(ui->sale_cash_check->isChecked()){
               if(ui->sale_credit_check->isChecked()){
                   myqry += "AND (sales_return.payment_method_id = 0";
                   myqry2 += "AND (sales_return.payment_method_id = 0";
               }else {
                   myqry += "AND sales_return.payment_method_id = 0";
                   myqry2 += "AND sales_return.payment_method_id = 0";
               }
           }
           if(ui->sale_credit_check->isChecked()){
               if(ui->sale_cash_check->isChecked()){
                   myqry += " OR sales_return.payment_method_id = 1)";
                   myqry2 += " OR sales_return.payment_method_id = 1)";
               }else {
                   myqry += " AND sales_return.payment_method_id = 1";
                   myqry2 += " AND sales_return.payment_method_id = 1";
               }
           }
           if(ui->sale_cash_depo_check->isChecked()&&!ui->sale_cash_depo_name->currentText().isEmpty()){
               myqry += "AND sales_return.cash_deposit_id = '"+model4->index(ui->sale_cash_depo_name->currentIndex(),1).data().toString()+"' ";
               myqry2 += "AND sales_return.cash_deposit_id = '"+model4->index(ui->sale_cash_depo_name->currentIndex(),1).data().toString()+"' ";
           }
           if(ui->sale_seller_check->isChecked()&&!ui->sale_seller_name->currentText().isEmpty()){
               myqry += "AND sales_return.seller_id = '"+model5->index(ui->sale_seller_name->currentIndex(),1).data().toString()+"' ";
               myqry2 += "AND sales_return.seller_id = '"+model5->index(ui->sale_seller_name->currentIndex(),1).data().toString()+"' ";
           }


        }else if (ui->sale_sales_check->isChecked()&&ui->sale_return_check->isChecked()) {

        }


        qDebug()<<myqry;
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
	else if (ui->report_type->currentIndex() == 2){
		QString myqry;
		QString item_id;
		if(ui->inv_type->currentIndex() == 0){
            QString storage_id = model->index(ui->storage->currentIndex(),1).data().toString();
			myqry = "SELECT items.name as ' item_name' ,purchases_items.exp_barcode as 'barcode' ,purchases_items.buy_price ,purchases_items.sell_price,(SUM(purchases_items.quantity) - IFNULL(SUM(purchases_return_items.quantity),0)- IFNULL(SUM(sales_items.quantity),0)+ IFNULL(SUM(sales_return_items.quantity),0))as 'item_balance',(purchases_items.buy_price*(purchases_items.quantity - IFNULL(SUM(purchases_return_items.quantity),0)- IFNULL(SUM(sales_items.quantity),0)+ IFNULL(SUM(sales_return_items.quantity),0)))as 'buy_total_per' ,(purchases_items.sell_price*(purchases_items.quantity - IFNULL(SUM(purchases_return_items.quantity),0)- IFNULL(SUM(sales_items.quantity),0)+ IFNULL(SUM(sales_return_items.quantity),0)))as 'sell_total_per' ,item_balance.expire , SUM(purchases_items.quantity) as 'buy_quan' , SUM(purchases_return_items.quantity) as 'buy_return_quan', SUM(sales_items.quantity) as 'sale_quan', SUM(sales_return_items.quantity) as 'sale_return_quan' FROM purchases_items"			
			" JOIN item_balance on purchases_items.exp_barcode = item_balance.exp_barcode AND purchases_items.create < '"+end+"' "
			" JOIN sub_items on sub_items.id = item_balance.sub_item_id "
			" LEFT JOIN purchases_return_items on purchases_return_items.exp_barcode = item_balance.exp_barcode AND purchases_return_items.create < '"+end+"' "
            " LEFT JOIN sales_items on sales_items.exp_barcode = item_balance.exp_barcode AND sales_items.create < '"+end+"' "
            " LEFT JOIN sales_return_items on sales_return_items.exp_barcode = item_balance.exp_barcode AND sales_return_items.create < '"+end+"' "
            " LEFT JOIN sales on sales_items.rec_no = sales.rec_no  AND sales.storage_id = '"+storage_id+"' "
            " LEFT JOIN sales_return on sales_return.rec_no = sales_return_items.rec_no  AND sales_return.storage_id = '"+storage_id+"' ";


			
            if(ui->inv_supplier_check->isChecked()){
				
				QString supp_id = model1->index(ui->inv_supplier_name->currentIndex(),1).data().toString();	
				QString storage_id = model->index(ui->storage->currentIndex(),1).data().toString();
                myqry += " JOIN purchases on purchases_items.rec_no = purchases.rec_no AND purchases.supplier_id = '"+supp_id+"' AND purchases.storage_id = '"+storage_id+"' ";

                myqry +=	" LEFT JOIN purchases_return on purchases_return.rec_no = purchases_return_items.rec_no AND purchases_return.supplier_id = '"+supp_id+"' AND purchases_return.storage_id = '"+storage_id+"' ";


			}
            else{
                QString storage_id = model->index(ui->storage->currentIndex(),1).data().toString();

                myqry +=	"  JOIN purchases on purchases_items.rec_no = purchases.rec_no AND purchases.storage_id = '"+storage_id+"' ";

                myqry +=	" LEFT JOIN purchases_return on purchases_return.rec_no = purchases_return_items.rec_no AND purchases_return.storage_id = '"+storage_id+"' ";

            }


            /*else if(inv_supplier_check->isChecked()&& !inv_storage_check->isChecked()){
				
				QString supp_id = model1->index(ui->inv_supplier_name->currentIndex(),1).data().toString();	
				
				myqry +=	" JOIN purchases on purchases_items.rec_no = purchases.rec_no AND purchases.supplier_id = '"+supp_id+"' "
			
				myqry +=	" JOIN purchases_return on purchases_return.rec_no = purchases_return_items.rec_no AND purchases_return.supplier_id = '"+supp_id+"' "

            }*/
            /*else if(!inv_supplier_check->isChecked()&& inv_storage_check->isChecked()){

            }*/
			if(ui->inv_item_name_check->isChecked()){
				
                item_id = model9->index(ui->inv_item_name->currentIndex(),1).data().toString();
							
			}
			if(ui->inv_item_bcode_check->isChecked()){
                QString bcode = model10->index(ui->inv_item_bcode->currentIndex(),0).data().toString();
				QSqlQuery qry2(db);
				qry2.prepare("SELECT item_id FROM sub_items WHERE id = :id");
				qry2.bindValue(":id",bcode);
				if( qry2.exec() ){
				 while (qry2.next())
				    {
                        item_id = qry2.value(0).toString();
					}
				}
				else{qDebug() << "Error inserting into the table:\n" << qry2.lastError();}	
				
				
			}
			if(ui->inv_item_int_check->isChecked()){
                QString int_barcode = model11->index(ui->inv_item_int->currentIndex(),0).data().toString();;
				QSqlQuery qry2(db);
                qry2.prepare("SELECT item_id FROM sub_items WHERE int_barcode = :id");
				qry2.bindValue(":id",int_barcode);
				if( qry2.exec() ){
				 while (qry2.next())
				    {
                        item_id = qry2.value(0).toString();
					}
				}
				else{qDebug() << "Error inserting into the table:\n" << qry2.lastError();}
						
			}
		
            if(ui->inv_categ_check->isChecked()&&ui->inv_season_check->isChecked()&&ui->inv_class_check->isChecked()){
				QString category_id = model6->index(ui->category_name->currentIndex(),1).data().toString();
				QString class_id = model7->index(ui->class_name->currentIndex(),1).data().toString();
				QString season_id = model8->index(ui->season_name->currentIndex(),1).data().toString();
				if(!item_id.isEmpty()){
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' AND items.season_id = '"+season_id+"' AND items.class_id = '"+class_id+"' AND items.id = '"+item_id+"' group by purchases_items.exp_barcode ";
				}else{
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' AND items.season_id = '"+season_id+"' AND items.class_id = '"+class_id+"' group by purchases_items.exp_barcode ";
					
				}
				
			}
            else if(!ui->inv_categ_check->isChecked()&&ui->inv_season_check->isChecked()&&ui->inv_class_check->isChecked()){
				QString class_id = model7->index(ui->class_name->currentIndex(),1).data().toString();
				QString season_id = model8->index(ui->season_name->currentIndex(),1).data().toString();
				if(!item_id.isEmpty()){
					myqry +=	" JOIN items on sub_items.item_id = items.id  AND items.season_id = '"+season_id+"' AND items.class_id = '"+class_id+"' AND items.id = '"+item_id+"' group by purchases_items.exp_barcode ";	
				}else{
					myqry +=	" JOIN items on sub_items.item_id = items.id  AND items.season_id = '"+season_id+"' AND items.class_id = '"+class_id+"' group by purchases_items.exp_barcode ";
					
				}
				
			}
            else if(ui->inv_categ_check->isChecked()&&!ui->inv_season_check->isChecked()&&ui->inv_class_check->isChecked()){
				QString category_id = model6->index(ui->category_name->currentIndex(),1).data().toString();
				QString class_id = model7->index(ui->class_name->currentIndex(),1).data().toString();
				if(!item_id.isEmpty()){
	                myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' AND items.class_id = '"+class_id+"' AND items.id = '"+item_id+"' group by purchases_items.exp_barcode ";
				
				}else{
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' AND items.class_id = '"+class_id+"' group by purchases_items.exp_barcode ";
					
				}
				
			}
            else if(ui->inv_categ_check->isChecked()&&ui->inv_season_check->isChecked()&&!ui->inv_class_check->isChecked()){
				QString category_id = model6->index(ui->category_name->currentIndex(),1).data().toString();
				QString season_id = model8->index(ui->season_name->currentIndex(),1).data().toString();
				if(!item_id.isEmpty()){
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' AND items.season_id = '"+season_id+"' AND items.id = '"+item_id+"' group by purchases_items.exp_barcode " ;
				}else{
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' AND items.season_id = '"+season_id+"' group by purchases_items.exp_barcode " ;
					
				}
			}
            else if(ui->inv_categ_check->isChecked()&&!ui->inv_season_check->isChecked()&&!ui->inv_class_check->isChecked()){
				QString category_id = model6->index(ui->category_name->currentIndex(),1).data().toString();
				if(!item_id.isEmpty()){
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' AND items.id = '"+item_id+"' group by purchases_items.exp_barcode ";					
				}else{
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' group by purchases_items.exp_barcode ";
					
				}
				
			}
            else if(!ui->inv_categ_check->isChecked()&&!ui->inv_season_check->isChecked()&&ui->inv_class_check->isChecked()){
				QString class_id = model7->index(ui->class_name->currentIndex(),1).data().toString();
				if(!item_id.isEmpty()){
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.class_id = '"+class_id+"' AND items.id = '"+item_id+"' group by purchases_items.exp_barcode ";
				}else{
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.class_id = '"+class_id+"' group by purchases_items.exp_barcode ";
					
				}
				
			}
            else if(!ui->inv_categ_check->isChecked()&&ui->inv_season_check->isChecked()&&!ui->inv_class_check->isChecked()){
				QString season_id = model8->index(ui->season_name->currentIndex(),1).data().toString();
				if(!item_id.isEmpty()){
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.season_id = '"+season_id+"' AND items.id = '"+item_id+"' group by purchases_items.exp_barcode ";
				}else{
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.season_id = '"+season_id+"' group by purchases_items.exp_barcode ";
					
				}
			}
			else{
				if(!item_id.isEmpty()){
					myqry +=	" JOIN items on sub_items.item_id = items.id AND items.id = '"+item_id+"' group by purchases_items.exp_barcode ";
				}else{
					myqry +=	" JOIN items on sub_items.item_id = items.id group by purchases_items.exp_barcode ";
					
				}
			}			
			
		}else if(ui->inv_type->currentIndex() == 1){

                    QString storage_id = model->index(ui->storage->currentIndex(),1).data().toString();
                    myqry = "SELECT items.name as ' item_name' ,purchases_items.exp_barcode as 'barcode' ,purchases_items.buy_price ,purchases_items.sell_price,(SUM(purchases_items.quantity) - IFNULL(SUM(purchases_return_items.quantity),0)- IFNULL(SUM(sales_items.quantity),0)+ IFNULL(SUM(sales_return_items.quantity),0))as 'item_balance',(purchases_items.buy_price*(purchases_items.quantity - IFNULL(SUM(purchases_return_items.quantity),0)- IFNULL(SUM(sales_items.quantity),0)+ IFNULL(SUM(sales_return_items.quantity),0)))as 'buy_total_per' ,(purchases_items.sell_price*(purchases_items.quantity - IFNULL(SUM(purchases_return_items.quantity),0)- IFNULL(SUM(sales_items.quantity),0)+ IFNULL(SUM(sales_return_items.quantity),0)))as 'sell_total_per' ,item_balance.expire , SUM(purchases_items.quantity) as 'buy_quan' , SUM(purchases_return_items.quantity) as 'buy_return_quan', SUM(sales_items.quantity) as 'sale_quan', SUM(sales_return_items.quantity) as 'sale_return_quan' FROM purchases_items"
                    " JOIN item_balance on purchases_items.exp_barcode = item_balance.exp_barcode AND purchases_items.create BETWEEN '"+start+"'  AND '"+end+"' "
                    " JOIN sub_items on sub_items.id = item_balance.sub_item_id "
                    " LEFT JOIN purchases_return_items on purchases_return_items.exp_barcode = item_balance.exp_barcode AND purchases_return_items.create BETWEEN '"+start+"'  AND '"+end+"' "
                    " LEFT JOIN sales_items on sales_items.exp_barcode = item_balance.exp_barcode AND sales_items.create BETWEEN '"+start+"'  AND '"+end+"' "
                    " LEFT JOIN sales_return_items on sales_return_items.exp_barcode = item_balance.exp_barcode AND sales_return_items.create BETWEEN '"+start+"'  AND '"+end+"' "
                    " LEFT JOIN sales on sales_items.rec_no = sales.rec_no  AND sales.storage_id = '"+storage_id+"' "
                    " LEFT JOIN sales_return on sales_return.rec_no = sales_return_items.rec_no  AND sales_return.storage_id = '"+storage_id+"' ";



                    if(ui->inv_supplier_check->isChecked()){

                        QString supp_id = model1->index(ui->inv_supplier_name->currentIndex(),1).data().toString();
                        QString storage_id = model->index(ui->storage->currentIndex(),1).data().toString();
                        myqry += " JOIN purchases on purchases_items.rec_no = purchases.rec_no AND purchases.supplier_id = '"+supp_id+"' AND purchases.storage_id = '"+storage_id+"' ";

                        myqry +=	" LEFT JOIN purchases_return on purchases_return.rec_no = purchases_return_items.rec_no AND purchases_return.supplier_id = '"+supp_id+"' AND purchases_return.storage_id = '"+storage_id+"' ";


                    }
                    else{
                        QString storage_id = model->index(ui->storage->currentIndex(),1).data().toString();

                        myqry +=	"  JOIN purchases on purchases_items.rec_no = purchases.rec_no AND purchases.storage_id = '"+storage_id+"' ";

                        myqry +=	" LEFT JOIN purchases_return on purchases_return.rec_no = purchases_return_items.rec_no AND purchases_return.storage_id = '"+storage_id+"' ";

                    }


                    /*else if(inv_supplier_check->isChecked()&& !inv_storage_check->isChecked()){

                        QString supp_id = model1->index(ui->inv_supplier_name->currentIndex(),1).data().toString();

                        myqry +=	" JOIN purchases on purchases_items.rec_no = purchases.rec_no AND purchases.supplier_id = '"+supp_id+"' "

                        myqry +=	" JOIN purchases_return on purchases_return.rec_no = purchases_return_items.rec_no AND purchases_return.supplier_id = '"+supp_id+"' "

                    }*/
                    /*else if(!inv_supplier_check->isChecked()&& inv_storage_check->isChecked()){

                    }*/


                    if(ui->inv_categ_check->isChecked()&&ui->inv_season_check->isChecked()&&ui->inv_class_check->isChecked()){
                        QString category_id = model6->index(ui->category_name->currentIndex(),1).data().toString();
                        QString class_id = model7->index(ui->class_name->currentIndex(),1).data().toString();
                        QString season_id = model8->index(ui->season_name->currentIndex(),1).data().toString();

                        myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' AND items.season_id = '"+season_id+"' AND items.class_id = '"+class_id+"' group by purchases_items.exp_barcode ";

                    }
                    else if(!ui->inv_categ_check->isChecked()&&ui->inv_season_check->isChecked()&&ui->inv_class_check->isChecked()){
                        QString class_id = model7->index(ui->class_name->currentIndex(),1).data().toString();
                        QString season_id = model8->index(ui->season_name->currentIndex(),1).data().toString();

                        myqry +=	" JOIN items on sub_items.item_id = items.id  AND items.season_id = '"+season_id+"' AND items.class_id = '"+class_id+"' group by purchases_items.exp_barcode ";

                    }
                    else if(ui->inv_categ_check->isChecked()&&!ui->inv_season_check->isChecked()&&ui->inv_class_check->isChecked()){
                        QString category_id = model6->index(ui->category_name->currentIndex(),1).data().toString();
                        QString class_id = model7->index(ui->class_name->currentIndex(),1).data().toString();

                        myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' AND items.class_id = '"+class_id+"' group by purchases_items.exp_barcode ";

                    }
                    else if(ui->inv_categ_check->isChecked()&&ui->inv_season_check->isChecked()&&!ui->inv_class_check->isChecked()){
                        QString category_id = model6->index(ui->category_name->currentIndex(),1).data().toString();
                        QString season_id = model8->index(ui->season_name->currentIndex(),1).data().toString();

                        myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' AND items.season_id = '"+season_id+"' group by purchases_items.exp_barcode " ;
                    }
                    else if(ui->inv_categ_check->isChecked()&&!ui->inv_season_check->isChecked()&&!ui->inv_class_check->isChecked()){
                        QString category_id = model6->index(ui->category_name->currentIndex(),1).data().toString();

                        myqry +=	" JOIN items on sub_items.item_id = items.id AND items.category_id = '"+category_id+"' group by purchases_items.exp_barcode ";

                    }
                    else if(!ui->inv_categ_check->isChecked()&&!ui->inv_season_check->isChecked()&&ui->inv_class_check->isChecked()){
                        QString class_id = model7->index(ui->class_name->currentIndex(),1).data().toString();

                        myqry +=	" JOIN items on sub_items.item_id = items.id AND items.class_id = '"+class_id+"' group by purchases_items.exp_barcode ";

                    }
                    else if(!ui->inv_categ_check->isChecked()&&ui->inv_season_check->isChecked()&&!ui->inv_class_check->isChecked()){
                        QString season_id = model8->index(ui->season_name->currentIndex(),1).data().toString();

                        myqry +=	" JOIN items on sub_items.item_id = items.id AND items.season_id = '"+season_id+"' group by purchases_items.exp_barcode ";
                    }
                    else{

                        myqry +=	" JOIN items on sub_items.item_id = items.id group by purchases_items.exp_barcode ";
                    }


		}
		
        buy_total = 0;
        sell_total = 0;
        QSqlQuery qry(db);
        qry.prepare(myqry);
        if( qry.exec() ){
            while (qry.next())
            {
                sell_total += qry.value(3).toDouble()*qry.value(4).toDouble();
                buy_total += qry.value(2).toDouble()*qry.value(4).toDouble();

            }
        }
        else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}
        qDebug()<<myqry;
		model2->setQuery(myqry,db);
        report->dataManager()->addModel("buy",model2,false);
        report->loadFromFile(":/Rc/Xml/inv.lrxml");
        report->previewReport();
	}
    else if (ui->report_type->currentIndex() == 3) {

      report->loadFromFile(":/Rc/Xml/profit.lrxml");
      report->previewReport();

    }





}
void reportmanager::slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    if (ui->report_type->currentIndex() == 0 || ui->report_type->currentIndex() == 1 || ui->report_type->currentIndex() == 2) {
        QSqlQuery qry2(db2);
        qry2.prepare("SELECT * from shop_info");
        if (qry2.exec())
        {while (qry2.next())
            {
               shop_name = qry2.value(0).toString();
               tax = qry2.value(1).toString();
               trade = qry2.value(2).toString();
               shop_phone = qry2.value(3).toString();
            }
        }
    QString start = QString::number(ui->start->date().year())+"-"+QString::number(ui->start->date().month())+"-"+QString::number(ui->start->date().day())+" "+"00:00:00";
    QString end = QString::number(ui->end->date().year())+"-"+QString::number(ui->end->date().month())+"-"+QString::number(ui->end->date().day())+" "+"23:59:59";
    QStringList columns;
    columns << "start" << "end" << "supplier" << "storage"<<"buy_total"<<"sell_total"<<"quan_total"<<"customer"<<"shop_name"<<"shop_phone"<<"tax"<<"trade"<<"phone";
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
            data = std::round(buy_total);
        }else if (info.columnName == "sell_total") {
            data = std::round(sell_total);
        }else if (info.columnName == "quan_total") {
            data = quan_total;
        }else if (info.columnName == "customer") {
            if(ui->sale_custom_check->isChecked()&&!ui->sale_custom_name->currentText().isEmpty()){
            data = ui->sale_custom_name->currentText();}else {
            data = "جميع العملاء";
            }
        }else if (info.columnName == "shop_name") {
            data = shop_name;
        }else if (info.columnName == "phone") {
            data = shop_phone;
        }else if (info.columnName == "trade") {
            data = trade;
        }else if (info.columnName == "tax") {
            data = tax;
        }


        break;
    default: break;
    }
    }
	else if (ui->report_type->currentIndex() == 3) {
        QString start = QString::number(ui->start->date().year())+"-"+QString::number(ui->start->date().month())+"-"+QString::number(ui->start->date().day())+" "+"00:00:00";
        QString end = QString::number(ui->end->date().year())+"-"+QString::number(ui->end->date().month())+"-"+QString::number(ui->end->date().day())+" "+"23:59:59";
        QStringList columns;
        columns << "start" << "end"<<"sale_cash"<<"sale_credit"<<"sale_total"<<"sale_re_cash"<<"sale_re_credit"<<"sale_re_total"<<"sales_total"<<"sales_buy_total"<<"purchase_cash"<<"purchase_credit"<<"purchase_total"<<"purchase_re_cash"<<"purchase_re_credit"<<"purchase_re_total"<<"purchases_total"<<"diff_total"<<"expenses_total"<<"store_total"<<"profit_total"<<"profit_perc";
        QSqlQuery qry1(db);
        QSqlQuery qry(db);
        QSqlQuery qry2(db);
		QSqlQuery qry3(db);
		QSqlQuery qry4(db);
		QSqlQuery qry5(db);
		QSqlQuery qry6(db);
		QSqlQuery qry7(db);
		QSqlQuery qry8(db);
		QSqlQuery qry9(db);
		QSqlQuery qry10(db);
        double sale_buy_price,sale_cash,sale_credit,sale_total,sale_re_cash,sale_re_credit,sale_re_total,sales_total,purchase_cash,purchase_credit,purchase_total,purchase_re_cash,purchase_re_credit,purchase_re_total,purchases_total,diff_total,expenses_total,store_total;
        qry.prepare("SELECT SUM((sub_items.buy_price * sales_items.quantity)) AS 'total_buy' FROM `sales_items` JOIN item_balance ON item_balance.exp_barcode = sales_items.exp_barcode JOIN sub_items ON sub_items.id = item_balance.sub_item_id JOIN sales ON sales.rec_no = sales_items.rec_no WHERE sales.create_date BETWEEN '"+start+"'  AND '"+end+"'");
        if (qry.exec())
        {while (qry.next())
            {
               sale_buy_price = qry.value(0).toDouble();
            }
        }

        qry1.prepare("select sum(total) from sales where payment_method_id = 0 AND create_date BETWEEN '"+start+"'  AND '"+end+"' ");
		if (qry1.exec())
		{while (qry1.next())
			{
			   sale_cash = qry1.value(0).toDouble();
			}
		}
        qry2.prepare("select sum(total) from sales where payment_method_id = 1 AND create_date BETWEEN '"+start+"'  AND '"+end+"'");
		if (qry2.exec())
		{while (qry2.next())
			{
			   sale_credit = qry2.value(0).toDouble();
			}
		}
        qry3.prepare("select sum(total) from sales_return where payment_method_id = 0 AND create_date BETWEEN '"+start+"'  AND '"+end+"'");
		if (qry3.exec())
		{while (qry3.next())
			{
			   sale_re_cash = qry3.value(0).toDouble();
			}
		}
        qry4.prepare("select sum(total) from sales_return where payment_method_id = 1 AND create_date BETWEEN '"+start+"'  AND '"+end+"'");
		if (qry4.exec())
		{while (qry4.next())
			{
			   sale_re_credit = qry4.value(0).toDouble();
			}
		}
        qry5.prepare("select sum(total) from purchases where payment_method_id = 0 AND create_date BETWEEN '"+start+"'  AND '"+end+"'");
		if (qry5.exec())
		{while (qry5.next())
			{
			   purchase_cash = qry5.value(0).toDouble();
			}
		}
        qry6.prepare("select sum(total) from purchases where payment_method_id = 1 AND create_date BETWEEN '"+start+"'  AND '"+end+"'");
		if (qry6.exec())
		{while (qry6.next())
			{
			   purchase_credit = qry6.value(0).toDouble();
			}
		}
        qry7.prepare("select sum(total) from purchases_return where payment_method_id = 0 AND create_date BETWEEN '"+start+"'  AND '"+end+"'");
		if (qry7.exec())
		{while (qry7.next())
			{
			   purchase_re_cash = qry7.value(0).toDouble();
			}
		}
        qry8.prepare("select sum(total) from purchases_return where payment_method_id = 1 AND create_date BETWEEN '"+start+"'  AND '"+end+"'");
		if (qry8.exec())
		{while (qry8.next())
			{
			   purchase_re_credit = qry8.value(0).toDouble();
			}
		}
        qry9.prepare("select sum(total) from expenses WHERE created_at BETWEEN '"+start+"'  AND '"+end+"'");
		if (qry9.exec())
		{while (qry9.next())
			{
			   expenses_total = qry9.value(0).toDouble();
			}
		}
		qry10.prepare("select sum(item_balance.item_balance) As balance ,sub_items.buy_price ,item_balance.sub_item_id from item_balance join sub_items on item_balance.sub_item_id = sub_items.id GROUP BY item_balance.sub_item_id");
		if (qry10.exec())
		{while (qry10.next())
			{
			   store_total += qry10.value(0).toDouble()*qry10.value(1).toDouble();
			}
		}
		
        sale_total = sale_cash + sale_credit ;
        sales_total = sale_total -(sale_re_cash + sale_re_credit);
        purchase_total = purchase_cash + purchase_credit;
        purchases_total = purchase_total - (purchase_re_credit + purchase_re_cash);
        diff_total = sales_total-sale_buy_price;

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
            }
			else if (info.columnName == "sale_cash") {
                data = sale_cash;
            }
			else if (info.columnName == "sale_credit") {
                data = sale_credit;
            }
			else if (info.columnName == "sale_total") {
                data = sale_total;
            }
			else if (info.columnName == "sale_re_cash") {
                data = sale_re_cash;
            }
			else if (info.columnName == "sale_re_credit") {
                data = sale_re_credit;
            }
            else if (info.columnName == "sales_total") {
                data = std::round(sales_total);
            }else if (info.columnName == "sales_buy_total") {
                data = std::round(sale_buy_price);
            }
			else if (info.columnName == "purchase_cash") {
                data = purchase_cash;
            }
			else if (info.columnName == "purchase_credit") {
                data = purchase_credit;
            }
			else if (info.columnName == "purchase_total") {
                data = purchase_total;
            }
			else if (info.columnName == "purchase_re_cash") {
                data = purchase_re_cash;
            }
			else if (info.columnName == "purchase_re_credit") {
                data = purchase_re_credit;
            }
			else if (info.columnName == "purchases_total") {
                data = std::round(purchases_total) ;
            }
			else if (info.columnName == "diff_total") {
                data = std::round(diff_total);
            }
			else if (info.columnName == "expenses_total") {
                data = std::round(expenses_total);
            }
			else if (info.columnName == "store_total") {
                data = std::round(store_total);
            }
            else if (info.columnName == "profit_total") {
                data = std::round(diff_total-expenses_total);
            }
            else if (info.columnName == "profit_perc") {
                data = std::round(((diff_total-expenses_total)/sales_total)*100);
            }
            break;
        default: break;
        }
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

void reportmanager::on_pur_item_name_check_toggled(bool checked)
{
    if(checked == true){
        ui->pur_item_name->setEnabled(true);

    }else {
        ui->pur_item_name->setEnabled(false);
        ui->pur_item_name->lineEdit()->clear();

    }
}

void reportmanager::on_sale_custom_check_toggled(bool checked)
{
    if(checked == true && ui->report_type->currentIndex() == 1){
        ui->sale_custom_name->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_custom_name->lineEdit()->clear();
        ui->sale_custom_name->setEnabled(false);
    }
}

void reportmanager::on_sale_cash_depo_check_toggled(bool checked)
{
    if(checked == true && ui->report_type->currentIndex() == 1){
        ui->sale_cash_depo_name->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_cash_depo_name->setEnabled(false);
        ui->sale_cash_depo_name->lineEdit()->clear();

    }
}

void reportmanager::on_sale_seller_check_toggled(bool checked)
{
    if(checked == true && ui->report_type->currentIndex() == 1){
        ui->sale_seller_name->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_seller_name->setEnabled(false);
        ui->sale_seller_name->lineEdit()->clear();
    }
}



void reportmanager::on_sale_barcode_check_toggled(bool checked)
{
    if(checked == true && ui->report_type->currentIndex() == 1){
        ui->sale_barcode->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_barcode->setEnabled(false);
        ui->sale_barcode->clear();
    }
}

void reportmanager::on_sale_int_check_toggled(bool checked)
{
    if(checked == true &&ui->report_type->currentIndex() == 1){
        ui->sale_int->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_int->setEnabled(false);
        ui->sale_int->clear();

    }
}

void reportmanager::on_sale_item_name_check_toggled(bool checked)
{
    if(checked == true && ui->report_type->currentIndex() == 1){
        ui->sale_item_name->setEnabled(true);
    }
    else if (checked == false) {
        ui->sale_item_name->setEnabled(false);
        ui->sale_item_name->lineEdit()->clear();
    }
}

void reportmanager::on_pushButton_2_clicked()
{
    QString myqry;
    QString start = QString::number(ui->start->date().year())+"-"+QString::number(ui->start->date().month())+"-"+QString::number(ui->start->date().day())+" "+"00:00:00";
    QString end = QString::number(ui->end->date().year())+"-"+QString::number(ui->end->date().month())+"-"+QString::number(ui->end->date().day())+" "+"23:59:59";
    myqry = "WITH "
        "sales as(SELECT exp_barcode , SUM(quantity) as quantity FROM sales_items WHERE sales_items.create < '"+end+"' group by exp_barcode) , "
        "sales_return as (SELECT exp_barcode , SUM(quantity) as quantity FROM sales_return_items WHERE sales_return_items.create < '"+end+"' group by exp_barcode),"
        "purchases as (SELECT exp_barcode , SUM(quantity) as quantity ,buy_price, sell_price FROM purchases_items WHERE purchases_items.create < '"+end+"' group by exp_barcode),"
        "purchases_return as(SELECT exp_barcode , SUM(quantity) as quantity FROM purchases_return_items WHERE purchases_return_items.create < '"+end+"' group by exp_barcode)  "

        "SELECT items.name as ' item_name'"
        ", purchases.exp_barcode as 'barcode'"
        ", purchases.buy_price"
        ", purchases.sell_price"
        ", (purchases.quantity - IFNULL(purchases_return.quantity,0)- IFNULL(sales.quantity,0)+ IFNULL(sales_return.quantity,0))as 'item_balance'"
        ", (purchases.buy_price*(purchases.quantity - IFNULL(purchases_return.quantity,0)- IFNULL(sales.quantity,0)+ IFNULL(sales_return.quantity,0)))as 'buy_total_per'"
        ", (purchases.sell_price*(purchases.quantity - IFNULL(purchases_return.quantity,0)- IFNULL(sales.quantity,0)+ IFNULL(sales_return.quantity,0)))as 'sell_total_per'"
        ", item_balance.expire , purchases.quantity as 'buy_quan' , purchases_return.quantity as 'buy_return_quan',sales.quantity as 'sale_quan', sales_return.quantity as 'sale_return_quan' "
        " FROM purchases "
        " JOIN item_balance on purchases.exp_barcode = item_balance.exp_barcode"
        " JOIN sub_items on sub_items.id = item_balance.sub_item_id"
        " JOIN items on sub_items.item_id = items.id "
        " LEFT JOIN purchases_return on purchases_return.exp_barcode = item_balance.exp_barcode"
        " LEFT JOIN sales on sales.exp_barcode = item_balance.exp_barcode"
        " LEFT JOIN sales_return on sales_return.exp_barcode = item_balance.exp_barcode"
        " group by purchases.exp_barcode";
    report = new LimeReport::ReportEngine(this);
    model2->setQuery(myqry,db);
    qDebug()<< model2->query().lastQuery();
    report->dataManager()->addModel("buy",model2,false);
    report->loadFromFile(":/Rc/Xml/inv.lrxml");
    report->designReport();
}


