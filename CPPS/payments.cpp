#include "payments.h"
#include "ui_payments.h"

payments::payments(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::payments)
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

    ui->cash_depo_name->setModel(model);
    ui->branch_name->setModel(model1);
    ui->supp_name->setModel(model2);
    ui->customer_name->setModel(model3);
    ui->cashier_name->setModel(model4);
    QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));
        timer->start(1000);

    ClearAll();


}

payments::~payments()
{
    delete ui;
}
void payments::update_List()
{
    int sup_ind = ui->supp_name->currentIndex();
    int cust_ind = ui->customer_name->currentIndex();

    model2->setQuery("SELECT name ,id FROM supplier",db);
    model3->setQuery("SELECT name ,id FROM customers",db);
    ui->supp_name->setCurrentIndex(sup_ind);
    ui->customer_name->setCurrentIndex(cust_ind);


}
void payments::ClearAll()
{
   saved = false;
   ui->cash_depo_name->lineEdit()->clear();
   ui->branch_name->lineEdit()->clear();
   ui->supp_name->lineEdit()->clear();
   ui->customer_name->lineEdit()->clear();
   ui->cashier_name->lineEdit()->clear();
   ui->notes->clear();
   ui->lineEdit->clear();
   ui->lineEdit->setText("0");
   ui->tableWidget->setRowCount(0);
   ui->cash->setChecked(true);
   ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());



   if(ui->rec_stat->currentIndex()==0){

       ui->out->setChecked(true);

   }else if (ui->rec_stat->currentIndex()==1) {
       ui->in->setChecked(true);
   }


   ui->amount->clear();
   db.close();
   db.open();
   if(ui->rec_stat->currentIndex()==0){
       QSqlQuery qry(db);
       qry.prepare("SELECT MAX(rec_no) from supplier_payment");
       if (qry.exec())
       {
           while (qry.next())
           {
               ui->rec_no->setText(QString::number(qry.value(0).toInt()+1));

           }
       }
     }
      else if (ui->rec_stat->currentIndex()==1) {

        QSqlQuery qry(db);
        qry.prepare("SELECT MAX(rec_no) from customer_payment");
        if (qry.exec())
         {
           while (qry.next())
           {
           ui->rec_no->setText(QString::number(qry.value(0).toInt()+1));

           }
         }
      }
   model->setQuery("SELECT name ,id FROM cash_deposit",db);
   model1->setQuery("SELECT name ,id FROM branch",db);
   model2->setQuery("SELECT name ,id FROM supplier",db);
   model3->setQuery("SELECT name ,id FROM customers",db);
   model4->setQuery("SELECT name ,id FROM employees WHERE appear_as_cashier = 1",db);
   if(ui->rec_stat->currentIndex()==0){
       ui->supplier_label->show();
       ui->supp_name->show();
       ui->customer_name->hide();
       ui->customer_label->hide();
       ui->supp_name->setCurrentIndex(1);
       ui->supp_name->setCurrentIndex(0);

   }else if (ui->rec_stat->currentIndex()==1) {
       ui->supplier_label->hide();
       ui->supp_name->hide();
       ui->customer_name->show();
       ui->customer_label->show();
       ui->customer_name->setCurrentIndex(1);
       ui->customer_name->setCurrentIndex(0);
   }
   ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   ui->cash_depo_name->setCurrentIndex(0);
   ui->branch_name->setCurrentIndex(0);
   ui->cashier_name->setCurrentIndex(0);
   set_edit();
}

void payments::set_edit()
{
    ui->cash_depo_name->lineEdit()->setReadOnly(false);
    ui->branch_name->lineEdit()->setReadOnly(false);
    ui->supp_name->lineEdit()->setReadOnly(false);
    ui->customer_name->lineEdit()->setReadOnly(false);
    ui->cashier_name->lineEdit()->setReadOnly(false);
    ui->notes->setReadOnly(false);
    ui->amount->setReadOnly(false);

}

void payments::set_unedit()
{
    ui->cash_depo_name->lineEdit()->setReadOnly(true);
    ui->branch_name->lineEdit()->setReadOnly(true);
    ui->supp_name->lineEdit()->setReadOnly(true);
    ui->customer_name->lineEdit()->setReadOnly(true);
    ui->cashier_name->lineEdit()->setReadOnly(true);
    ui->notes->setReadOnly(true);
    ui->amount->setReadOnly(true);

}

void payments::save()
{
    QString rec_no = ui->rec_no->text();

    QString branch_id = model1->index(ui->branch_name->currentIndex(),1).data().toString();
    QString cashier_id = model4->index(ui->cashier_name->currentIndex(),1).data().toString();
    QString cash_depo_id = model->index(ui->cash_depo_name->currentIndex(),1).data().toString();
    QString supplier_id = model2->index(ui->supp_name->currentIndex(),1).data().toString();
    QString customer_id = model3->index(ui->customer_name->currentIndex(),1).data().toString();
    QString notes = ui->notes->toPlainText();
    double amount = ui->amount->text().toDouble();
    double taxes_dis = ui->lineEdit->text().toDouble();
    int payment_method;
    int flow;
    QDate create_date = ui->dateTimeEdit->date();


    if(ui->cash->isChecked()){
        payment_method = 0;
    }else if (ui->fin_paper->isChecked()) {
        payment_method = 1;
    }else if (ui->mixed->isChecked()) {
        payment_method = 2;
    }

    if(ui->out->isChecked()){
        flow =0;
    }else if (ui->in->isChecked()) {
        flow = 1;
    }

    if(ui->rec_stat->currentIndex() == 0)
    {
        int recCount = 0;
        QSqlQuery qry(db);
        qry.prepare("SELECT * FROM supplier_payment WHERE rec_no = "+rec_no+"");
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

            if(ui->cash->isChecked()){
				QSqlQuery qry(db);
                qry.prepare("INSERT INTO supplier_payment(rec_no,amount,notes,payment_method,flow,taxes_discount,cashier_id,currency_id,cash_deposit_id,branch_id,supplier_id,created_at)VALUES(:rec_no,:amount,:notes,:payment_method,:flow,:taxes_discount,:cashier_id,1,:cash_deposit_id,:branch_id,:supplier_id,:create_date)");
				qry.bindValue(":rec_no",rec_no);
				qry.bindValue(":amount",amount);
				qry.bindValue(":notes",notes);
				qry.bindValue(":payment_method",payment_method);
				qry.bindValue(":flow",flow);
				qry.bindValue(":taxes_discount",taxes_dis);
				qry.bindValue(":cashier_id",cashier_id);
				qry.bindValue(":cash_deposit_id",cash_depo_id);
				qry.bindValue(":branch_id",branch_id);
				qry.bindValue(":supplier_id",supplier_id);
                qry.bindValue(":create_date",create_date);

				if( qry.exec() ){
				}
				else{
					  QMessageBox::warning(this,tr("error checking record count"),"خظاء في ادخال البيان لقاعدة البيانات  error insert supplier_payment"+qry.lastError().text(),QMessageBox::Ok);
					  qDebug() << "Error inserting into the table:\n" << qry.lastError();
				}				
                if(ui->out->isChecked()){
					QSqlQuery qry1(db);
					qry1.prepare("UPDATE supplier SET balance = balance - :amount WHERE id = :supplier_id");
					qry1.bindValue(":supplier_id",supplier_id);
					qry1.bindValue(":amount",amount);
                    if( qry1.exec() ){
					}
					else{
                          QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                          qDebug() << "Error inserting into the table:\n" << qry1.lastQuery();
					}					

                }else if (ui->in->isChecked()) {
					QSqlQuery qry1(db);
					qry1.prepare("UPDATE supplier SET balance = balance + :amount WHERE id = :supplier_id");
					qry1.bindValue(":supplier_id",supplier_id);
					qry1.bindValue(":amount",amount);
                    if( qry1.exec() ){
					}
					else{
                          QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                          qDebug() << "Error inserting into the table:\n" << qry1.lastError();
					}					
                }


            }else if (ui->fin_paper->isChecked()) {
					//add payment deatails plz
					QSqlQuery qry(db);
                    qry.prepare("INSERT INTO supplier_payment(rec_no,amount,notes,payment_method,flow,taxes_discount,cashier_id,currency_id,cash_deposit_id,branch_id,supplier_id,created_at)VALUES(:rec_no,:amount,:notes,:payment_method,:flow,:taxes_discount,:cashier_id,1,:cash_deposit_id,:branch_id,:supplier_id,:create_date)");
					qry.bindValue(":rec_no",rec_no);
					qry.bindValue(":amount",amount);
					qry.bindValue(":notes",notes);
					qry.bindValue(":payment_method",payment_method);
					qry.bindValue(":flow",flow);
					qry.bindValue(":taxes_discount",taxes_dis);
					qry.bindValue(":cashier_id",cashier_id);
					qry.bindValue(":cash_deposit_id",cash_depo_id);
					qry.bindValue(":branch_id",branch_id);
					qry.bindValue(":supplier_id",supplier_id);
                    qry.bindValue(":create_date",create_date);

					if( qry.exec() ){
					}
					else{
						  QMessageBox::warning(this,tr("error checking record count"),"خظاء في ادخال البيان لقاعدة البيانات  error insert supplier_payment"+qry.lastError().text(),QMessageBox::Ok);
						  qDebug() << "Error inserting into the table:\n" << qry.lastError();
					}
            }else if (ui->mixed->isChecked()) {
					//add payment deatails plz				
					QSqlQuery qry(db);
                    qry.prepare("INSERT INTO supplier_payment(rec_no,amount,notes,payment_method,flow,taxes_discount,cashier_id,currency_id,cash_deposit_id,branch_id,supplier_id,created_at)VALUES(:rec_no,:amount,:notes,:payment_method,:flow,:taxes_discount,:cashier_id,1,:cash_deposit_id,:branch_id,:supplier_id,:create_date)");
					qry.bindValue(":rec_no",rec_no);
					qry.bindValue(":amount",amount);
					qry.bindValue(":notes",notes);
					qry.bindValue(":payment_method",payment_method);
					qry.bindValue(":flow",flow);
					qry.bindValue(":taxes_discount",taxes_dis);
					qry.bindValue(":cashier_id",cashier_id);
					qry.bindValue(":cash_deposit_id",cash_depo_id);
					qry.bindValue(":branch_id",branch_id);
					qry.bindValue(":supplier_id",supplier_id);
                    qry.bindValue(":create_date",create_date);

					if( qry.exec() ){
					}
					else{
						  QMessageBox::warning(this,tr("error checking record count"),"خظاء في ادخال البيان لقاعدة البيانات  error insert supplier_payment"+qry.lastError().text(),QMessageBox::Ok);
						  qDebug() << "Error inserting into the table:\n" << qry.lastError();
					}
        }
    }else {
			QSqlQuery qry(db);
			qry.prepare("UPDATE supplier_payment SET amount=:amount ,notes=:notes ,payment_method=:payment_method ,flow=:flow ,taxes_discount=:taxes_discount ,cashier_id=:cashier_id ,cash_deposit_id=:cash_deposit_id ,branch_id=:branch_id ,supplier_id=:supplier_id WHERE rec_no = :rec_no ");
			qry.bindValue(":rec_no",rec_no);
			qry.bindValue(":amount",amount);
			qry.bindValue(":notes",notes);
			qry.bindValue(":payment_method",payment_method);
			qry.bindValue(":flow",flow);
			qry.bindValue(":taxes_discount",taxes_dis);
			qry.bindValue(":cashier_id",cashier_id);
			qry.bindValue(":cash_deposit_id",cash_depo_id);
			qry.bindValue(":branch_id",branch_id);
			qry.bindValue(":supplier_id",supplier_id);
			if( qry.exec() ){	
			}
			else{
				  QMessageBox::warning(this,tr("error checking record count"),"خظاء في تحديث البيان لقاعدة البيانات  error update supplier_payment"+qry.lastError().text(),QMessageBox::Ok);
                  qDebug() << "Error inserting into the table:\n" << qry.lastError();
			}
			if(ui->cash->isChecked()){
				if(ui->out->isChecked()){
					QSqlQuery qry1(db);
					qry1.prepare("UPDATE supplier SET balance = balance - :amount WHERE id = :supplier_id");
					qry1.bindValue(":supplier_id",supplier_id);
					qry1.bindValue(":amount",amount);
                    if( qry1.exec() ){
					}
					else{
                          QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                          qDebug() << "Error inserting into the table:\n" << qry1.lastError();
					}					
				}
				else if (ui->in->isChecked()){
					QSqlQuery qry1(db);
					qry1.prepare("UPDATE supplier SET balance = balance + :amount WHERE id = :supplier_id");
					qry1.bindValue(":supplier_id",supplier_id);
					qry1.bindValue(":amount",amount);
                    if( qry1.exec() ){
					}
					else{
                          QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                          qDebug() << "Error inserting into the table:\n" << qry1.lastError();
					}										
				}		
			}
			
			
			
			
			
        }
        ClearAll();

    }else if (ui->rec_stat->currentIndex() == 1) {
        int recCount = 0;
        QSqlQuery qry(db);
        qry.prepare("SELECT * FROM customer_payment WHERE rec_no = "+rec_no+"");
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
            if(ui->cash->isChecked()){
				QSqlQuery qry(db);
                qry.prepare("INSERT INTO customer_payment(rec_no,amount,notes,payment_method,flow,taxes_discount,cashier_id,currency_id,cash_deposit_id,branch_id,customer_id,created_at)VALUES(:rec_no,:amount,:notes,:payment_method,:flow,:taxes_discount,:cashier_id,1,:cash_deposit_id,:branch_id,:customer_id,:create_date)");
				qry.bindValue(":rec_no",rec_no);
				qry.bindValue(":amount",amount);
				qry.bindValue(":notes",notes);
				qry.bindValue(":payment_method",payment_method);
				qry.bindValue(":flow",flow);
				qry.bindValue(":taxes_discount",taxes_dis);
				qry.bindValue(":cashier_id",cashier_id);
				qry.bindValue(":cash_deposit_id",cash_depo_id);
				qry.bindValue(":branch_id",branch_id);
				qry.bindValue(":customer_id",customer_id);
                qry.bindValue(":create_date",create_date);

				if( qry.exec() ){
				}
				else{
					QMessageBox::warning(this,tr("error checking record count"),"خظاء في ادخال البيان لقاعدة البيانات  error insert supplier_payment"+qry.lastError().text(),QMessageBox::Ok);
					qDebug() << "Error inserting into the table:\n" << qry.lastError();
					}				
                if(ui->out->isChecked()){

					QSqlQuery qry1(db);
					qry1.prepare("UPDATE customers SET balance = balance + :amount WHERE id = :customer_id");
					qry1.bindValue(":customer_id",customer_id);
					qry1.bindValue(":amount",amount);
                    if( qry1.exec() ){
					}
					else{
                          QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                          qDebug() << "Error inserting into the table:\n" << qry1.lastError();
					}					

                }else if (ui->in->isChecked()) {
					QSqlQuery qry1(db);
					qry1.prepare("UPDATE customers SET balance = balance - :amount WHERE id = :customer_id");
					qry1.bindValue(":customer_id",customer_id);
					qry1.bindValue(":amount",amount);
                    if( qry1.exec() ){
					}
					else{
                          QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                          qDebug() << "Error inserting into the table:\n" << qry1.lastError();
					}					
                }
            }else if (ui->fin_paper->isChecked()) {
					//add payment deatails plz
					QSqlQuery qry(db);
                    qry.prepare("INSERT INTO customer_payment(rec_no,amount,notes,payment_method,flow,taxes_discount,cashier_id,currency_id,cash_deposit_id,branch_id,customer_id,created_at)VALUES(:rec_no,:amount,:notes,:payment_method,:flow,:taxes_discount,:cashier_id,1,:cash_deposit_id,:branch_id,:customer_id,:create_date)");
					qry.bindValue(":rec_no",rec_no);
					qry.bindValue(":amount",amount);
					qry.bindValue(":notes",notes);
					qry.bindValue(":payment_method",payment_method);
					qry.bindValue(":flow",flow);
					qry.bindValue(":taxes_discount",taxes_dis);
					qry.bindValue(":cashier_id",cashier_id);
					qry.bindValue(":cash_deposit_id",cash_depo_id);
					qry.bindValue(":branch_id",branch_id);
					qry.bindValue(":customer_id",customer_id);
                    qry.bindValue(":create_date",create_date);

					if( qry.exec() ){
					}
					else{
						  QMessageBox::warning(this,tr("error checking record count"),"خظاء في ادخال البيان لقاعدة البيانات  error insert supplier_payment"+qry.lastError().text(),QMessageBox::Ok);
						  qDebug() << "Error inserting into the table:\n" << qry.lastError();
					}
            }else if (ui->mixed->isChecked()) {
					//add payment deatails plz				
					QSqlQuery qry(db);
                    qry.prepare("INSERT INTO customer_payment(rec_no,amount,notes,payment_method,flow,taxes_discount,cashier_id,currency_id,cash_deposit_id,branch_id,customer_id,created_at)VALUES(:rec_no,:amount,:notes,:payment_method,:flow,:taxes_discount,:cashier_id,1,:cash_deposit_id,:branch_id,:customer_id,:create_date)");
					qry.bindValue(":rec_no",rec_no);
					qry.bindValue(":amount",amount);
					qry.bindValue(":notes",notes);
					qry.bindValue(":payment_method",payment_method);
					qry.bindValue(":flow",flow);
					qry.bindValue(":taxes_discount",taxes_dis);
					qry.bindValue(":cashier_id",cashier_id);
					qry.bindValue(":cash_deposit_id",cash_depo_id);
					qry.bindValue(":branch_id",branch_id);
					qry.bindValue(":customer_id",customer_id);
                    qry.bindValue(":create_date",create_date);

					if( qry.exec() ){
					}
					else{
						  QMessageBox::warning(this,tr("error checking record count"),"خظاء في ادخال البيان لقاعدة البيانات  error insert supplier_payment"+qry.lastError().text(),QMessageBox::Ok);
						  qDebug() << "Error inserting into the table:\n" << qry.lastError();
					}
            }
        }else {
			QSqlQuery qry(db);
			qry.prepare("UPDATE customer_payment SET amount=:amount ,notes=:notes ,payment_method=:payment_method ,flow=:flow ,taxes_discount=:taxes_discount ,cashier_id=:cashier_id ,cash_deposit_id=:cash_deposit_id ,branch_id=:branch_id ,customer_id=:customer_id WHERE rec_no = :rec_no ");
			qry.bindValue(":rec_no",rec_no);
			qry.bindValue(":amount",amount);
			qry.bindValue(":notes",notes);
			qry.bindValue(":payment_method",payment_method);
			qry.bindValue(":flow",flow);
			qry.bindValue(":taxes_discount",taxes_dis);
			qry.bindValue(":cashier_id",cashier_id);
			qry.bindValue(":cash_deposit_id",cash_depo_id);
			qry.bindValue(":branch_id",branch_id);
			qry.bindValue(":customer_id",customer_id);
			if( qry.exec() ){	
			}
			else{
				  QMessageBox::warning(this,tr("error checking record count"),"خظاء في تحديث البيان لقاعدة البيانات  error update supplier_payment"+qry.lastError().text(),QMessageBox::Ok);
				  qDebug() << "Error inserting into the table:\n" << qry.lastError();
			}
			if(ui->cash->isChecked()){
				if(ui->out->isChecked()){
					QSqlQuery qry1(db);
					qry1.prepare("UPDATE customers SET balance = balance + :amount WHERE id = :customer_id");
					qry1.bindValue(":customer_id",customer_id);
					qry1.bindValue(":amount",amount);
                    if( qry1.exec() ){
					}
					else{
                          QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                          qDebug() << "Error inserting into the table:\n" << qry1.lastError();
					}					
				
				}else if (ui->in->isChecked()) {
					QSqlQuery qry1(db);
					qry1.prepare("UPDATE customers SET balance = balance - :amount WHERE id = :customer_id");
					qry1.bindValue(":customer_id",customer_id);
					qry1.bindValue(":amount",amount);
                    if( qry1.exec() ){
					}
					else{
                          QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                          qDebug() << "Error inserting into the table:\n" << qry1.lastError();
					}									
				}		
			}
			
			
			
        }

        ClearAll();
    }
}
void payments::on_rec_stat_currentIndexChanged(int index)
{
    if(index == 0){
        if(saved == false){
        db.close();
        db.open();
        QSqlQuery qry(db);
        qry.prepare("SELECT MAX(rec_no) from supplier_payment");
        if (qry.exec())
        {
            while (qry.next())
            {
                ui->rec_no->setText(QString::number(qry.value(0).toInt()+1));

            }
        }
        ui->supp_name->show();
        ui->supplier_label->show();
        ui->customer_name->hide();
        ui->customer_label->hide();
        ui->balance->clear();
        ui->supp_name->setCurrentIndex(1);
        ui->supp_name->setCurrentIndex(0);
        ui->out->setChecked(true);
        }else if (saved == true) {
            db.close();
            db.open();
            QSqlQuery qry(db);
            qry.prepare("SELECT MAX(rec_no) from supplier_payment");
            if (qry.exec())
            {
                while (qry.next())
                {
                    ui->rec_no->setText(QString::number(qry.value(0).toInt()));

                }
            }
            ui->supp_name->show();
            ui->supplier_label->show();
            ui->customer_name->hide();
            ui->customer_label->hide();
            ui->balance->clear();
            ui->supp_name->setCurrentIndex(1);
            ui->supp_name->setCurrentIndex(0);
        }



    }else if (index == 1) {
        if(saved == false){
        db.close();
        db.open();
        QSqlQuery qry(db);
        qry.prepare("SELECT MAX(rec_no) from customer_payment");
        if (qry.exec())
         {
           while (qry.next())
           {
           ui->rec_no->setText(QString::number(qry.value(0).toInt()+1));

           }
         }
        ui->supp_name->hide();
        ui->supplier_label->hide();
        ui->customer_name->show();
        ui->customer_label->show();
        ui->balance->clear();
        ui->customer_name->setCurrentIndex(1);
        ui->customer_name->setCurrentIndex(0);
        ui->in->setChecked(true);

        }else if (saved == true) {
            db.close();
            db.open();
            QSqlQuery qry(db);
            qry.prepare("SELECT MAX(rec_no) from customer_payment");
            if (qry.exec())
             {
               while (qry.next())
               {
               ui->rec_no->setText(QString::number(qry.value(0).toInt()));

               }
             }
            ui->supp_name->hide();
            ui->supplier_label->hide();
            ui->customer_name->show();
            ui->customer_label->show();
            ui->balance->clear();
            ui->customer_name->setCurrentIndex(1);
            ui->customer_name->setCurrentIndex(0);

        }

    }
}

void payments::on_saved_clicked()
{
  saved = true;
  set_unedit();
  if(ui->rec_stat->currentIndex()==0){
  QSqlQuery qry(db);
  QSqlQuery qry2(db);
  QString rec_no;
  qry.prepare("SELECT MAX(rec_no) from supplier_payment");
  if (qry.exec())
  {
      while (qry.next())
      {
          rec_no = QString::number(qry.value(0).toInt());

      }
  }
  else{
        QMessageBox::warning(this,tr("error checking record count"),"خطاء في الحصول علي رقم الفاتورة رجاء الاتصال بخدمة العملاء"+qry.lastError().text(),QMessageBox::Ok);
        qDebug() << "Error inserting into the table:\n" << qry.lastError();
  }
  ui->rec_no->setText(rec_no);

  QString payment;
  QString flow;
  qry2.prepare("SELECT created_at,payment_method,supplier_id,amount,taxes_discount,branch_id,cashier_id,cash_deposit_id,flow,notes from supplier_payment WHERE rec_no = "+rec_no+" ");
  if (qry2.exec())
  {
      while (qry2.next())
      {
          ui->dateTimeEdit->setDateTime(qry2.value(0).toDateTime());
          payment = qry2.value(1).toString();
          model2->setQuery("SELECT name ,id FROM supplier WHERE id = "+qry2.value(2).toString()+" ",db);
          ui->amount->setText(qry2.value(3).toString());
          ui->lineEdit->setText(qry2.value(4).toString());
          model1->setQuery("SELECT name ,id FROM branch WHERE id = "+qry2.value(5).toString()+" ",db);
          model4->setQuery("SELECT name ,id FROM employees WHERE id = "+qry2.value(6).toString()+" ",db);
          model->setQuery("SELECT name ,id FROM cash_deposit WHERE id = "+qry2.value(7).toString()+" ",db);
          flow = qry2.value(8).toString();
          ui->notes->setText(qry2.value(9).toString());
             ui->supp_name->setCurrentIndex(0);
             ui->branch_name->setCurrentIndex(0);
             ui->cashier_name->setCurrentIndex(0);
             ui->cash_depo_name->setCurrentIndex(0);

      }
  }
  else{
      QMessageBox::warning(this,tr("error getting saved record"),"خطاء في الحصول علي الفاتورة رجاء الاتصال بخدمة العملاء"+qry2.lastError().text(),QMessageBox::Ok);
      qDebug() << "Error selectttt1 into the table:\n" << qry2.lastError();
  }

  if(payment == "0"){
      ui->cash->setChecked(true);

  }
  else if(payment == "1"){
      ui->fin_paper->setChecked(true);
  }
  else if (payment == "2") {
      ui->mixed->setChecked(true);
  }

  if (flow == "0"){
        ui->out->setChecked(true);
  }
  else if(flow == "1"){
      ui->in->setChecked(true);
  }

  }
  else if (ui->rec_stat->currentIndex()==1){
      QSqlQuery qry(db);
      QSqlQuery qry2(db);
      QString rec_no;
      qry.prepare("SELECT MAX(rec_no) from customer_payment");
      if (qry.exec())
      {
          while (qry.next())
          {
              rec_no = QString::number(qry.value(0).toInt());

          }
      }
      else{
            QMessageBox::warning(this,tr("error checking record count"),"خطاء في الحصول علي رقم الفاتورة رجاء الاتصال بخدمة العملاء"+qry.lastError().text(),QMessageBox::Ok);
            qDebug() << "Error inserting into the table:\n" << qry.lastError();
      }
      ui->rec_no->setText(rec_no);

      QString payment;
      QString flow;
      qry2.prepare("SELECT created_at,payment_method,customer_id,amount,taxes_discount,branch_id,cashier_id,cash_deposit_id,flow,notes from customer_payment WHERE rec_no = "+rec_no+" ");
      if (qry2.exec())
      {
          while (qry2.next())
          {
              ui->dateTimeEdit->setDateTime(qry2.value(0).toDateTime());
              payment = qry2.value(1).toString();
              model3->setQuery("SELECT name ,id FROM customers WHERE id = "+qry2.value(2).toString()+" ",db);
              ui->amount->setText(qry2.value(3).toString());
              ui->lineEdit->setText(qry2.value(4).toString());
              model1->setQuery("SELECT name ,id FROM branch WHERE id = "+qry2.value(5).toString()+" ",db);
              model4->setQuery("SELECT name ,id FROM employees WHERE id = "+qry2.value(6).toString()+" ",db);
              model->setQuery("SELECT name ,id FROM cash_deposit WHERE id = "+qry2.value(7).toString()+" ",db);
              flow = qry2.value(8).toString();
              ui->notes->setText(qry2.value(9).toString());
                 ui->customer_name->setCurrentIndex(0);
                 ui->branch_name->setCurrentIndex(0);
                 ui->cashier_name->setCurrentIndex(0);
                 ui->cash_depo_name->setCurrentIndex(0);

          }
      }
      else{
          QMessageBox::warning(this,tr("error getting saved record"),"خطاء في الحصول علي الفاتورة رجاء الاتصال بخدمة العملاء"+qry2.lastError().text(),QMessageBox::Ok);
          qDebug() << "Error selectttt1 into the table:\n" << qry2.lastError();
      }

      if(payment == "0"){
          ui->cash->setChecked(true);

      }
      else if(payment == "1"){
          ui->fin_paper->setChecked(true);
      }
      else if (payment == "2") {
          ui->mixed->setChecked(true);
      }

      if (flow == "0"){
            ui->out->setChecked(true);
      }
      else if(flow == "1"){
          ui->in->setChecked(true);
      }
  }

}
void payments::save_print()
{

}
void payments::on_delete_rec_clicked()
{
    if (QMessageBox::question(this, tr("delete?!!!"),
        tr("Are You Sure You WANT TO delete!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
        if(saved==true){
            QString rec_no = ui->rec_no->text();
            QString supplier_id = model2->index(ui->supp_name->currentIndex(),1).data().toString();
            QString customer_id = model3->index(ui->customer_name->currentIndex(),1).data().toString();
            double amount =ui->amount->text().toDouble();
            if(ui->rec_stat->currentIndex()==0){
                if(ui->cash->isChecked()){
                    if(ui->out->isChecked()){
                        QSqlQuery qry1(db);
                        qry1.prepare("UPDATE supplier SET balance = balance + :amount WHERE id = :supplier_id");
                        qry1.bindValue(":supplier_id",supplier_id);
                        qry1.bindValue(":amount",amount);
                        if( qry1.exec() ){
                            QSqlQuery qry(db);
                            qry.prepare("DELETE FROM supplier_payment WHERE rec_no = :rec_no");
                            qry.bindValue(":rec_no",rec_no);
                            if(!qry.exec()){
                              qDebug() << "Error inserting into the table:\n" << qry.lastError();
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
                        qry.prepare("DELETE FROM supplier_payment WHERE rec_no = :rec_no");
                        qry.bindValue(":rec_no",rec_no);
                        if(!qry.exec()){
                          qDebug() << "Error inserting into the table:\n" << qry.lastError();
                        }


                    }
                    else{
                          QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                          qDebug() << "Error inserting into the table:\n" << qry1.lastError();
                        }
                    }
                }
                else if(ui->fin_paper->isChecked()){


                }
                else if(ui->mixed->isChecked()){


                }


                ClearAll();
            }
            else if(ui->rec_stat->currentIndex()==1){
                if(ui->cash->isChecked()){
                    if(ui->out->isChecked()){

                        QSqlQuery qry1(db);
                        qry1.prepare("UPDATE customers SET balance = balance - :amount WHERE id = :customer_id");
                        qry1.bindValue(":customer_id",customer_id);
                        qry1.bindValue(":amount",amount);
                        if( qry1.exec() ){
                            QSqlQuery qry(db);
                            qry.prepare("DELETE FROM customer_payment WHERE rec_no = :rec_no");
                            qry.bindValue(":rec_no",rec_no);
                            if(!qry.exec()){
                              qDebug() << "Error inserting into the table:\n" << qry.lastError();
                            }

                        }
                        else{
                              QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                              qDebug() << "Error inserting into the table:\n" << qry1.lastError();
                        }

                    }else if (ui->in->isChecked()) {
                        QSqlQuery qry1(db);
                        qry1.prepare("UPDATE customers SET balance = balance + :amount WHERE id = :customer_id");
                        qry1.bindValue(":customer_id",customer_id);
                        qry1.bindValue(":amount",amount);
                        if( qry1.exec() ){
                           QSqlQuery qry(db);
                           qry.prepare("DELETE FROM customer_payment WHERE rec_no = :rec_no");
                           qry.bindValue(":rec_no",rec_no);
                           if(!qry.exec()){
                             qDebug() << "Error inserting into the table:\n" << qry.lastError();
                           }
                        }
                        else{
                              QMessageBox::warning(this,tr("error checking record count"),"خطاء في تحديث رصيد المورد  error updating  supplier balance"+qry1.lastError().text(),QMessageBox::Ok);
                              qDebug() << "Error inserting into the table:\n" << qry1.lastError();
                        }
                    }


                }
                else if(ui->fin_paper->isChecked()){


                }
                else if(ui->mixed->isChecked()){


                }
                ClearAll();
            }
        }else {

         }
   }



}

void payments::on_next_clicked()
{
    if(ui->rec_stat->currentIndex()==0){
    QSqlQuery qry(db);
    int test7;
    qry.prepare("SELECT MAX(rec_no) from supplier_payment");
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
    else if (ui->rec_stat->currentIndex()==1){
        QSqlQuery qry(db);
        int test7;
        qry.prepare("SELECT MAX(rec_no) from customer_payment");
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

void payments::on_back_clicked()
{
    if(ui->rec_stat->currentIndex()==0){
    QSqlQuery qry(db);
    int test7;
    qry.prepare("SELECT MIN(rec_no) from supplier_payment");
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
    else if (ui->rec_stat->currentIndex()==1){
        QSqlQuery qry(db);
        int test7;
        qry.prepare("SELECT MIN(rec_no) from customer_payment");
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
void payments::update_time()
{
   if (saved == false) {
      ui->dateTimeEdit->setTime( QTime::currentTime() );
   }
}



void payments::on_customer_name_currentIndexChanged(int index)
{
    QString customer_id = model3->index(index,1).data().toString();
        QSqlQuery qry(db);
        qry.prepare("SELECT balance FROM customers WHERE id = :id");
        qry.bindValue(":id",customer_id);
        if( qry.exec() ){
            while (qry.next())
            {
                ui->balance->setText(qry.value(0).toString());

            }
        }
        else{
            QMessageBox::warning(this,tr("error checking record count"),"خطاء في الحصول علي رقم رصيد العميل رجاء الاتصال بخدمة العملاء"+qry.lastError().text(),QMessageBox::Ok);
            qDebug() << "Error inserting into the table:\n" << qry.lastError();}
}

void payments::on_supp_name_currentIndexChanged(int index)
{
    QString supplier_id = model2->index(index,1).data().toString();
        QSqlQuery qry(db);
        qry.prepare("SELECT balance FROM supplier WHERE id = :id");
        qry.bindValue(":id",supplier_id);
        if( qry.exec() ){
            while (qry.next())
            {
                ui->balance->setText(qry.value(0).toString());

            }
        }
        else{
            QMessageBox::warning(this,tr("error checking record count"),"خطاء في الحصول علي رقم رصيد المورد رجاء الاتصال بخدمة العملاء"+qry.lastError().text(),QMessageBox::Ok);
            qDebug() << "Error inserting into the table:\n" << qry.lastError();}
}

void payments::on_save_clicked()
{
    save();
}

void payments::on_rec_no_textChanged(const QString &arg1)
{
    if(ui->rec_stat->currentIndex()==0&&saved == true){
        QSqlQuery qry(db);
        QSqlQuery qry2(db);
        QString rec_no = ui->rec_no->text();
        QString payment;
        QString flow;
        qry2.prepare("SELECT created_at,payment_method,supplier_id,amount,taxes_discount,branch_id,cashier_id,cash_deposit_id,flow,notes from supplier_payment WHERE rec_no = "+rec_no+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                ui->dateTimeEdit->setDateTime(qry2.value(0).toDateTime());
                payment = qry2.value(1).toString();
                model2->setQuery("SELECT name ,id FROM supplier WHERE id = "+qry2.value(2).toString()+" ",db);
                ui->amount->setText(qry2.value(3).toString());
                ui->lineEdit->setText(qry2.value(4).toString());
                model1->setQuery("SELECT name ,id FROM branch WHERE id = "+qry2.value(5).toString()+" ",db);
                model4->setQuery("SELECT name ,id FROM employees WHERE id = "+qry2.value(6).toString()+" ",db);
                model->setQuery("SELECT name ,id FROM cash_deposit WHERE id = "+qry2.value(7).toString()+" ",db);
                flow = qry2.value(8).toString();
                ui->notes->setText(qry2.value(9).toString());
                   ui->supp_name->setCurrentIndex(0);
                   ui->branch_name->setCurrentIndex(0);
                   ui->cashier_name->setCurrentIndex(0);
                   ui->cash_depo_name->setCurrentIndex(0);

            }
        }
        else{
            QMessageBox::warning(this,tr("error getting saved record"),"خطاء في الحصول علي الفاتورة رجاء الاتصال بخدمة العملاء"+qry2.lastError().text(),QMessageBox::Ok);
            qDebug() << "Error selectttt1 into the table:\n" << qry2.lastError();
        }

        if(payment == "0"){
            ui->cash->setChecked(true);

        }
        else if(payment == "1"){
            ui->fin_paper->setChecked(true);
        }
        else if (payment == "2") {
            ui->mixed->setChecked(true);
        }

        if (flow == "0"){
              ui->out->setChecked(true);
        }
        else if(flow == "1"){
            ui->in->setChecked(true);
        }

    }else if (ui->rec_stat->currentIndex()==0&&saved == true) {
        QSqlQuery qry(db);
        QSqlQuery qry2(db);
        QString rec_no = ui->rec_no->text();
        QString payment;
        QString flow;
        qry2.prepare("SELECT created_at,payment_method,customer_id,amount,taxes_discount,branch_id,cashier_id,cash_deposit_id,flow,notes from customer_payment WHERE rec_no = "+rec_no+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                ui->dateTimeEdit->setDateTime(qry2.value(0).toDateTime());
                payment = qry2.value(1).toString();
                model3->setQuery("SELECT name ,id FROM customers WHERE id = "+qry2.value(2).toString()+" ",db);
                ui->amount->setText(qry2.value(3).toString());
                ui->lineEdit->setText(qry2.value(4).toString());
                model1->setQuery("SELECT name ,id FROM branch WHERE id = "+qry2.value(5).toString()+" ",db);
                model4->setQuery("SELECT name ,id FROM employees WHERE id = "+qry2.value(6).toString()+" ",db);
                model->setQuery("SELECT name ,id FROM cash_deposit WHERE id = "+qry2.value(7).toString()+" ",db);
                flow = qry2.value(8).toString();
                ui->notes->setText(qry2.value(9).toString());
                   ui->customer_name->setCurrentIndex(0);
                   ui->branch_name->setCurrentIndex(0);
                   ui->cashier_name->setCurrentIndex(0);
                   ui->cash_depo_name->setCurrentIndex(0);

            }
        }
        else{
            QMessageBox::warning(this,tr("error getting saved record"),"خطاء في الحصول علي الفاتورة رجاء الاتصال بخدمة العملاء"+qry2.lastError().text(),QMessageBox::Ok);
            qDebug() << "Error selectttt1 into the table:\n" << qry2.lastError();
        }

        if(payment == "0"){
            ui->cash->setChecked(true);

        }
        else if(payment == "1"){
            ui->fin_paper->setChecked(true);
        }
        else if (payment == "2") {
            ui->mixed->setChecked(true);
        }

        if (flow == "0"){
              ui->out->setChecked(true);
        }
        else if(flow == "1"){
            ui->in->setChecked(true);
        }
    }
}

void payments::on_pushButton_4_clicked()
{
    ClearAll();
}
