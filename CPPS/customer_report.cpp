#include "customer_report.h"
#include "ui_customer_report.h"

customer_report::customer_report(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::customer_report)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("test");
    db2 = QSqlDatabase::database("setting");
    if(!db.isOpen()){
    if(db.open()){

    }
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
    model = new QSqlQueryModel(this);
    model1 = new QSqlQueryModel(this);
    model2 = new QSqlQueryModel(this);
    model3 = new QSqlQueryModel(this);
    model4 = new QSqlQueryModel(this);
    model5 = new QSqlQueryModel(this);
    model6 = new QSqlQueryModel(this);
    clearall();
}

customer_report::~customer_report()
{
    delete ui;
}

void customer_report::clearall()
{
    db.close();
    db.open();
    double total_balance ;
    ui->puchase_cash_view_6->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->puchase_credit_view_6->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->payment_view_6->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->pur_cash_return_view_6->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->pur_credit_return_view_6->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->supplier_view_6->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    model->setQuery("SELECT name,id FROM customers ",db);
    ui->customer->setModel(model);
    model4->setQuery("SELECT name AS 'اسم العميل' , balance AS 'رصيد' FROM customers",db);
    ui->supplier_view_6->setModel(model4);
    QSqlQuery qry(db);
    qry.prepare("SELECT SUM(balance) FROM customers");
    if(qry.exec()){
        while (qry.next()) {
          total_balance = qry.value(0).toDouble();
        }
    }
    ui->total_balance_6->setText(QString::number(total_balance));
}

void customer_report::on_checkBox_toggled(bool checked)
{

}

void customer_report::on_customer_currentIndexChanged(int index)
{
    QString id =  model->index(index,1).data().toString();
    QString balance ;
    model1->setQuery("SELECT CONCAT(rec_no,' ',' فاتورة رقم') AS 'رقم الفاتورة' , total AS 'مجموع' , create_date AS 'تاريخ' FROM sales WHERE customer_id = '"+id+"' AND payment_method_id = 0 ",db);
    model3->setQuery("SELECT CONCAT(rec_no,' ',' فاتورة رقم') AS 'رقم الفاتورة' , total AS 'مجموع' , create_date AS 'تاريخ' FROM sales_return WHERE customer_id = '"+id+"' AND payment_method_id = 0 ",db);
    model5->setQuery("SELECT CONCAT(rec_no,' ',' فاتورة رقم') AS 'رقم الفاتورة' , total AS 'مجموع' , create_date AS 'تاريخ' FROM sales WHERE customer_id = '"+id+"' AND payment_method_id = 1 ",db);
    model6->setQuery("SELECT CONCAT(rec_no,' ',' فاتورة رقم') AS 'رقم الفاتورة' , total AS 'مجموع' , create_date AS 'تاريخ' FROM sales_return WHERE customer_id = '"+id+"' AND payment_method_id = 1 ",db);
    model2->setQuery("SELECT CONCAT(rec_no,' ',' فاتورة رقم') AS 'رقم الفاتورة' , amount AS 'مبلغ' , created_at AS 'تاريخ' FROM customer_payment WHERE customer_id = '"+id+"'",db);
    ui->puchase_cash_view_6->setModel(model1);
    ui->payment_view_6->setModel(model2);
    ui->puchase_credit_view_6->setModel(model5);
    ui->pur_credit_return_view_6->setModel(model6);
    ui->pur_cash_return_view_6->setModel(model3);
    QSqlQuery qry(db);
    qry.prepare("SELECT balance FROM customers WHERE id = '"+id+"' ");
    if(qry.exec()){
        while (qry.next()) {
           balance = qry.value(0).toString();
        }
    }
    double re_total = 0;
    double pur_total = 0;
    QSqlQuery qry1(db);
    qry1.prepare("SELECT SUM(total) FROM sales WHERE customer_id = '"+id+"' AND payment_method_id = 0 ");
    if(qry1.exec()){
        while (qry1.next()) {
           ui->pur_cash_total_6->setText(qry1.value(0).toString());
           pur_total += qry1.value(0).toDouble();
        }
    }
    QSqlQuery qry2(db);
    qry2.prepare("SELECT SUM(total) FROM sales_return WHERE customer_id = '"+id+"' AND payment_method_id = 0 ");
    if(qry2.exec()){
        while (qry2.next()) {
           ui->pur_re_cash_total_6->setText( qry2.value(0).toString());
           re_total += qry2.value(0).toDouble();
        }
    }
    QSqlQuery qry4(db);
    qry4.prepare("SELECT SUM(total) FROM sales WHERE customer_id = '"+id+"' AND payment_method_id = 1 ");
    if(qry4.exec()){
        while (qry4.next()) {
           ui->pur_credit_total_6->setText( qry4.value(0).toString());
           pur_total += qry4.value(0).toDouble();
        }
    }
    QSqlQuery qry5(db);
    qry5.prepare("SELECT SUM(total) FROM sales_return WHERE customer_id = '"+id+"' AND payment_method_id = 1  ");
    if(qry5.exec()){
        while (qry5.next()) {
           ui->pur_re_credit_total_6->setText( qry5.value(0).toString());
           re_total += qry5.value(0).toDouble();
        }
    }
    ui->pur_total_6->setText(QString::number(pur_total));
    ui->pur_re_total_6->setText(QString::number(re_total));

    QSqlQuery qry3(db);
    qry3.prepare("SELECT SUM(amount) FROM customer_payment WHERE customer_id = '"+id+"' ");
    if(qry3.exec()){
        while (qry3.next()) {
           ui->payment_total_6->setText(qry3.value(0).toString());
        }
    }


    ui->balance->setText(balance);
}



void customer_report::on_checkBox_6_clicked(bool checked)
{
    if(checked == true){

        model4->setQuery("SELECT name AS 'اسم العمسل' , balance AS 'رصيد' FROM customers WHERE balance > 0",db);
        ui->supplier_view_6->setModel(model4);

    }else {
        model4->setQuery("SELECT name AS 'اسم العميل' , balance AS 'رصيد' FROM customers",db);
        ui->supplier_view_6->setModel(model4);
    }
}

void customer_report::on_pushButton_clicked()
{
    report = new LimeReport::ReportEngine(this);
        LimeReport::ICallbackDatasource *ds =report->dataManager()->createCallbackDatasouce("ds");
        connect(ds,SIGNAL(getCallbackData(const LimeReport::CallbackInfo& , QVariant& )),
                this,SLOT(slotGetCallbackData(const LimeReport::CallbackInfo& , QVariant&)));

        report->dataManager()->addModel("re_cash",ui->pur_cash_return_view_6->model(),false);
        report->dataManager()->addModel("re_cre",ui->pur_credit_return_view_6->model(),false);
        report->dataManager()->addModel("cash",ui->puchase_cash_view_6->model(),false);
        report->dataManager()->addModel("credit",ui->puchase_credit_view_6->model(),false);
        report->dataManager()->addModel("payment",ui->payment_view_6->model(),false);
        report->loadFromFile(":/Rc/Xml/custom_rep.lrxml");
        report->previewReport();
}

void customer_report::slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
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
        columns <<"shop_name"<<"phone"<<"tax"<<"trade"<<"balance"<<"name";
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
            if(info.columnName == "balance"){
                data = ui->balance->text();
            }else if (info.columnName == "name") {
                data = ui->customer->currentText();
            }else if (info.columnName == "shop_name") {
                data = shop_name;
            }else if (info.columnName == "phone") {
                data = shop_phone;
            }else if (info.columnName == "trade") {
                data = trade;
            }
            break;
        default: break;
        }
}
