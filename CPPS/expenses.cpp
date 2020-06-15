#include "expenses.h"
#include "ui_expenses.h"
expenses::expenses(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::expenses)
{
    ui->setupUi(this);
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
    this->model = new QSqlQueryModel();
    this->model1 = new QSqlQueryModel();
    this->model2 = new QSqlQueryModel();
    this->model3 = new QSqlQueryModel();
    this->model4 = new QSqlQueryModel();
    this->model5 = new QSqlQueryModel();

    //model->setQuery("SELECT name ,id FROM supplier",db);

    //box->setModel(model);


    //ui->cashier_name->setModel(model4);
    QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));
        timer->start(1000);

    ClearAll();
}

expenses::~expenses()
{
    delete ui;
}
void expenses::ClearAll()
{
    saved = false;
    ui->total_after->setText("0");
    ui->out->setChecked(true);
    ui->new_expense->setChecked(true);
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    QComboBox *box;
    QComboBox *box1;
    QComboBox *box2;
    QComboBox *box3;
    box = new QComboBox();
    box->setEditable(true);
    box1 = new QComboBox();
    box1->setEditable(true);
    box2 = new QComboBox();
    box2->setEditable(true);
    box3 = new QComboBox();
    box3->setEditable(true);
	ui->tableView->hide();
    ui->tableWidget->show();
    QSqlQuery qry(db);
    QStringList inlist;
    QSqlQuery qry1(db);
    qry1.prepare("SELECT MAX(rec_no) from expenses");
    if (qry1.exec())
     {
       while (qry1.next())
       {
       ui->rec_no->setText(QString::number(qry1.value(0).toInt()+1));

       }
     }
    qry.prepare("SELECT DISTINCT expense_item ,expense_details FROM expenses_details");
    if (qry.exec())
    {
        while (qry.next())
        {

            box->addItem(qry.value(0).toString());
            box1->addItem(qry.value(1).toString());
        }
    }
    else{
        QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
          qDebug() << "Error getting shit:\n" << qry.lastError();
   }
    model->setQuery("SELECT name ,id FROM cash_deposit",db);
    model1->setQuery("SELECT name ,id FROM employees WHERE appear_as_cashier = 1",db);
    //model2->setQuery("SELECT name ,id FROM supplier WHERE type = 2 OR type = 3 ",db);
    //ui->supplier_name->setCurrentIndex(0);

    box2->setModel(model);
    box3->setModel(model1);   
    //ui->supplier_name->setModel(model2);
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,0,box);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,2,box1);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,3,box2);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,4,box3);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"بند الصرف"<<"المبلغ"<<"تفاصيل المصروف"<<"الخزنة"<<"الكاشير");
    box->clearEditText();
    box1->clearEditText();
    box2->clearEditText();
    box3->clearEditText();
	set_edit();



}

void expenses::set_edit()
{
    //ui->supplier_name->lineEdit()->setReadOnly(false);
    ui->dateTimeEdit->setReadOnly(false);
	ui->in->setEnabled(true);
	ui->out->setEnabled(true);
    ui->new_expense->setEnabled(true);
    ui->old_expense->setEnabled(true);

	
}

void expenses::set_unedit()
{
    //ui->supplier_name->lineEdit()->setReadOnly(true);
    ui->dateTimeEdit->setReadOnly(true);
	ui->in->setEnabled(false);
	ui->out->setEnabled(false);
    ui->new_expense->setEnabled(false);
    ui->old_expense->setEnabled(false);
}

void expenses::save()
{
	QString rec_no = ui->rec_no->text();
	double total = ui->total_after->text().toDouble();
    //QString supplier_id = model2->index(ui->supplier_name->currentIndex(),1).data().toString();
    int rowCount = ui->tableWidget->rowCount();
    int flow;
    int expense_age;
    QDate create_date = ui->dateTimeEdit->date();

    if(ui->out->isChecked()){
        flow =0;
    }else if (ui->in->isChecked()) {
        flow = 1;
    }
    if(ui->next_rec->isChecked()){
        expense_age = 0;
    }else if (ui->old_expense->isChecked()) {
        expense_age = 1;
    }
	int recCount = 0;
        QSqlQuery qry(db);
        qry.prepare("SELECT * FROM expenses WHERE rec_no = "+rec_no+"");
        if( qry.exec() ){
            while (qry.next())
            {
                recCount++;
            }
        }

        else{
              QMessageBox::warning(this,tr("error checking record count"),"خطاء في الحصول علي رقم الفاتورة رجاء الاتصال بخدمة العملاء"+qry.lastError().text(),QMessageBox::Ok);
              qDebug() << "Error inserting into the table:\n" << qry.lastError();
        }
        if(recCount == 0){
			QSqlQuery qry(db);
                qry.prepare("INSERT INTO expenses(rec_no,total,old_expense,flow,branch_id,created_at)VALUES(:rec_no,:total,:old_expense,:flow,1,:created_at)");
				qry.bindValue(":rec_no",rec_no);
				qry.bindValue(":total",total);
                qry.bindValue(":old_expense",expense_age);
				qry.bindValue(":flow",flow);
                qry.bindValue(":created_at",create_date);

				if( qry.exec() ){
				}
				else{
					  QMessageBox::warning(this,tr("error checking record count"),"خظاء في ادخال البيان لقاعدة البيانات  error insert supplier_payment"+qry.lastError().text(),QMessageBox::Ok);
                      qDebug() << qry.lastError();
				}
                    /*if(ui->credit->isChecked()){
								 if(ui->out->isChecked()){
							QSqlQuery qry1(db);
							qry1.prepare("UPDATE supplier SET balance = balance - :amount WHERE id = :supplier_id");
                            //qry1.bindValue(":supplier_id",supplier_id);
                            qry1.bindValue(":amount",total);
							if( qry1.exec() ){
							}
							else{
								  QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
								  qDebug() << "Error inserting into the table:\n" << qry1.lastQuery();
							}					

						}else if (ui->in->isChecked()) {
							QSqlQuery qry1(db);
							qry1.prepare("UPDATE supplier SET balance = balance + :amount WHERE id = :supplier_id");
                            //qry1.bindValue(":supplier_id",supplier_id);
                            qry1.bindValue(":amount",total);
							if( qry1.exec() ){
							}
							else{
								  QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
								  qDebug() << "Error inserting into the table:\n" << qry1.lastError();
							}					
						}						
						
                    }*/
			
			int x  = 0 ;
                while(x<rowCount-1){
					QComboBox *myCB = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(x,0));
					QString amount = ui->tableWidget->item(x,1)->text();	
					QComboBox *myCB1 = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(x,2));	
					QComboBox *myCB2 = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(x,3));	
					QComboBox *myCB3 = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(x,4));
					QString exp_item = myCB->currentText();
					QString exp_details = myCB1->currentText();
                    QString cash_deposit_id =model->index(myCB2->currentIndex(),1).data().toString();
                    QString cashier_id = model1->index(myCB3->currentIndex(),1).data().toString();
					QSqlQuery qry(db);
                    qry.prepare("INSERT INTO expenses_details(expense_item,amount,cash_deposit_id,cashier_id,expense_id,expense_details)VALUES(:expense_item,:amount,:cash_deposit_id,:cashier_id,:expense_id,:expense_details)");
					qry.bindValue(":expense_item",exp_item);
					qry.bindValue(":amount",amount);
					qry.bindValue(":cash_deposit_id",cash_deposit_id);
                    qry.bindValue(":cashier_id",cashier_id);
                    qry.bindValue(":expense_id",rec_no);
					qry.bindValue(":expense_details",exp_details);
					if( qry.exec() ){
					}
					else{
                          QMessageBox::warning(this,tr("error checking record count"),"خظاء في ادخال البيان لقاعدة البيانات  error insert expenses"+qry.lastError().text(),QMessageBox::Ok);
						  qDebug() << "Error inserting into the table:\n" << qry.lastError();
					}	
						x++;
				}		
		}
	
	
	
	

}

void expenses::save_print()
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
    if(ui->tableWidget->isVisible()&&ui->tableWidget->rowCount()!=0){
    QtRPT *report = new QtRPT(this);
    report->loadReport(":/Rc/Xml/rec1.xml");
    report->recordCount.append(ui->tableWidget->rowCount()-1);



       connect(report, &QtRPT::setValue, [&](const int recNo,
                   const QString paramName, QVariant &paramValue,
                   const int reportPage) {
                   (void) reportPage;
           QComboBox *myCB = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(recNo,0));
           QComboBox *myCB3 = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(recNo,4));
           QString exp_item = myCB->currentText();
           QString cashier_name = myCB3->currentText();

               if (paramName == "recNo") {
                   paramValue = ui->rec_no->text();
               }
               if (paramName == "cashier") {
                   paramValue = cashier_name ;
               }
               if (paramName == "exp_item") {
                   paramValue = exp_item ;
               }
               if (paramName == "amount") {
                   paramValue = ui->tableWidget->item(recNo,1)->text();
               }
               if (paramName == "total") {
                   paramValue = ui->total_after->text() ;
               }


       });

    report->printExec(true,true,printer_name);}
}

void expenses::update_time()
{
    if (saved == false) {
       ui->dateTimeEdit->setTime( QTime::currentTime() );
    }
}

void expenses::on_tableWidget_cellChanged(int row, int column)
{
    if(row == ui->tableWidget->rowCount()-1){
        QComboBox *box;
        QComboBox *box1;
        QComboBox *box2;
        QComboBox *box3;
        box = new QComboBox();
        box->setEditable(true);
        box1 = new QComboBox();
        box1->setEditable(true);
        box2 = new QComboBox();
        box2->setEditable(true);
        box3 = new QComboBox();
        box3->setEditable(true);
        QSqlQuery qry(db);
        QStringList inlist;
        qry.prepare("SELECT DISTINCT expense_item ,expense_details FROM expenses_details");
        if (qry.exec())
        {
            while (qry.next())
            {

                box->addItem(qry.value(0).toString());
                box1->addItem(qry.value(1).toString());
            }
        }
        else{
            QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
              qDebug() << "Error getting shit:\n" << qry.lastError();
       }
        box2->setModel(model);
        box3->setModel(model1);
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,0,box);
        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,2,box1);
        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,3,box2);
        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,4,box3);
        box->clearEditText();
        box1->clearEditText();
        box2->clearEditText();
        box3->clearEditText();

        int column = 1;
        double total = 0 ;
        for(int row = 0; row < ui->tableWidget->rowCount()-1; row++){
            total +=  ui->tableWidget->item(row,column)->text().toDouble();
        }
           ui->total_after->setText(QString::number(total));

    }


}

void expenses::on_save_clicked()
{
  save();
  ClearAll();
}

void expenses::on_pushButton_4_clicked()
{
    ClearAll();
}

void expenses::on_pushButton_5_clicked()
{
 save_print();
 save();
 ClearAll();
}


void expenses::on_saved_clicked()
{
    saved = true;
    set_unedit();
    QSqlQuery qry(db);
    QSqlQuery qry2(db);
    QString rec_no;
    QDateTime date;
    QString expense_age;
    QString flow;
    qry.prepare("SELECT MAX(rec_no) from expenses");
    if (qry.exec())
    {
        while (qry.next())
        {
            rec_no = QString::number(qry.value(0).toInt());

        }
    }
    QSqlTableModel *GetTable = new QSqlTableModel(this,db);
    GetTable->setTable("expenses_details");
    GetTable->setFilter("expense_id = "+rec_no+" ");
    ui->tableWidget->hide();
    ui->tableView->show();
    ui->tableView->setModel(GetTable);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);
    GetTable->select();

    ui->rec_no->setText(rec_no);
    qry2.prepare("SELECT created_at,old_expense,total,flow from expenses WHERE rec_no = "+rec_no+" ");
    if (qry2.exec())
    {
        while (qry2.next())
        {
            date = qry2.value(0).toDateTime();
            expense_age = qry2.value(1).toString();
            ui->total_after->setText(qry2.value(2).toString());
            flow = qry2.value(3).toString();
        }
    }
    else{qDebug() << "Error selectttt1 into the table:\n" << qry2.lastError();}
    ui->dateTimeEdit->setDateTime(date);
    if(expense_age == "0"){
        ui->new_expense->setChecked(true);

    }
    else if(expense_age == "1"){
        ui->old_expense->setChecked(true);
    }


    if(flow == "0"){

        ui->out->setChecked(true);
    }
    else if (flow == "1"){
        ui->in->setChecked(true);

    }
}

void expenses::on_delete_rec_clicked()
{
     if(saved==true){
    if (QMessageBox::question(this, tr("delete?!!!"),
        tr("Are You Sure You WANT TO delete!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {

            QString rec_no = ui->rec_no->text();
            QSqlQuery qry(db);
            qry.prepare("DELETE FROM expenses WHERE rec_no = :rec_no");
            qry.bindValue(":rec_no",rec_no);
            if(!qry.exec()){
              qDebug() << "Error inserting into the table:\n" << qry.lastError();
            }
            QSqlQuery qry1(db);
            qry1.prepare("DELETE FROM expenses_details WHERE expense_id = :rec_no");
            qry1.bindValue(":rec_no",rec_no);
            if(!qry1.exec()){
              qDebug() << "Error inserting into the table:\n" << qry1.lastError();
            }
            ClearAll();


            /*//QString supplier_id = model2->index(ui->supplier_name->currentIndex(),1).data().toString();
            double amount =ui->total_after->text().toDouble();
            if(ui->credit->isChecked()){
                    if(ui->out->isChecked()){
                        QSqlQuery qry1(db);
                        qry1.prepare("UPDATE supplier SET balance = balance + :amount WHERE id = :supplier_id");
                        qry1.bindValue(":supplier_id",supplier_id);
                        qry1.bindValue(":amount",amount);
                        if( qry1.exec() ){
                            QSqlQuery qry(db);
                            qry.prepare("DELETE FROM expenses WHERE rec_no = :rec_no");
                            qry.bindValue(":rec_no",rec_no);
                            if(!qry.exec()){
                              qDebug() << "Error inserting into the table:\n" << qry.lastError();
                            }
                            QSqlQuery qry1(db);
                            qry1.prepare("DELETE FROM expenses_details WHERE expense_id = :rec_no");
                            qry1.bindValue(":rec_no",rec_no);
                            if(!qry1.exec()){
                              qDebug() << "Error inserting into the table:\n" << qry1.lastError();
                            }

                        }
                        else{
                              QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                              qDebug() << "Error inserting into the table:\n" << qry1.lastQuery();
                        }
                    }else if (ui->in->isChecked()) {
                    QSqlQuery qry1(db);
                    qry1.prepare("UPDATE supplier SET balance = balance - :amount WHERE id = :supplier_id");
                    qry1.bindValue(":supplier_id",supplier_id);
                    qry1.bindValue(":amount",amount);
                    if( qry1.exec() ){
                        QSqlQuery qry(db);
                        qry.prepare("DELETE FROM expenses WHERE rec_no = :rec_no");
                        qry.bindValue(":rec_no",rec_no);
                        if(!qry.exec()){
                          qDebug() << "Error inserting into the table:\n" << qry.lastError();
                        }
                        QSqlQuery qry1(db);
                        qry1.prepare("DELETE FROM expenses_details WHERE expense_id = :rec_no");
                        qry1.bindValue(":rec_no",rec_no);
                        if(!qry1.exec()){
                            qDebug() << "Error inserting into the table:\n" << qry1.lastError();
                        }


                    }
                    else{
                          QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                          qDebug() << "Error inserting into the table:\n" << qry1.lastError();
                        }
                    }
                    ClearAll();

            }else if (ui->cash->isChecked()) {

            }*/
        }
    }
}

void expenses::on_rec_no_textChanged(const QString &arg1)
{
    if(saved == true){
        set_unedit();
        QSqlQuery qry(db);
        QSqlQuery qry2(db);
        QDateTime date;
        QString expense_age;
        QString flow;

        QSqlTableModel *GetTable = new QSqlTableModel(this,db);
        GetTable->setTable("expenses_details");
        GetTable->setFilter("expense_id = "+ui->rec_no->text()+" ");
        ui->tableWidget->hide();
        ui->tableView->show();
        ui->tableView->setModel(GetTable);
        ui->tableView->hideColumn(0);
        ui->tableView->hideColumn(1);
        ui->tableView->hideColumn(5);
        ui->tableView->hideColumn(6);
        GetTable->select();
        qry2.prepare("SELECT created_at,old_expense,total,flow from expenses WHERE rec_no = "+ui->rec_no->text()+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                date = qry2.value(0).toDateTime();
                expense_age = qry2.value(1).toString();
                ui->total_after->setText(qry2.value(2).toString());
                flow = qry2.value(3).toString();
            }
        }
        else{qDebug() << "Error selectttt1 into the table:\n" << qry2.lastError();}
        ui->dateTimeEdit->setDateTime(date);
        if(expense_age == "0"){
            ui->new_expense->setChecked(true);

        }
        else if(expense_age == "1"){
            ui->old_expense->setChecked(true);
        }


        if(flow == "0"){

            ui->out->setChecked(true);
        }
        else if (flow == "1"){
            ui->in->setChecked(true);

        }

    }
}

void expenses::on_next_rec_clicked()
{
    if(saved == true){
        QSqlQuery qry(db);
        int test7;
        qry.prepare("SELECT MAX(rec_no) from expenses");
        if (qry.exec())
        {
            while (qry.next())
            {
                test7 = qry.value(0).toInt();
            }
        }
        qDebug()<<test7<<saved;

        if(saved == true&&ui->rec_no->text().toInt()<test7){
            ui->rec_no->setText(QString::number(ui->rec_no->text().toInt()+1));
        }

    }
}

void expenses::on_back_rec_clicked()
{
    if(saved == true){
        QSqlQuery qry(db);
        int test7;
        qry.prepare("SELECT MIN(rec_no) from expenses");
        if (qry.exec())
        {
            while (qry.next())
            {
                test7 = qry.value(0).toInt();
            }
        }
        qDebug()<<test7<<saved;

        if(saved == true&&ui->rec_no->text().toInt()>test7){
            ui->rec_no->setText(QString::number(ui->rec_no->text().toInt()-1));
        }

    }
}
