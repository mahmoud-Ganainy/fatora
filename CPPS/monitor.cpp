#include "monitor.h"
#include "ui_monitor.h"



monitor::monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::monitor)
{
    db = QSqlDatabase::database("test");
    db2 = QSqlDatabase::database("setting");
    if(!db.isOpen()){
    if(db.open()){}
    else {
      QMessageBox::warning(this,tr("error!!@!"),"لم يتم الاتصال بقاعدة البيانات",QMessageBox::Yes);
    }
    }
    if(!db2.isOpen()){
    if(db2.open()){}
    else {
     QMessageBox::warning(this,tr("error!!@!"),"لم يتم الاتصال بقاعدة البيانات",QMessageBox::Yes);
    }
    }
    ui->setupUi(this);
    model = new QSqlQueryModel(this);
    ui->start->setDate(QDate::currentDate());
    ui->end->setDate(QDate::currentDate());
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->all->setChecked(true);
    getstat(0);
    getTable();


}
void monitor::getTable()
{
    sales_cash=0;
    sales_credit=0;
    sales_return_cash=0;
    sales_return_credit=0;
    purchases_cash=0;
    purchases_credit=0;
    purchases_return_cash=0;
    purchases_return_credit=0;
    supplier_payment_out=0;
    supplier_payment_in=0;
    customer_payment_in=0;
    customer_payment_out=0;
    expenses_cash=0;
    expenses_credit=0;
    sales_total=0;
    purchases_total=0;
    purchases_return_total=0;
    cash_deposit_total=0;
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    QString start = QString::number(ui->start->date().year())+"-"+QString::number(ui->start->date().month())+"-"+QString::number(ui->start->date().day())+" "+"00:00:00";
    QString end = QString::number(ui->end->date().year())+"-"+QString::number(ui->end->date().month())+"-"+QString::number(ui->end->date().day())+" "+"23:59:59";

    if(ui->all->isChecked()){
        model->setQuery("SELECT CONCAT(rec_no,' ','مبيعات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `sales` WHERE create_date BETWEEN '"+start+"' AND '"+end+"' UNION SELECT CONCAT(rec_no,' ','مرتجع مبيعات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `sales_return` WHERE create_date BETWEEN '"+start+"'  AND '"+end+"' UNION SELECT CONCAT(rec_no,' ','مشتريات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `purchases` WHERE create_date BETWEEN '"+start+"'  AND '"+end+"' UNION SELECT CONCAT(rec_no,' ',' مرتجع مشتريات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `purchases_return` WHERE create_date BETWEEN '"+start+"'  AND '"+end+"' UNION SELECT CONCAT(rec_no,' ',customers.name,'- ',' دفعة عميل برقم') AS 'record number',`amount`,`created_at` FROM `customer_payment` JOIN customers ON customer_payment.customer_id = customers.id WHERE created_at BETWEEN '"+start+"'  AND '"+end+"' UNION SELECT CONCAT(rec_no,' ',supplier.name,'- ',' دفعة لمورد برقم') AS 'record number',`amount`,`created_at` FROM `supplier_payment` JOIN supplier ON supplier_payment.supplier_id = supplier.id WHERE created_at BETWEEN '"+start+"'  AND '"+end+"' UNION SELECT CONCAT(rec_no,' ','مصرفات فاتورة رقم') AS 'record number',`total`,`created_at` FROM `expenses` WHERE created_at BETWEEN '"+start+"'  AND '"+end+"' ",db);
        proxyModel->setSourceModel(model);
        ui->tableView->setModel(proxyModel);

    }else if (ui->sales->isChecked()) {
        model->setQuery("SELECT CONCAT(rec_no,' ','مبيعات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `sales` WHERE create_date BETWEEN '"+start+"' AND '"+end+"' ",db);
        proxyModel->setSourceModel(model);

        ui->tableView->setModel(proxyModel);
    }else if (ui->return_sales->isChecked()) {
        model->setQuery("SELECT CONCAT(rec_no,' ','مرتجع مبيعات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `sales_return` WHERE create_date BETWEEN '"+start+"'  AND '"+end+"'",db);
        proxyModel->setSourceModel(model);

        ui->tableView->setModel(proxyModel);
    }else if (ui->buy->isChecked()) {
        model->setQuery("SELECT CONCAT(rec_no,' ','مشتريات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `purchases` WHERE create_date BETWEEN '"+start+"'  AND '"+end+"'",db);

        proxyModel->setSourceModel(model);
        ui->tableView->setModel(proxyModel);
    }else if (ui->buy_return->isChecked()) {
        model->setQuery("SELECT CONCAT(rec_no,' ',' مرتجع مشتريات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `purchases_return` WHERE create_date BETWEEN '"+start+"'  AND '"+end+"'",db);
        ui->tableView->setModel(proxyModel);
    }else if (ui->custom_payment->isChecked()) {
        model->setQuery("SELECT CONCAT(rec_no,' ',customers.name,'- ',' دفعة عميل برقم') AS 'record number',`amount`,`created_at` FROM `customer_payment` JOIN customers ON customer_payment.customer_id = customers.id WHERE created_at BETWEEN '"+start+"'  AND '"+end+"'",db);
        proxyModel->setSourceModel(model);

        ui->tableView->setModel(proxyModel);
    }else if (ui->supplier_payment->isChecked()) {
        model->setQuery("SELECT CONCAT(rec_no,' ',supplier.name,'- ',' دفعة لمورد برقم') AS 'record number',`amount`,`created_at` FROM `supplier_payment` JOIN supplier ON supplier_payment.supplier_id = supplier.id WHERE created_at BETWEEN '"+start+"'  AND '"+end+"'",db);
        proxyModel->setSourceModel(model);

        ui->tableView->setModel(proxyModel);
    }else if (ui->expenses->isChecked()) {
        model->setQuery("SELECT CONCAT(rec_no,' ','مصرفات فاتورة رقم') AS 'record number',`total`,`created_at` FROM `expenses` WHERE created_at BETWEEN '"+start+"'  AND '"+end+"'",db);
        proxyModel->setSourceModel(model);

        ui->tableView->setModel(proxyModel);
    }



    //SELECT CONCAT(rec_no," ",'مبيعات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `sales`
    //SELECT CONCAT(rec_no," ",'مرتجع مبيعات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `sales_return`
    //SELECT CONCAT(rec_no," ",'مشتريات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `purchases`
    //SELECT CONCAT(rec_no," ",' مرتجع مشتريات فاتورة رقم') AS 'record number',`total`,`create_date` FROM `purchases_return`
    //SELECT CONCAT(rec_no," ",supplier.name,'- ',' دفعة لمورد برقم') AS 'record number',`amount`,`created_at` FROM `supplier_payment` JOIN supplier ON supplier_payment.supplier_id = supplier.id
    //SELECT CONCAT(rec_no," ",customers.name,'- ',' دفعة عميل برقم') AS 'record number',`amount`,`created_at` FROM `customer_payment` JOIN customers ON customer_payment.customer_id = customers.id
    //SELECT CONCAT(rec_no," ",'مصرفات فاتورة رقم') AS 'record number',`total`,`created_at` FROM `expenses`

}

void monitor::getstat(int id)
{
     sales_cash=0;
     sales_credit=0;
     sales_return_cash=0;
     sales_return_credit=0;
     purchases_cash=0;
     purchases_credit=0;
     purchases_return_cash=0;
     purchases_return_credit=0;
     supplier_payment_out=0;
     supplier_payment_in=0;
     customer_payment_in=0;
     customer_payment_out=0;
     expenses_cash=0;
     expenses_credit=0;
     sales_total=0;
     purchases_total=0;
     purchases_return_total=0;
     cash_deposit_total=0;
     QString start = QString::number(ui->start->date().year())+"-"+QString::number(ui->start->date().month())+"-"+QString::number(ui->start->date().day())+" "+"00:00:00";
     QString end = QString::number(ui->end->date().year())+"-"+QString::number(ui->end->date().month())+"-"+QString::number(ui->end->date().day())+" "+"23:59:59";


     if(id == 0){
         //sales
         QSqlQuery qry(db);
             qry.prepare("SELECT `total` FROM `sales` WHERE `payment_method_id` = '0' AND create_date BETWEEN :start  AND :end ");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);

             if( qry.exec() ){
                 while (qry.next())
                 {
                     sales_cash += qry.value(0).toDouble();
                 }
             }
             else {
                 qDebug()<<qry.lastError();
             }
             qry.prepare("SELECT `total` FROM `sales` WHERE `payment_method_id` = '1' AND create_date BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     sales_credit += qry.value(0).toDouble();
                 }
             }

             qry.prepare("SELECT `total` FROM `sales_return` WHERE `payment_method_id` = '0' AND create_date BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     sales_return_cash += qry.value(0).toDouble();
                 }
             }


             qry.prepare("SELECT `total` FROM `sales_return` WHERE `payment_method_id` = '1' AND create_date BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     sales_return_credit += qry.value(0).toDouble();
                 }
             }


         // purchase

             qry.prepare("SELECT `total` FROM `purchases` WHERE `payment_method_id` = '0' AND create_date BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     purchases_cash += qry.value(0).toDouble();
                 }
             }

             qry.prepare("SELECT `total` FROM `purchases` WHERE `payment_method_id` = '1' AND create_date BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     purchases_credit += qry.value(0).toDouble();
                 }
             }


             qry.prepare("SELECT `total` FROM `purchases_return` WHERE `payment_method_id` = '0' AND create_date BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     purchases_return_cash += qry.value(0).toDouble();
                 }
             }



             qry.prepare("SELECT `total` FROM `purchases_return` WHERE `payment_method_id` = '1' AND create_date BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     purchases_return_credit += qry.value(0).toDouble();
                 }
             }




         //supplier_payment

             qry.prepare("SELECT `amount` FROM `supplier_payment` WHERE `flow` = '0' AND created_at BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     supplier_payment_out += qry.value(0).toDouble();
                 }
             }

             qry.prepare("SELECT `amount` FROM `supplier_payment` WHERE `flow` = '1' AND created_at BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     supplier_payment_in += qry.value(0).toDouble();
                 }
             }
         // customer_payment

             qry.prepare("SELECT `amount` FROM `customer_payment` WHERE `flow` = '0' AND created_at BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     customer_payment_out += qry.value(0).toDouble();
                 }
             }

             qry.prepare("SELECT `amount` FROM `customer_payment` WHERE `flow` = '1' AND created_at BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     customer_payment_in += qry.value(0).toDouble();
                 }
             }
         // expenses

             qry.prepare("SELECT `total` FROM `expenses` WHERE `payment_method` = '0' AND created_at BETWEEN :start  AND :end");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     expenses_cash += qry.value(0).toDouble();
                 }
             }
             qry.prepare("SELECT `total` FROM `expenses` WHERE `payment_method` = '1' AND created_at BETWEEN :start  AND :end ");
             qry.bindValue(":start",start);
             qry.bindValue(":end",end);
             if( qry.exec() ){
                 while (qry.next())
                 {
                     expenses_credit += qry.value(0).toDouble();
                 }
             }



             qDebug()<<sales_cash<<expenses_credit<<purchases_return_credit<<qry.lastQuery();

             ui->sales_cash->setText(QString::number(sales_cash));
             ui->sales_credit->setText(QString::number(sales_credit));

             ui->sales_return_cash->setText(QString::number(sales_return_cash));
             ui->sales_return_credit->setText(QString::number(sales_return_credit));
             ui->purchases_cash->setText(QString::number(purchases_cash));
             ui->purchases_credit->setText(QString::number(purchases_credit));
             ui->purchases_return_cash->setText(QString::number(purchases_return_cash));
             ui->purchases_return_credit->setText(QString::number(purchases_return_credit));
             ui->supp_payment_out->setText(QString::number(supplier_payment_out));
             ui->supp_payment_in->setText(QString::number(supplier_payment_in));
             ui->custom_payment_in->setText(QString::number(customer_payment_in));
             ui->custom_payment_out->setText(QString::number(customer_payment_out));
             ui->expense_cash->setText(QString::number(expenses_cash));
             ui->expense_credit->setText(QString::number(expenses_credit));



              sales_total = sales_cash + sales_credit;
              purchases_total = purchases_cash + purchases_credit;
              purchases_return_total = purchases_return_cash + purchases_return_credit;
              cash_deposit_total = sales_cash-sales_return_cash-purchases_cash+purchases_return_cash-supplier_payment_out+supplier_payment_in+customer_payment_in-customer_payment_out-expenses_cash;
              ui->sales_total->setText(QString::number(sales_total));
              ui->purchases_total->setText(QString::number(purchases_total));
              ui->purchases_return_total->setText(QString::number(purchases_return_total));
              ui->cash_depo_total->setText(QString::number(cash_deposit_total));

     }
}
monitor::~monitor()
{
    delete ui;
}  
   /*
//sales
SqlQuery qry(db);
    qry.prepare("SELECT `total` FROM `sales` WHERE `payment_method_id` = '0' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            sales_cash += qry.value(0).toDouble()
        }
    }   
SqlQuery qry(db);
    qry.prepare("SELECT `total` FROM `sales` WHERE `payment_method_id` = '1' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            sales_credit += qry.value(0).toDouble()
        }
    }
    
SqlQuery qry(db);
    qry.prepare("SELECT `total` FROM `sales_return` WHERE `payment_method_id` = '0' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            sales_return_cash += qry.value(0).toDouble()
        }
    }

   
SqlQuery qry(db);
    qry.prepare("SELECT `total` FROM `sales_return` WHERE `payment_method_id` = '1' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            sales_return_credit += qry.value(0).toDouble()
        }
    }
	
	
// purchase	
SqlQuery qry(db);
    qry.prepare("SELECT `total` FROM `purchases` WHERE `payment_method_id` = '0' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            purchases_cash += qry.value(0).toDouble()
        }
    }   
SqlQuery qry(db);
    qry.prepare("SELECT `total` FROM `purchases` WHERE `payment_method_id` = '1' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            purchases_credit += qry.value(0).toDouble()
        }
    }
    
SqlQuery qry(db);
    qry.prepare("SELECT `total` FROM `purchases_return` WHERE `payment_method_id` = '0' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            purchases_return_cash += qry.value(0).toDouble()
        }
    }

   
SqlQuery qry(db);
    qry.prepare("SELECT `total` FROM `purchases_return` WHERE `payment_method_id` = '1' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            purchases_return_credit += qry.value(0).toDouble()
        }
    }
	
	
	
	
//supplier_payment	
SqlQuery qry(db);
    qry.prepare("SELECT `amount` FROM `supplier_payment` WHERE `flow` = '0' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            supplier_payment_out += qry.value(0).toDouble()
        }
    }
SqlQuery qry(db);
    qry.prepare("SELECT `amount` FROM `supplier_payment` WHERE `flow` = '1' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            supplier_payment_in += qry.value(0).toDouble()
        }
    }
// customer_payment	
SqlQuery qry(db);
    qry.prepare("SELECT `amount` FROM `customer_payment` WHERE `flow` = '0' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            customer_payment_out += qry.value(0).toDouble()
        }
    }
SqlQuery qry(db);
    qry.prepare("SELECT `amount` FROM `customer_payment` WHERE `flow` = '1' AND cash_deposit_id = "+cash_id+"");
    if( qry.exec() ){
        while (qry.next())
        {
            customer_payment_in += qry.value(0).toDouble()
        }
    }
// expenses	
	
SqlQuery qry(db);
    qry.prepare("SELECT `amount` FROM `expenses_details` JOIN expenses on expenses.rec_no = expenses_details.expense_id AND expenses.payment_method = 0 AND expenses_details.cash_deposit_id = "+cash_id+" ");
    if( qry.exec() ){
        while (qry.next())
        {
            expenses_cash += qry.value(0).toDouble()
        }
    }	
SqlQuery qry(db);
    qry.prepare("SELECT `amount` FROM `expenses_details` JOIN expenses on expenses.rec_no = expenses_details.expense_id AND expenses.payment_method = 1 AND expenses_details.cash_deposit_id = "+cash_id+" ");
    if( qry.exec() ){
        while (qry.next())
        {
            expenses_credit += qry.value(0).toDouble()
        }
    }
   */
   
   
   
void monitor::on_comboBox_currentIndexChanged(int index)
{

}

void monitor::on_pushButton_2_clicked()
{
    getstat(0);
    getTable();
    qDebug()<<ui->start->date().toString();
}

void monitor::on_pushButton_clicked()
{

    QString printer_name;
    QSqlQuery qry(db2);
    QSqlQuery qry1(db);
    QSqlQuery qry2(db);
    QSqlQuery qry3(db);
    qry.prepare("SELECT print_name from print_info WHERE print_type = 2");
    if (qry.exec())
    {while (qry.next())
        {
           printer_name = qry.value(0).toString();
        }
    }
    QPrinter * rec_print;
    rec_print = new QPrinter();
    rec_print->setPrinterName(printer_name);

    report = new LimeReport::ReportEngine(this);
    LimeReport::ICallbackDatasource *ds =report->dataManager()->createCallbackDatasouce("ds");
    connect(ds,SIGNAL(getCallbackData(const LimeReport::CallbackInfo& , QVariant& )),
            this,SLOT(slotGetCallbackData(const LimeReport::CallbackInfo& , QVariant&)));
    report->loadFromFile(":/Rc/Xml/daily.lrxml");
    report->previewReport();

}

void monitor::slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
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
    QStringList columns;
    columns << "start" << "end" << "sales_total" << "sales_cash"<<"sales_credit"<<"sales_return_total"<<"sales_return_cash"<<"sales_return_credit"<<"purchases_total"<<"purchases_cash"<<"purchases_credit"<<"purchases_return_total"<<"purchases_return_cash"<<"purchases_return_credit"<<"customer_payment_out"<<"customer_payment_in"<<"supplier_payment_out"<<"supplier_payment_in"<<"expenses_cash"<<"cash_deposit_total"<<"shop_name"<<"phone"<<"tax"<<"trade";
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
            data = ui->start->date();
        }else if (info.columnName == "end") {
            data = ui->end->date() ;
        }else if (info.columnName == "shop_name") {
            data = shop_name;
        }else if (info.columnName == "phone") {
            data = shop_phone;
        }else if (info.columnName == "trade") {
            data = trade;
        }else if (info.columnName == "sales_total") {
            data = sales_total;
        }else if (info.columnName == "sales_cash") {
            data = sales_cash;
        }else if (info.columnName == "sales_credit") {
            data = sales_credit;
        }else if (info.columnName == "sales_return_cash") {
            data = sales_return_cash;
        }else if (info.columnName == "sales_return_credit") {
            data = sales_return_credit;
        }else if (info.columnName == "purchases_total") {
            data = purchases_total;
        }else if (info.columnName == "purchases_cash") {
            data = purchases_cash;
        }else if (info.columnName == "purchases_credit") {
            data = purchases_credit;
        }else if (info.columnName == "purchases_return_total") {
            data = purchases_return_total;
        }else if (info.columnName == "purchases_return_cash") {
            data = purchases_return_cash;
        }else if (info.columnName == "purchases_return_credit") {
            data = purchases_return_credit;
        }else if (info.columnName == "customer_payment_out") {
            data = customer_payment_out;
        }else if (info.columnName == "customer_payment_in") {
            data = customer_payment_in;
        }else if (info.columnName == "supplier_payment_out") {
            data = supplier_payment_out;
        }else if (info.columnName == "supplier_payment_in") {
            data = supplier_payment_in;
        }else if (info.columnName == "expenses_cash") {
            data = expenses_cash;
        }else if (info.columnName == "cash_deposit_total") {
            data = cash_deposit_total;
        }
        break;
    default: break;
    }
}
