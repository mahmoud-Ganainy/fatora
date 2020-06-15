#include "sell_win.h"
#include "ui_sell_win.h"

sell_win::sell_win(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sell_win)
{
    ui->setupUi(this);
    //this->setStyleSheet(QString::fromUtf8("font: 12pt FreeMono;"));
    db = QSqlDatabase::database("test");
    db2 = QSqlDatabase::database("setting");
    if(!db.isOpen()){
    if(db.open()){

        ui->statusbar->showMessage("DataBase Connected!",200000);


    }
    else {
        ui->statusbar->showMessage("Error Error",200000);
    }
    }
    if(!db2.isOpen()){
    if(db2.open()){

        ui->statusbar->showMessage("DataBase Connected!",200000);


    }
    else {
        ui->statusbar->showMessage("Error Error",200000);
    }
    }


    model = new QSqlQueryModel(this);
    model1 = new QSqlQueryModel(this);
    model2 = new QSqlQueryModel(this);
    model3 = new QSqlQueryModel(this);
    model4 = new QSqlQueryModel(this);
    model5 = new QSqlQueryModel(this);
    model6 = new QSqlQueryModel(this);
    model7 = new QSqlQueryModel(this);
    model8 = new QSqlQueryModel(this);
    model10 = new QSqlQueryModel(this);
    model11 = new QSqlQueryModel(this);
    model12 = new QSqlQueryModel(this);
    model13 = new QSqlQueryModel(this);
    model14 = new QSqlQueryModel(this);
    model15 = new QSqlQueryModel(this);
    model16 = new QSqlQueryModel(this);
    completer = new QCompleter(this);
    tv = new QTableView(this);

    tv->setModel(model4);
    ui->stock_name->setModel(model2);
    ui->seller_name->setModel(model3);
    ui->cashier_name->setModel(model11);
    ui->currency_name->setModel(model13);
    ui->item_name->setModel(model4);
    ui->item_name->setView(tv);
    ui->item_int->setModel(model5);
    ui->cash_name->setModel(model10);
    ui->branch_name->setModel(model12);
    ui->custom_name->setModel(model14);
    ui->ex_bcode->setModel(model6);
    search = new item_search(this);
    inv_search = new invoice_search(this);

    connect(tv,SIGNAL(pressed(QModelIndex)),this,SLOT(setData()));
    connect(tv,SIGNAL(activated(QModelIndex)),this,SLOT(setData()));
    connect(ui->item_name->lineEdit(),SIGNAL(returnPressed()),this,SLOT(enterName()));
    connect(ui->item_int->lineEdit(),SIGNAL(returnPressed()),this,SLOT(enterInt()));
    connect(ui->ex_bcode->lineEdit(),SIGNAL(returnPressed()),this,SLOT(enterBcode()));
    connect(inv_search,SIGNAL(accepted()),this,SLOT(search_invoice()));

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_P), this, SLOT(print_save()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this, SLOT(show_search()));
    new QShortcut(QKeySequence(Qt::ALT + Qt::Key_X), this, SLOT(clearLines()));
    new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(delete_row()));
    //new QShortcut(QKeySequence(Qt::ALT + Qt::Key_L), this, SLOT(test()));
    QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));
        timer->start(1000);

}
void sell_win::showEvent(QShowEvent *event){
    QMainWindow::showEvent(event);
    if(first_open == true){
     clearAll();
     first_open = false;
    }

}
void sell_win::test(){

    qDebug()<<"hhdddddddh";
}

void sell_win::update_time()
{
        if(saved == false){
    ui->dateTimeEdit->setTime(QTime::currentTime());
}

}

void sell_win::update_itemsList()
{
    clearLines();
    db.close();
    db.open();
    int customer_ind = ui->custom_name->currentIndex();
	model4->setQuery("SELECT items.name, sub_items.size ,sub_items.color, sub_items.int_barcode , sub_items.id FROM items JOIN sub_items on items.id = sub_items.item_id JOIN (SELECT DISTINCT sub_item_id as sub_item_id FROM item_balance) ss on ss.sub_item_id = sub_items.id ORDER BY items.name ASC",db);
	model5->setQuery("SELECT sub_items.int_barcode FROM sub_items JOIN (SELECT DISTINCT sub_item_id as sub_item_id FROM item_balance) ss on ss.sub_item_id = sub_items.id ORDER BY sub_items.id ASC",db);
	model6->setQuery("SELECT exp_barcode , sub_item_id FROM item_balance order by sub_item_id ASC",db);
	model7->setQuery("SELECT sub_items.id,items.name,sub_items.int_barcode,sub_items.sell_price FROM items JOIN sub_items on items.id = sub_items.item_id JOIN (SELECT DISTINCT sub_item_id as sub_item_id FROM item_balance) ss on ss.sub_item_id = sub_items.id ORDER BY sub_items.id ASC",db);
	while (model7->canFetchMore()) {
		model7->fetchMore();
	}
    model10->setQuery("SELECT name,id FROM cash_deposit",db);
    model12->setQuery("SELECT name,id FROM branch",db);
    model14->setQuery("SELECT name,id FROM customers ORDER BY id ASC ",db);
    model2->setQuery("SELECT name,id FROM storage",db);
    model3->setQuery("SELECT name,id FROM employees",db);
    model11->setQuery("SELECT name,id FROM employees WHERE appear_as_cashier = 1",db);
    model13->setQuery("SELECT name,id FROM currency",db);
    ui->stock_name->setCurrentIndex(0);
    ui->branch_name->setCurrentIndex(0);
    ui->cash_name->setCurrentIndex(0);
    ui->custom_name->setCurrentIndex(customer_ind);
    ui->currency_name->setCurrentIndex(0);
    ui->cashier_name->setCurrentIndex(0);
    ui->seller_name->setCurrentIndex(0);

}

void sell_win::update_List()
{
    db.close();
    db.open();
    int customer_ind = ui->custom_name->currentIndex();
    model14->setQuery("SELECT name,id FROM customers ORDER BY id ASC ",db);
    ui->custom_name->setCurrentIndex(customer_ind);

}

void sell_win::search_invoice()
{
    QString search_id = inv_search->id_out;
    int search_type = inv_search->type;
    if(!search_id.isEmpty()){
        if(search_type == 0){
            QSqlQuery qry(db);
            int counter = 0;
           if(!ui->tableWidget->isHidden()){
               if(ui->rec_stat->currentIndex() == 0){
                  qry.prepare("SELECT * from sales_g where rec_no = '"+search_id+"'");
                  if (qry.exec())
                  {
                      while (qry.next())
                       {
                           counter++;
                       }
                  }
                  if(counter!=0){
                      ui->rec_no->setText(search_id);
                  }
                  else{
                        QMessageBox::warning(this,"No result was Found","لم  يتم العثور علي نتيجة");
                      //insert error messege
                  }
               }
               else if(ui->rec_stat->currentIndex() == 1){
                   qry.prepare("SELECT * from sales_return_g where rec_no = '"+search_id+"'");
                   if (qry.exec())
                   {
                      while (qry.next())
                       {
                           counter++;
                       }
                   }
                   if(counter!=0){
                      ui->rec_no->setText(search_id);
                   }
                   else{
                        QMessageBox::warning(this,"No result was Found","لم  يتم العثور علي نتيجة");
                      //insert error messege
                   }
                }
           }else{
            if(ui->rec_stat->currentIndex() == 0){
               qry.prepare("SELECT * from sales where rec_no = '"+search_id+"'");
               if (qry.exec())
               {
                   while (qry.next())
                    {
                        counter++;
                    }
               }
               if(counter!=0){
                   set_unedit();
                   saved = true;
                   ui->tableWidget->hide();
                   ui->tableWidget_2->hide();
                   ui->tableView_2->hide();
                   ui->tableView->show();
                   ui->rec_no->setText(search_id);
               }
               else{
                   QMessageBox::warning(this,"No result was Found","لم  يتم العثور علي نتيجة");
                   //insert error messege
               }
            }
            else if(ui->rec_stat->currentIndex() == 1){
                qry.prepare("SELECT * from sales_return where rec_no = '"+search_id+"'");
                if (qry.exec())
                {
                   while (qry.next())
                    {
                        counter++;
                    }
                }
                if(counter!=0){
                    set_unedit();
                    saved = true;
                    ui->tableWidget->hide();
                    ui->tableWidget_2->hide();
                    ui->tableView_2->show();
                    ui->tableView->hide();
                   ui->rec_no->setText(search_id);
                }
                else{
                   QMessageBox::warning(this,"No result was Found","لم  يتم العثور علي نتيجة");
                   //insert error messege
                }
            }
           }
        }else if (search_type == 1){
            QSqlQuery qry(db);
            search_list.clear();
            max_search_index = 0;
            cur_search_index = 0;
            if(!ui->tableWidget->isHidden()){
                if(ui->rec_stat->currentIndex() == 0){
                   qry.prepare("SELECT rec_no from sales_g where customer_id = '"+search_id+"'");
                   if (qry.exec())
                   {
                       while (qry.next())
                       {
                           search_list.append(qry.value(0).toInt());

                       }
                   }
                   if(search_list.size() == 0){
                       QMessageBox::warning(this,"No result was Found","لم  يتم العثور علي نتيجة");
                   }else {
                       searching = true;
                       max_search_index = search_list.size()-1;
                       cur_search_index = search_list.size()-1;
                       ui->rec_no->setText(QString::number(search_list.at(cur_search_index)));
                   }
                }
                else if(ui->rec_stat->currentIndex() == 1){
                    qry.prepare("SELECT rec_no from sales_return_g where customer_id = '"+search_id+"'");
                   if (qry.exec())
                   {
                       while (qry.next())
                       {
                        search_list.append(qry.value(0).toInt());
                       }
                   }
                   if(search_list.size() == 0){
                       QMessageBox::warning(this,"No result was Found","لم  يتم العثور علي نتيجة");
                   }else {
                   searching = true;
                   max_search_index = search_list.size()-1;
                   cur_search_index = search_list.size()-1;
                   ui->rec_no->setText(QString::number(search_list.at(cur_search_index)));}
                }

            }else {
                if(ui->rec_stat->currentIndex() == 0){
                   qry.prepare("SELECT rec_no from sales where customer_id = '"+search_id+"'");
                   if (qry.exec())
                   {
                       while (qry.next())
                       {
                           search_list.append(qry.value(0).toInt());

                       }
                   }
                   if(search_list.size() == 0){
                       QMessageBox::warning(this,"No result was Found","لم  يتم العثور علي نتيجة");
                   }else {
                   max_search_index = search_list.size()-1;
                   cur_search_index = search_list.size()-1;
                   set_unedit();
                   saved = true;
                   searching = true;
                   ui->tableWidget->hide();
                   ui->tableWidget_2->hide();
                   ui->tableView_2->hide();
                   ui->tableView->show();
                   ui->rec_no->setText(QString::number(search_list.at(cur_search_index)));}

                }
                else if(ui->rec_stat->currentIndex() == 1){
                    qry.prepare("SELECT rec_no from sales_return where customer_id = '"+search_id+"'");
                   if (qry.exec())
                   {
                       while (qry.next())
                       {
                        search_list.append(qry.value(0).toInt());
                       }
                   }
                   if(search_list.size() == 0){
                       QMessageBox::warning(this,"No result was Found","لم  يتم العثور علي نتيجة");
                   }else {
                   max_search_index = search_list.size()-1;
                   cur_search_index = search_list.size()-1;
                   set_unedit();
                   saved = true;
                   searching = true;
                   ui->tableWidget->hide();
                   ui->tableWidget_2->hide();
                   ui->tableView_2->show();
                   ui->tableView->hide();
                   ui->rec_no->setText(QString::number(search_list.at(cur_search_index)));}
                }
            }



        }
    }
}

void sell_win::show_search()
{
    inv_search->search_type = 1;
    inv_search->show();
    inv_search->clear_all();
}


sell_win::~sell_win()
{
    delete ui;
}

void sell_win::setData()
{
     ui->item_int->lineEdit()->clear();
     ui->ex_bcode->lineEdit()->clear();
     QModelIndex qq = ui->item_name->view()->currentIndex();
     int row = qq.row();
     code_id = model4->index(row,4).data().toInt();
     QString int_code = model4->index(row,3).data().toString();
	 QList<QModelIndex> modelIndexList;
     modelIndexList.clear(); 
	 modelIndexList = model5->match(model5->index(0, 0), Qt::DisplayRole, QVariant::fromValue(int_code), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
         ui->item_int->setCurrentIndex(modelIndexList.first().row());
     }
	 modelIndexList.clear();
	 modelIndexList = model7->match(model7->index(0, 0), Qt::DisplayRole, QVariant::fromValue(code_id), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
		 ui->sell_price->setText(model7->index(modelIndexList.first().row(),3).data().toString());
     }
	 
     QSqlQuery qry3(db);
     qry3.prepare("SELECT expire FROM item_balance WHERE sub_item_id = :id");
     qry3.bindValue(":id",code_id);
     if(qry3.exec())
     {}
     else{qDebug() << "Error inserting into the table:\n" << qry3.lastError();}	 
     model8->setQuery(qry3);
     ui->expire->setModel(model8);
     ui->expire->setCurrentIndex(0);
	 
	 modelIndexList.clear();
	 modelIndexList = model6->match(model6->index(0, 1), Qt::DisplayRole, QVariant::fromValue(code_id), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
		 ui->ex_bcode->setCurrentIndex(modelIndexList.first().row());
     }
	 
	 
     ui->quan->setText("1");
     ui->discount_per->setText("0");
}
void sell_win::on_item_name_currentIndexChanged(int index)
{
     ui->item_int->lineEdit()->clear();
     ui->ex_bcode->lineEdit()->clear();
     int row = index;
     code_id = model4->index(row,4).data().toInt();
     QString int_code = model4->index(row,3).data().toString();
	 QList<QModelIndex> modelIndexList;
     modelIndexList.clear(); 
	 modelIndexList = model5->match(model5->index(0, 0), Qt::DisplayRole, QVariant::fromValue(int_code), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
         ui->item_int->setCurrentIndex(modelIndexList.first().row());
     }
	 modelIndexList.clear();
	 modelIndexList = model7->match(model7->index(0, 0), Qt::DisplayRole, QVariant::fromValue(code_id), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
		 ui->sell_price->setText(model7->index(modelIndexList.first().row(),3).data().toString());
     }
	 
     QSqlQuery qry3(db);
     qry3.prepare("SELECT expire FROM item_balance WHERE sub_item_id = :id");
     qry3.bindValue(":id",code_id);
     if(qry3.exec())
     {}
     else{qDebug() << "Error inserting into the table:\n" << qry3.lastError();}	 
     model8->setQuery(qry3);
     ui->expire->setModel(model8);
     ui->expire->setCurrentIndex(0);
	 
	 modelIndexList.clear();
	 modelIndexList = model6->match(model6->index(0, 1), Qt::DisplayRole, QVariant::fromValue(code_id), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
		 ui->ex_bcode->setCurrentIndex(modelIndexList.first().row());
     }
	 
	 
     ui->quan->setText("1");
     ui->discount_per->setText("0");


}
void sell_win::enterName()
{
    if(!ui->item_name->lineEdit()->text().isEmpty()){
        ui->item_int->lineEdit()->clear();
        ui->ex_bcode->lineEdit()->clear();
    int qq = ui->item_name->currentIndex();
    code_id = model4->index(qq,4).data().toInt();
     QString int_code = model4->index(qq,3).data().toString();
	 QList<QModelIndex> modelIndexList;
     modelIndexList.clear(); 
	 modelIndexList = model5->match(model5->index(0, 0), Qt::DisplayRole, QVariant::fromValue(int_code), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
         ui->item_int->setCurrentIndex(modelIndexList.first().row());
     }
	 modelIndexList.clear();
	 modelIndexList = model7->match(model7->index(0, 0), Qt::DisplayRole, QVariant::fromValue(code_id), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
		 ui->sell_price->setText(model7->index(modelIndexList.first().row(),3).data().toString());
     }
	 
     QSqlQuery qry3(db);
     qry3.prepare("SELECT expire FROM item_balance WHERE sub_item_id = :id");
     qry3.bindValue(":id",code_id);
     if(qry3.exec())
     {}
     else{qDebug() << "Error inserting into the table:\n" << qry3.lastError();}	 
     model8->setQuery(qry3);
     ui->expire->setModel(model8);
     ui->expire->setCurrentIndex(0);
	 
	 modelIndexList.clear();
	 modelIndexList = model6->match(model6->index(0, 1), Qt::DisplayRole, QVariant::fromValue(code_id), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
		 ui->ex_bcode->setCurrentIndex(modelIndexList.first().row());
     }
	 
	 
     ui->quan->setText("1");
     ui->discount_per->setText("0");
     ui->item_int->lineEdit()->setFocus();
    }
    else{
        ui->item_int->lineEdit()->setFocus();
    }
}
void sell_win::enterInt()
{
    if(!ui->item_int->lineEdit()->text().isEmpty()){
        ui->ex_bcode->lineEdit()->clear();
        QString int_code = ui->item_int->currentText();
		QList<QModelIndex> modelIndexList;
		modelIndexList.clear(); 
		modelIndexList = model7->match(model7->index(0, 2), Qt::DisplayRole, QVariant::fromValue(int_code), -1, Qt::MatchExactly);
		 if(!modelIndexList.isEmpty()){
			 ui->sell_price->setText(model7->index(modelIndexList.first().row(),3).data().toString());
			 code_id = model7->index(modelIndexList.first().row(),0).data().toInt();
		 }
		modelIndexList.clear(); 
		 modelIndexList = model4->match(model4->index(0, 3), Qt::DisplayRole, QVariant::fromValue(int_code), -1, Qt::MatchExactly);
		 if(!modelIndexList.isEmpty()){
			 ui->item_name->setCurrentIndex(modelIndexList.first().row());
		 } 
		
		QSqlQuery qry3(db);
		qry3.prepare("SELECT expire FROM item_balance WHERE sub_item_id = :id");
		qry3.bindValue(":id",code_id);
		if(qry3.exec())
		{
		}
		else{qDebug() << "Error inserting into the table:\n" << qry3.lastError();}
		model8->setQuery(qry3);
		ui->expire->setModel(model8);
		ui->expire->setCurrentIndex(0);
		
		modelIndexList.clear();
		modelIndexList = model6->match(model6->index(0, 1), Qt::DisplayRole, QVariant::fromValue(code_id), -1, Qt::MatchExactly);
		if(!modelIndexList.isEmpty()){
		 ui->ex_bcode->setCurrentIndex(modelIndexList.first().row());
		}
		
		ui->ex_bcode->lineEdit()->setFocus();
    }
    else{
       ui->ex_bcode->lineEdit()->setFocus();
    }
}
void sell_win::enterBcode(){
    if(!ui->ex_bcode->lineEdit()->text().isEmpty()){
		QString code_id1 = ui->ex_bcode->lineEdit()->text();
		int bcode;
		QString int_barcode;
		QList<QModelIndex> modelIndexList;
		modelIndexList.clear();
		modelIndexList = model6->match(model6->index(0, 0), Qt::DisplayRole, QVariant::fromValue(code_id1), -1, Qt::MatchExactly);
		if(!modelIndexList.isEmpty()){
		 bcode = model6->index(modelIndexList.first().row(),1).data().toInt();
		}

     QSqlQuery qry3(db);
     qry3.prepare("SELECT expire FROM item_balance WHERE exp_barcode = :id");
     qry3.bindValue(":id",code_id1);
     if(qry3.exec())
     {

     }
     else{qDebug() << "Error inserting into the table:\n" << qry3.lastError();}
     model8->setQuery(qry3);
     ui->expire->setModel(model8);
     ui->expire->setCurrentIndex(0);
	 
	 modelIndexList.clear(); 
	 modelIndexList = model7->match(model7->index(0, 0), Qt::DisplayRole, QVariant::fromValue(bcode), -1, Qt::MatchExactly);
	 if(!modelIndexList.isEmpty()){
			 ui->sell_price->setText(model7->index(modelIndexList.first().row(),3).data().toString());
			 int_barcode = model7->index(modelIndexList.first().row(),2).data().toString();
		 }
	 modelIndexList.clear(); 
	 modelIndexList = model4->match(model4->index(0, 4), Qt::DisplayRole, QVariant::fromValue(bcode), -1, Qt::MatchExactly);
	 if(!modelIndexList.isEmpty()){
			 ui->item_name->setCurrentIndex(modelIndexList.first().row());
		 }

     modelIndexList.clear();
     modelIndexList = model5->match(model5->index(0, 0), Qt::DisplayRole, QVariant::fromValue(int_barcode), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
         ui->item_int->setCurrentIndex(modelIndexList.first().row());
     }

     ui->quan->setText("1");
     ui->discount_per->setText("0");
     ui->quan->setFocus();
     ui->quan->selectAll();
    }
    else{
        ui->quan->setText("1");
        ui->discount_per->setText("0");
        ui->quan->setFocus();
        ui->quan->selectAll();
    }
}
void sell_win::clearAll()
{
   saved = false;
   searching = false;
   search_list.clear();
   max_search_index = 0;
   cur_search_index = 0;
   clearLines();
   ui->custom_name->lineEdit()->clear();
   ui->stock_name->lineEdit()->clear();
   ui->branch_name->lineEdit()->clear();
   ui->currency_name->lineEdit()->clear();
   ui->cashier_name->lineEdit()->clear();
   ui->seller_name->lineEdit()->clear();
   ui->custom_name->clear();
   ui->custom_bala->clear();
   ui->cash_name->lineEdit()->clear();
   ui->custom_bala->clear();
   ui->total_before->clear();
   ui->total_items->clear();
   ui->total_after->clear();
   ui->discount_perc->clear();
   ui->dicount->clear();
   ui->tableWidget_2->clear();
   ui->tableWidget->clear();
   ui->tableView->hide();
   ui->tableView_2->hide();
   ui->tableWidget->hide();
   ui->tableWidget_2->show();
   ui->tableWidget_2->setRowCount(0);
   ui->tableWidget->setRowCount(0);
   ui->cash->setChecked(true);
   ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
   ui->item_name->completer()->setCompletionMode(QCompleter::PopupCompletion);
   db.close();
   db.open();
   int saved_count;
   int unsaved_count;
   if(ui->rec_stat->currentIndex()==0){
   QSqlQuery qry(db);
   qry.prepare("SELECT MAX(rec_no) from sales");
   if (qry.exec())
   {
       while (qry.next())
       {          
         saved_count = qry.value(0).toInt();
       }
   }
   QSqlQuery qry2(db);
   qry2.prepare("SELECT MAX(rec_no) from sales_g");
   if (qry2.exec())
   {
       while (qry2.next())
       {
         unsaved_count = qry2.value(0).toInt();
       }
   }
   if(saved_count>unsaved_count||(saved_count&&unsaved_count)==0){
       saved_count+=1;
       ui->rec_no->setText(QString::number(saved_count));
   }
   else if(unsaved_count>saved_count){
       unsaved_count+=1;
       ui->rec_no->setText(QString::number(unsaved_count));
   }

 }
   else if(ui->rec_stat->currentIndex()==1){
       QSqlQuery qry(db);
       qry.prepare("SELECT MAX(rec_no) from sales_return");
       if (qry.exec())
       {
           while (qry.next())
           {
				saved_count = qry.value(0).toInt();
				ui->rec_no->setText(QString::number(saved_count));


           }
       }
       QSqlQuery qry2(db);
       qry2.prepare("SELECT MAX(rec_no) from sales_return_g");
       if (qry2.exec())
       {
           while (qry2.next())
           {
             unsaved_count = qry2.value(0).toInt();
           }
       }
       if(saved_count>unsaved_count||(saved_count&&unsaved_count)==0){
           saved_count+=1;
           ui->rec_no->setText(QString::number(saved_count));
       }
       else if(unsaved_count>saved_count){
           unsaved_count+=1;
           ui->rec_no->setText(QString::number(unsaved_count));
       }

   }
   /**/
   model4->setQuery("SELECT items.name, sub_items.size ,sub_items.color, sub_items.int_barcode , sub_items.id FROM items JOIN sub_items on items.id = sub_items.item_id JOIN (SELECT DISTINCT sub_item_id as sub_item_id FROM item_balance) ss on ss.sub_item_id = sub_items.id ORDER BY items.name ASC",db);
   tv->hideColumn(3);
   tv->hideColumn(4);
   model5->setQuery("SELECT sub_items.int_barcode FROM sub_items JOIN (SELECT DISTINCT sub_item_id as sub_item_id FROM item_balance) ss on ss.sub_item_id = sub_items.id ORDER BY sub_items.id ASC",db);
   model6->setQuery("SELECT exp_barcode , sub_item_id FROM item_balance order by sub_item_id ASC",db);
   model7->setQuery("SELECT sub_items.id,items.name,sub_items.int_barcode,sub_items.sell_price FROM items JOIN sub_items on items.id = sub_items.item_id JOIN (SELECT DISTINCT sub_item_id as sub_item_id FROM item_balance) ss on ss.sub_item_id = sub_items.id ORDER BY sub_items.id ASC",db);
   while (model7->canFetchMore()) {
      model7->fetchMore();
    }
   model10->setQuery("SELECT name,id FROM cash_deposit",db);
   model12->setQuery("SELECT name,id FROM branch",db);
   model14->setQuery("SELECT name,id FROM customers ORDER BY id ASC ",db);
   model2->setQuery("SELECT name,id FROM storage",db);
   model3->setQuery("SELECT name,id FROM employees",db);
   model11->setQuery("SELECT name,id FROM employees WHERE appear_as_cashier = 1",db);
   model13->setQuery("SELECT name,id FROM currency",db);


   ui->stock_name->setCurrentIndex(0);
   ui->branch_name->setCurrentIndex(0);
   ui->cash_name->setCurrentIndex(0);
   
   QList<QModelIndex> modelIndexList;
     modelIndexList.clear();
	 //change to barcode matching
     modelIndexList = model14->match(model14->index(0, 1), Qt::DisplayRole, QVariant::fromValue(10001), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
         ui->custom_name->setCurrentIndex(modelIndexList.first().row());
     }
	 
   ui->currency_name->setCurrentIndex(0);
   ui->cashier_name->setCurrentIndex(0);
   ui->seller_name->setCurrentIndex(0);
   ui->dicount->setText("0");
   ui->discount_perc->setText("0");
   ui->tableWidget_2->setColumnCount(7);
   ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"Item name"<<"international Barcode"<<"Barcode"<<"Quantity"<<"Price"<<"Discount"<<"Total");
   ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   ui->tableWidget->setColumnCount(7);
   ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Item name"<<"international Barcode"<<"Barcode"<<"Quantity"<<"Price"<<"Discount"<<"Total");
   ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   tv->horizontalHeader()->setVisible(false);
   tv->verticalHeader()->setVisible(false);
   tv->resizeColumnsToContents();
   tv->setMinimumWidth(400);
   tv->setSelectionBehavior(QAbstractItemView::SelectRows);

}
void sell_win::clearLines()
{
    ui->item_int->lineEdit()->clear();
    ui->item_name->lineEdit()->clear();
    ui->ex_bcode->lineEdit()->clear();
    ui->quan->clear();
    ui->sell_price->clear();
    ui->total_per->clear();
    ui->discount_per->clear();
    ui->item_int->lineEdit()->setFocus();
}
void sell_win::setTable()
{
		//tableWidget2 is normal before saving

    if(ui->tableWidget_2->isVisible()&&ui->tableWidget->isHidden()){
    int CurrentRow = ui->tableWidget_2->rowCount();
    QTableWidgetItem *item_name = new QTableWidgetItem();
    QTableWidgetItem *int_code = new QTableWidgetItem();
    QTableWidgetItem *barcode = new QTableWidgetItem();
    QTableWidgetItem *quan = new QTableWidgetItem();
    QTableWidgetItem *price = new QTableWidgetItem();
    QTableWidgetItem *discount = new QTableWidgetItem();
    QTableWidgetItem *total = new QTableWidgetItem();
    item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);
    item_name->setCheckState(Qt::Unchecked);
    int_code->setFlags(int_code->flags() & ~Qt::ItemIsEditable);
    barcode->setFlags(barcode->flags() & ~Qt::ItemIsEditable);
    price->setFlags(price->flags() & ~Qt::ItemIsEditable);
    total->setFlags(total->flags() & ~Qt::ItemIsEditable);
    discount->setFlags(total->flags() & ~Qt::ItemIsEditable);
    quan->setFlags(total->flags() & ~Qt::ItemIsEditable);

    item_name->setText("  "+model4->index(ui->item_name->currentIndex(),0).data().toString()+model4->index(ui->item_name->currentIndex(),1).data().toString()+"  "+model4->index(ui->item_name->currentIndex(),2).data().toString()+"  ");
    int_code->setText(ui->item_int->lineEdit()->text());
    barcode->setText(ui->ex_bcode->lineEdit()->text());
    quan->setText(QString::number(ui->quan->text().toDouble()));
    price->setText(QString::number(ui->sell_price->text().toDouble()));
    discount->setText(QString::number(ui->discount_per->text().toDouble()));
    total->setText(QString::number(ui->total_per->text().toDouble()));

    double discount_per = ui->discount_per->text().toDouble();
    double discount_tot=ui->dicount->text().toDouble();
    double discount_total= discount_per + discount_tot;
    ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
    ui->tableWidget_2->setItem(CurrentRow, 0, item_name);
    ui->tableWidget_2->setItem(CurrentRow, 1, int_code);
    ui->tableWidget_2->setItem(CurrentRow, 2, barcode);
    ui->tableWidget_2->setItem(CurrentRow, 3, quan);
    ui->tableWidget_2->setItem(CurrentRow, 4, price);
    ui->tableWidget_2->setItem(CurrentRow, 5, discount);
    ui->tableWidget_2->setItem(CurrentRow, 6,total);
    ui->dicount->setText(QString::number(discount_total));
	getTotal();
	getTotal_items();
	
	QString rec_no = ui->rec_no->text();
    QString custom_id = model14->index(ui->custom_name->currentIndex(),1).data().toString();
    QString discount_g = ui->dicount->text();
    QString total_g = ui->total_after->text();
    QString total_before =ui->total_before->text();
    QString cashier_id = model10->index(ui->cash_name->currentIndex(),1).data().toString();
    QString stock_id = model2->index(ui->stock_name->currentIndex(),1).data().toString();
    QString branch_id = model12->index(ui->branch_name->currentIndex(),1).data().toString();
    QString seller_id = model3->index(ui->seller_name->currentIndex(),1).data().toString();
    QString cashieer_id = model11->index(ui->cashier_name->currentIndex(),1).data().toString();
    QString currency_id = model13->index(ui->currency_name->currentIndex(),1).data().toString();
    QString payment;
    if(ui->cash->isChecked()){
      payment = "Cash";

    }
    else if(ui->credit->isChecked()){
       payment = "Credit";
    }
    else if ( ui->credit_card->isChecked()) {
       payment = "Credit card";
    }
	
	 if(ui->rec_stat->currentIndex() == 0){
            int recCount=0;
             QSqlQuery qry2(db);
             qry2.prepare("SELECT * FROM sales_g WHERE rec_no = :rec_no");
             qry2.bindValue(":rec_no",rec_no);
             if( qry2.exec() ){
                 while (qry2.next())
                 {
                     recCount++;
                 }
             }
             else{
                   qDebug() << "Error inserting into the table:\n" << qry2.lastError();
            }
        if(recCount==0){
        if(ui->cash->isChecked()){
        QSqlQuery sales_csh(db);
        sales_csh.prepare("INSERT INTO sales_g(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,0,:cashier,:seller,:currency)");
        sales_csh.bindValue(":rec_no",rec_no);
        sales_csh.bindValue(":custom_id",custom_id);
        sales_csh.bindValue(":discount",discount_g);
        sales_csh.bindValue(":total_before",total_before);
        sales_csh.bindValue(":discount",discount_g);
        sales_csh.bindValue(":cashier_id",cashier_id);
        sales_csh.bindValue(":stock_id",stock_id);
        sales_csh.bindValue(":branch_id",branch_id);
        sales_csh.bindValue(":payment",payment);
        sales_csh.bindValue(":total",total_g);
        sales_csh.bindValue(":cashier",cashieer_id);
        sales_csh.bindValue(":seller",seller_id);
        sales_csh.bindValue(":currency",currency_id);
        if(!sales_csh.exec())
            qDebug()<<sales_csh.lastError();
        }
        else if(ui->credit->isChecked()){
            QSqlQuery sales_crdt(db);
            sales_crdt.prepare("INSERT INTO sales_g(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,1,:cashier,:seller,:currency)");
            sales_crdt.bindValue(":rec_no",rec_no);
            sales_crdt.bindValue(":custom_id",custom_id);
            sales_crdt.bindValue(":discount",discount_g);
            sales_crdt.bindValue(":total_before",total_before);
            sales_crdt.bindValue(":discount",discount_g);
            sales_crdt.bindValue(":cashier_id",cashier_id);
            sales_crdt.bindValue(":stock_id",stock_id);
            sales_crdt.bindValue(":branch_id",branch_id);
            sales_crdt.bindValue(":payment",payment);
            sales_crdt.bindValue(":total",total_g);
            sales_crdt.bindValue(":cashier",cashieer_id);
            sales_crdt.bindValue(":seller",seller_id);
            sales_crdt.bindValue(":currency",currency_id);
            if(!sales_crdt.exec())
                qDebug()<<sales_crdt.lastError();

        }
        else if(ui->credit_card->isChecked()){
            QSqlQuery sales_crdt_crd(db);
            sales_crdt_crd.prepare("INSERT INTO sales_g(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,2,:cashier,:seller,:currency)");
            sales_crdt_crd.bindValue(":rec_no",rec_no);
            sales_crdt_crd.bindValue(":custom_id",custom_id);
            sales_crdt_crd.bindValue(":discount",discount_g);
            sales_crdt_crd.bindValue(":total_before",total_before);
            sales_crdt_crd.bindValue(":discount",discount_g);
            sales_crdt_crd.bindValue(":cashier_id",cashier_id);
            sales_crdt_crd.bindValue(":stock_id",stock_id);
            sales_crdt_crd.bindValue(":branch_id",branch_id);
            sales_crdt_crd.bindValue(":payment",payment);
            sales_crdt_crd.bindValue(":total",total_g);
            sales_crdt_crd.bindValue(":cashier",cashieer_id);
            sales_crdt_crd.bindValue(":seller",seller_id);
            sales_crdt_crd.bindValue(":currency",currency_id);
            if(!sales_crdt_crd.exec())
                qDebug()<<sales_crdt_crd.lastError();}
		
		}
			QSqlQuery salesb(db);
             salesb.prepare("INSERT INTO sales_items_g(rec_no,item_name,item_int,exp_barcode,quantity,price,discount_per,total)VALUES( :sales_id, :item_name, :item_int, :barcode, :quan, :price, :discount, :total)");
						salesb.bindValue(":sales_id",ui->rec_no->text());
                        salesb.bindValue(":item_name",ui->item_name->lineEdit()->text());
                        salesb.bindValue(":item_int",ui->item_int->lineEdit()->text());
                        salesb.bindValue(":barcode",ui->ex_bcode->lineEdit()->text());
                        salesb.bindValue(":quan",ui->quan->text().toDouble());
                        salesb.bindValue(":price",ui->sell_price->text().toDouble());
						salesb.bindValue(":discount",ui->discount_per->text().toDouble());
						salesb.bindValue(":total",ui->total_per->text().toDouble());
                         if(!salesb.exec())
                            qDebug()<<salesb.lastError();		
    }
	else if(ui->rec_stat->currentIndex()==1){
		            int recCount=0;
             QSqlQuery qry2(db);
             qry2.prepare("SELECT * FROM sales_return_g WHERE rec_no = :rec_no");
             qry2.bindValue(":rec_no",rec_no);
             if( qry2.exec() ){
                 while (qry2.next())
                 {
                     recCount++;
                 }
             }
             else{
                   qDebug() << "Error inserting into the table:\n" << qry2.lastError();
            }
             if(recCount==0){
            if(ui->cash->isChecked()){

            QSqlQuery salesrt_csh(db);
            salesrt_csh.prepare("INSERT INTO sales_return_g(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,0,:cashier,:seller,:currency)");
            salesrt_csh.bindValue(":rec_no",rec_no);
            salesrt_csh.bindValue(":custom_id",custom_id);
            salesrt_csh.bindValue(":discount",discount_g);
            salesrt_csh.bindValue(":total_before",total_before);
            salesrt_csh.bindValue(":discount",discount_g);
            salesrt_csh.bindValue(":cashier_id",cashier_id);
            salesrt_csh.bindValue(":stock_id",stock_id);
            salesrt_csh.bindValue(":branch_id",branch_id);
            salesrt_csh.bindValue(":payment",payment);
            salesrt_csh.bindValue(":total",total_g);
            salesrt_csh.bindValue(":cashier",cashieer_id);
            salesrt_csh.bindValue(":seller",seller_id);
            salesrt_csh.bindValue(":currency",currency_id);
            if(!salesrt_csh.exec())
                qDebug()<<salesrt_csh.lastError();
            }
            else if(ui->credit->isChecked()){

                QSqlQuery salesrt_crdt(db);
                salesrt_crdt.prepare("INSERT INTO sales_return_g(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,1,:cashier,:seller,:currency)");
                salesrt_crdt.bindValue(":rec_no",rec_no);
                salesrt_crdt.bindValue(":custom_id",custom_id);
                salesrt_crdt.bindValue(":discount",discount_g);
                salesrt_crdt.bindValue(":total_before",total_before);
                salesrt_crdt.bindValue(":discount",discount_g);
                salesrt_crdt.bindValue(":cashier_id",cashier_id);
                salesrt_crdt.bindValue(":stock_id",stock_id);
                salesrt_crdt.bindValue(":branch_id",branch_id);
                salesrt_crdt.bindValue(":payment",payment);
                salesrt_crdt.bindValue(":total",total_g);
                salesrt_crdt.bindValue(":cashier",cashieer_id);
                salesrt_crdt.bindValue(":seller",seller_id);
                salesrt_crdt.bindValue(":currency",currency_id);
                if(!salesrt_crdt.exec())
                    qDebug()<<salesrt_crdt.lastError();

            }
            else if(ui->credit_card->isChecked()){

                QSqlQuery salesrt_crdt_crd(db);
                salesrt_crdt_crd.prepare("INSERT INTO sales_return_g(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,2,:cashier,:seller,:currency)");
                salesrt_crdt_crd.bindValue(":rec_no",rec_no);
                salesrt_crdt_crd.bindValue(":custom_id",custom_id);
                salesrt_crdt_crd.bindValue(":discount",discount_g);
                salesrt_crdt_crd.bindValue(":total_before",total_before);
                salesrt_crdt_crd.bindValue(":discount",discount_g);
                salesrt_crdt_crd.bindValue(":cashier_id",cashier_id);
                salesrt_crdt_crd.bindValue(":stock_id",stock_id);
                salesrt_crdt_crd.bindValue(":branch_id",branch_id);
                salesrt_crdt_crd.bindValue(":payment",payment);
                salesrt_crdt_crd.bindValue(":total",total_g);
                salesrt_crdt_crd.bindValue(":cashier",cashieer_id);
                salesrt_crdt_crd.bindValue(":seller",seller_id);
                salesrt_crdt_crd.bindValue(":currency",currency_id);
                if(!salesrt_crdt_crd.exec())
                    qDebug()<<salesrt_crdt_crd.lastError();
            }
		
		}
		 QSqlQuery sales_return(db);
		sales_return.prepare("INSERT INTO sales_return_items_g(rec_no,item_name,item_int,exp_barcode,quantity,price,discount_per,total)VALUES( :sales_id, :item_name, :item_int, :barcode, :quan, :price, :discount, :total)");
		sales_return.bindValue(":sales_id",ui->rec_no->text());
        sales_return.bindValue(":item_name",ui->item_name->lineEdit()->text());
        sales_return.bindValue(":item_int",ui->item_int->lineEdit()->text());
        sales_return.bindValue(":barcode",ui->ex_bcode->lineEdit()->text());
        sales_return.bindValue(":quan",ui->quan->text().toDouble());
        sales_return.bindValue(":price",ui->sell_price->text().toDouble());
		sales_return.bindValue(":discount",ui->discount_per->text().toDouble());
		sales_return.bindValue(":total",ui->total_per->text().toDouble());
         if(!sales_return.exec())
          qDebug()<<sales_return.lastError();
		
		
		
	}
    }
	//tableWidget is unsaved
    else if(ui->tableWidget->isVisible()&&!ui->tableWidget_2->isVisible())
    {
        int CurrentRow = ui->tableWidget->rowCount();
        QTableWidgetItem *item_name = new QTableWidgetItem();
        QTableWidgetItem *int_code = new QTableWidgetItem();
        QTableWidgetItem *barcode = new QTableWidgetItem();
        QTableWidgetItem *quan = new QTableWidgetItem();
        QTableWidgetItem *price = new QTableWidgetItem();
        QTableWidgetItem *discount = new QTableWidgetItem();
        QTableWidgetItem *total = new QTableWidgetItem();
        item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);
        int_code->setFlags(int_code->flags() & ~Qt::ItemIsEditable);
        barcode->setFlags(barcode->flags() & ~Qt::ItemIsEditable);
        price->setFlags(price->flags() & ~Qt::ItemIsEditable);
        total->setFlags(total->flags() & ~Qt::ItemIsEditable);
        item_name->setText("  "+model4->index(ui->item_name->currentIndex(),0).data().toString()+model4->index(ui->item_name->currentIndex(),1).data().toString()+"  "+model4->index(ui->item_name->currentIndex(),2).data().toString()+"  ");
        int_code->setText(ui->item_int->lineEdit()->text());
        barcode->setText(ui->ex_bcode->lineEdit()->text());
        quan->setText(QString::number(ui->quan->text().toDouble()));
        price->setText(QString::number(ui->sell_price->text().toDouble()));
        discount->setText(QString::number(ui->discount_per->text().toDouble()));
        total->setText(QString::number(ui->total_per->text().toDouble()));
        double discount_per = ui->discount_per->text().toDouble();
        double discount_tot=ui->dicount->text().toDouble();
        double discount_total= discount_per + discount_tot;
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(CurrentRow, 0, item_name);
        ui->tableWidget->setItem(CurrentRow, 1, int_code);
        ui->tableWidget->setItem(CurrentRow, 2, barcode);
        ui->tableWidget->setItem(CurrentRow, 3, quan);
        ui->tableWidget->setItem(CurrentRow, 4, price);
        ui->tableWidget->setItem(CurrentRow, 5, discount);
        ui->tableWidget->setItem(CurrentRow, 6,total);
        ui->dicount->setText(QString::number(discount_total));
		getTotal();
		getTotal_items();
		
        if(ui->rec_stat->currentIndex()==0){
        QSqlQuery salesb(db);
		// create unsaved table and change it's name
        salesb.prepare("INSERT INTO sales_items_g(rec_no,item_name,item_int,exp_barcode,quantity,price,discount_per,total)VALUES( :sales_id, :item_name, :item_int, :barcode, :quan, :price, :discount, :total)");
        salesb.bindValue(":sales_id",ui->rec_no->text());
        salesb.bindValue(":item_name",ui->item_name->lineEdit()->text());
        salesb.bindValue(":item_int",ui->item_int->lineEdit()->text());
        salesb.bindValue(":barcode",ui->ex_bcode->lineEdit()->text());
        salesb.bindValue(":quan",ui->quan->text().toDouble());
        salesb.bindValue(":price",ui->sell_price->text().toDouble());
        salesb.bindValue(":discount",ui->discount_per->text().toDouble());
        salesb.bindValue(":total",ui->total_per->text().toDouble());
         if(!salesb.exec())
            qDebug()<<salesb.lastError();}
        else if(ui->rec_stat->currentIndex()==1){
            QSqlQuery salesb(db);
            salesb.prepare("INSERT INTO sales_return_items_g(rec_no,item_name,item_int,exp_barcode,quantity,price,discount_per,total)VALUES( :sales_id, :item_name, :item_int, :barcode, :quan, :price, :discount, :total)");
            salesb.bindValue(":sales_id",ui->rec_no->text());
            salesb.bindValue(":item_name",ui->item_name->lineEdit()->text());
            salesb.bindValue(":item_int",ui->item_int->lineEdit()->text());
            salesb.bindValue(":barcode",ui->ex_bcode->lineEdit()->text());
            salesb.bindValue(":quan",ui->quan->text().toDouble());
            salesb.bindValue(":price",ui->sell_price->text().toDouble());
            salesb.bindValue(":discount",ui->discount_per->text().toDouble());
            salesb.bindValue(":total",ui->total_per->text().toDouble());
             if(!salesb.exec())
                qDebug()<<salesb.lastError();
        }

    }

   clearLines();

}
void sell_win::getTotal()
{
    if(ui->tableWidget_2->isVisible()){
    int column = 6;
    int column2 = 5;
    double discount_total = 0;
    double total_2 = 0 ;

    for(int row = 0; row < ui->tableWidget_2->rowCount(); row++){
        total_2 +=  ui->tableWidget_2->item(row,column)->text().toDouble();
        discount_total +=  ui->tableWidget_2->item(row,column2)->text().toDouble();
    }
       ui->total_before->setText(QString::number(total_2+discount_total));}
    else if(ui->tableWidget->isVisible()){
        int column = 6;
        double total_2 = 0 ;
        int column2 = 5;
        double discount_total = 0;
        for(int row = 0; row < ui->tableWidget->rowCount(); row++){
            total_2 +=  ui->tableWidget->item(row,column)->text().toDouble();
            discount_total +=  ui->tableWidget->item(row,column2)->text().toDouble();

        }
           ui->total_before->setText(QString::number(total_2+discount_total));
           if(ui->rec_stat->currentIndex()==0){
           QSqlQuery qry(db);
		   //check to use total before or after
           qry.prepare("UPDATE sales_g SET total = :total , discount = :discount WHERE rec_no = :rec ");
           qry.bindValue(":total",ui->total_after->text());
           qry.bindValue(":discount",ui->dicount->text());
           qry.bindValue(":rec",ui->rec_no->text());
           if(!qry.exec())
               qDebug()<<qry.lastError();

           }
           else if(ui->rec_stat->currentIndex()==1){
               QSqlQuery qry(db);
               qry.prepare("UPDATE sales_return_g SET total = :total , discount = :discount WHERE rec_no = :rec ");
               qry.bindValue(":total",ui->total_after->text());
               qry.bindValue(":discount",ui->dicount->text());
               qry.bindValue(":rec",ui->rec_no->text());
               if(!qry.exec())
                   qDebug()<<qry.lastError();

           }


    }
}
void sell_win::getTotal_items()
{
	if(ui->tableWidget_2->isVisible()){
    int column = 3;
    double total = 0 ;
    for(int row = 0; row < ui->tableWidget_2->rowCount(); row++){
        total +=  ui->tableWidget_2->item(row,column)->text().toDouble();
    }
	ui->total_items->setText(QString::number(total));}
	else if (ui->tableWidget->isVisible()){
	int column = 3;
    double total = 0 ;
    for(int row = 0; row < ui->tableWidget->rowCount(); row++){
        total +=  ui->tableWidget->item(row,column)->text().toDouble();
    }
	ui->total_items->setText(QString::number(total));
		
	}
}
void sell_win::save(){
    int rowCount = ui->tableWidget_2->rowCount();
    int rowCount_g= ui->tableWidget->rowCount();
    int x = 0;
    QString rec_no = ui->rec_no->text();
    QString custom_id = model14->index(ui->custom_name->currentIndex(),1).data().toString();
    QString discount = ui->dicount->text();
    QString total = ui->total_after->text();
    QString total_before =ui->total_before->text();
    QString cashier_id = model10->index(ui->cash_name->currentIndex(),1).data().toString();
    QString stock_id = model2->index(ui->stock_name->currentIndex(),1).data().toString();
    QString branch_id = model12->index(ui->branch_name->currentIndex(),1).data().toString();
    QString seller_id = model3->index(ui->seller_name->currentIndex(),1).data().toString();
    QString cashieer_id = model11->index(ui->cashier_name->currentIndex(),1).data().toString();
    QString currency_id = model13->index(ui->currency_name->currentIndex(),1).data().toString();
    QString payment;
    if(ui->cash->isChecked()){
      payment = "Cash";

    }
    else if(ui->credit->isChecked()){
       payment = "Credit";
    }
    else if ( ui->credit_card->isChecked()) {
       payment = "Credit card";
    }

    QString quan;
    QString price;
    QString total_per;
    QString discount_per;
    QString barcode;
    QString item_name;
    QString int_code;
    if(ui->tableWidget_2->isVisible()){
        if(ui->rec_stat->currentIndex() == 0){
            int recCount=0;
             QSqlQuery qry2(db);
             qry2.prepare("SELECT * FROM sales WHERE rec_no = :rec_no");
             qry2.bindValue(":rec_no",rec_no);
             if( qry2.exec() ){
                 while (qry2.next())
                 {
                     recCount++;
                 }
             }
             else{
                   qDebug() << "Error inserting into the table:\n" << qry2.lastError();
            }
        if(recCount==0){
        if(ui->cash->isChecked()){
        QSqlQuery sales_csh(db);
        sales_csh.prepare("INSERT INTO sales(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,0,:cashier,:seller,:currency)");
        sales_csh.bindValue(":rec_no",rec_no);
        sales_csh.bindValue(":custom_id",custom_id);
        sales_csh.bindValue(":discount",discount);
        sales_csh.bindValue(":total_before",total_before);
        sales_csh.bindValue(":discount",discount);
        sales_csh.bindValue(":cashier_id",cashier_id);
        sales_csh.bindValue(":stock_id",stock_id);
        sales_csh.bindValue(":branch_id",branch_id);
        sales_csh.bindValue(":payment",payment);
        sales_csh.bindValue(":total",total);
        sales_csh.bindValue(":cashier",cashieer_id);
        sales_csh.bindValue(":seller",seller_id);
        sales_csh.bindValue(":currency",currency_id);
        if(!sales_csh.exec())
            qDebug()<<sales_csh.lastError();
        }
        else if(ui->credit->isChecked()){
            QSqlQuery sales_crdt(db);
            sales_crdt.prepare("INSERT INTO sales(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,1,:cashier,:seller,:currency)");
            sales_crdt.bindValue(":rec_no",rec_no);
            sales_crdt.bindValue(":custom_id",custom_id);
            sales_crdt.bindValue(":discount",discount);
            sales_crdt.bindValue(":total_before",total_before);
            sales_crdt.bindValue(":discount",discount);
            sales_crdt.bindValue(":cashier_id",cashier_id);
            sales_crdt.bindValue(":stock_id",stock_id);
            sales_crdt.bindValue(":branch_id",branch_id);
            sales_crdt.bindValue(":payment",payment);
            sales_crdt.bindValue(":total",total);
            sales_crdt.bindValue(":cashier",cashieer_id);
            sales_crdt.bindValue(":seller",seller_id);
            sales_crdt.bindValue(":currency",currency_id);
            if(!sales_crdt.exec())
                qDebug()<<sales_crdt.lastError();
            if(custom_id!=10001){
            QSqlQuery custom_bala(db);
            custom_bala.prepare("UPDATE customers SET balance = balance + :total WHERE id = :custom_id");
            custom_bala.bindValue(":custom_id",custom_id);
            custom_bala.bindValue(":total",total);
            if(!custom_bala.exec())
                qDebug()<<custom_bala.lastError();}
        }
        else if(ui->credit_card->isChecked()){
            QSqlQuery sales_crdt_crd(db);
            sales_crdt_crd.prepare("INSERT INTO sales(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,2,:cashier,:seller,:currency)");
            sales_crdt_crd.bindValue(":rec_no",rec_no);
            sales_crdt_crd.bindValue(":custom_id",custom_id);
            sales_crdt_crd.bindValue(":discount",discount);
            sales_crdt_crd.bindValue(":total_before",total_before);
            sales_crdt_crd.bindValue(":discount",discount);
            sales_crdt_crd.bindValue(":cashier_id",cashier_id);
            sales_crdt_crd.bindValue(":stock_id",stock_id);
            sales_crdt_crd.bindValue(":branch_id",branch_id);
            sales_crdt_crd.bindValue(":payment",payment);
            sales_crdt_crd.bindValue(":total",total);
            sales_crdt_crd.bindValue(":cashier",cashieer_id);
            sales_crdt_crd.bindValue(":seller",seller_id);
            sales_crdt_crd.bindValue(":currency",currency_id);
            if(!sales_crdt_crd.exec())
                qDebug()<<sales_crdt_crd.lastError();}
    while(rowCount>x){
        item_name = ui->tableWidget_2->item(x,0)->text();
        int_code = ui->tableWidget_2->item(x,1)->text();
        barcode = ui->tableWidget_2->item(x,2)->text();
        quan = ui->tableWidget_2->item(x,3)->text();
        price = ui->tableWidget_2->item(x,4)->text();
        total_per = ui->tableWidget_2->item(x,6)->text();
        discount_per = ui->tableWidget_2->item(x,5)->text();      
            

        QSqlQuery salesb(db);
             salesb.prepare("INSERT INTO sales_items(rec_no,item_name,item_int,exp_barcode,quantity,price,discount_per,total)VALUES( :sales_id, :item_name, :item_int, :barcode, :quan, :price, :discount, :total)");
             salesb.bindValue(":sales_id",rec_no);
             salesb.bindValue(":item_name",item_name);
             salesb.bindValue(":item_int",int_code);
             salesb.bindValue(":barcode",barcode);
             salesb.bindValue(":quan",quan);
             salesb.bindValue(":price",price);
             salesb.bindValue(":discount",discount_per);
             salesb.bindValue(":total",total_per);
                         if(!salesb.exec())
                            qDebug()<<salesb.lastError();
		QSqlQuery bala2(db);
            bala2.prepare("UPDATE item_balance SET item_balance = item_balance - :quan ,sales_balance = sales_balance + :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
            bala2.bindValue(":ex_bcode",barcode);
            bala2.bindValue(":quan",quan);
            bala2.bindValue(":stock",stock_id);
            bala2.bindValue(":branch",branch_id);
            if(!bala2.exec())
                qDebug()<<bala2.lastError();				
      x++;

     }
   }
 }
         else if (ui->rec_stat->currentIndex() == 1) {
            int recCount=0;
             QSqlQuery qry2(db);
             qry2.prepare("SELECT * FROM sales_return WHERE rec_no = :rec_no");
             qry2.bindValue(":rec_no",rec_no);
             if( qry2.exec() ){
                 while (qry2.next())
                 {
                     recCount++;
                 }
             }
             else{
                   qDebug() << "Error inserting into the table:\n" << qry2.lastError();
            }
             if(recCount==0){
            if(ui->cash->isChecked()){

            QSqlQuery salesrt_csh(db);
            salesrt_csh.prepare("INSERT INTO sales_return(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,0,:cashier,:seller,:currency)");
            salesrt_csh.bindValue(":rec_no",rec_no);
            salesrt_csh.bindValue(":custom_id",custom_id);
            salesrt_csh.bindValue(":discount",discount);
            salesrt_csh.bindValue(":total_before",total_before);
            salesrt_csh.bindValue(":discount",discount);
            salesrt_csh.bindValue(":cashier_id",cashier_id);
            salesrt_csh.bindValue(":stock_id",stock_id);
            salesrt_csh.bindValue(":branch_id",branch_id);
            salesrt_csh.bindValue(":payment",payment);
            salesrt_csh.bindValue(":total",total);
            salesrt_csh.bindValue(":cashier",cashieer_id);
            salesrt_csh.bindValue(":seller",seller_id);
            salesrt_csh.bindValue(":currency",currency_id);
            if(!salesrt_csh.exec())
                qDebug()<<salesrt_csh.lastError();
            }
            else if(ui->credit->isChecked()){

                QSqlQuery salesrt_crdt(db);
                salesrt_crdt.prepare("INSERT INTO sales_return(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,1,:cashier,:seller,:currency)");
                salesrt_crdt.bindValue(":rec_no",rec_no);
                salesrt_crdt.bindValue(":custom_id",custom_id);
                salesrt_crdt.bindValue(":discount",discount);
                salesrt_crdt.bindValue(":total_before",total_before);
                salesrt_crdt.bindValue(":discount",discount);
                salesrt_crdt.bindValue(":cashier_id",cashier_id);
                salesrt_crdt.bindValue(":stock_id",stock_id);
                salesrt_crdt.bindValue(":branch_id",branch_id);
                salesrt_crdt.bindValue(":payment",payment);
                salesrt_crdt.bindValue(":total",total);
                salesrt_crdt.bindValue(":cashier",cashieer_id);
                salesrt_crdt.bindValue(":seller",seller_id);
                salesrt_crdt.bindValue(":currency",currency_id);
                if(!salesrt_crdt.exec())
                    qDebug()<<salesrt_crdt.lastError();
                if(custom_id!=10001){
                QSqlQuery custom_bala(db);
                custom_bala.prepare("UPDATE customers SET balance = balance - :total WHERE id = :custom_id");
                custom_bala.bindValue(":custom_id",custom_id);
                custom_bala.bindValue(":total",total);
                if(!custom_bala.exec())
                    qDebug()<<custom_bala.lastError();}
            }
            else if(ui->credit_card->isChecked()){

                QSqlQuery salesrt_crdt_crd(db);
                salesrt_crdt_crd.prepare("INSERT INTO sales_return(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,2,:cashier,:seller,:currency)");
                salesrt_crdt_crd.bindValue(":rec_no",rec_no);
                salesrt_crdt_crd.bindValue(":custom_id",custom_id);
                salesrt_crdt_crd.bindValue(":discount",discount);
                salesrt_crdt_crd.bindValue(":total_before",total_before);
                salesrt_crdt_crd.bindValue(":discount",discount);
                salesrt_crdt_crd.bindValue(":cashier_id",cashier_id);
                salesrt_crdt_crd.bindValue(":stock_id",stock_id);
                salesrt_crdt_crd.bindValue(":branch_id",branch_id);
                salesrt_crdt_crd.bindValue(":payment",payment);
                salesrt_crdt_crd.bindValue(":total",total);
                salesrt_crdt_crd.bindValue(":cashier",cashieer_id);
                salesrt_crdt_crd.bindValue(":seller",seller_id);
                salesrt_crdt_crd.bindValue(":currency",currency_id);
                if(!salesrt_crdt_crd.exec())
                    qDebug()<<salesrt_crdt_crd.lastError();
            }
            while(rowCount>x){
                item_name = ui->tableWidget_2->item(x,0)->text();
                int_code = ui->tableWidget_2->item(x,1)->text();
                barcode = ui->tableWidget_2->item(x,2)->text();
                quan = ui->tableWidget_2->item(x,3)->text();
                price = ui->tableWidget_2->item(x,4)->text();
                total_per = ui->tableWidget_2->item(x,6)->text();
                discount_per = ui->tableWidget_2->item(x,5)->text();
                         QSqlQuery bala2(db);
                         bala2.prepare("UPDATE item_balance SET item_balance = item_balance + :quan ,sales_return_balance = sales_return_balance + :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
                         bala2.bindValue(":ex_bcode",barcode);
                         bala2.bindValue(":quan",quan);
                         bala2.bindValue(":stock",stock_id);
                         bala2.bindValue(":branch",branch_id);
                          if(!bala2.exec())
                            qDebug()<<bala2.lastError();


                          QSqlQuery sales_return(db);
							sales_return.prepare("INSERT INTO sales_return_items(rec_no,item_name,item_int,exp_barcode,quantity,price,discount_per,total)VALUES( :sales_id, :item_name, :item_int, :barcode, :quan, :price, :discount, :total)");
							sales_return.bindValue(":sales_id",rec_no);
							sales_return.bindValue(":item_name",item_name);
							sales_return.bindValue(":item_int",int_code);
							sales_return.bindValue(":barcode",barcode);
							sales_return.bindValue(":quan",quan);
							sales_return.bindValue(":price",price);
							sales_return.bindValue(":discount",discount_per);
							sales_return.bindValue(":total",total_per);
                         if(!sales_return.exec())
                            qDebug()<<sales_return.lastError();
                           x++;
            }
          }
        }       
    }
    else if(ui->tableWidget->isVisible()){
            if(ui->rec_stat->currentIndex() == 0){
                int recCount=0;
                 QSqlQuery qry2(db);
                 qry2.prepare("SELECT * FROM sales WHERE rec_no = :rec_no");
                 qry2.bindValue(":rec_no",rec_no);
                 if( qry2.exec() ){
                     while (qry2.next())
                     {
                         recCount++;
                     }
                 }
                 else{
                       qDebug() << "Error inserting into the table:\n" << qry2.lastError();
                }
                 if(recCount==0){
                     if(ui->cash->isChecked()){

                     QSqlQuery sales_csh(db);
                        sales_csh.prepare("INSERT INTO sales(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,0,:cashier,:seller,:currency)");
						sales_csh.bindValue(":rec_no",rec_no);
						sales_csh.bindValue(":custom_id",custom_id);
						sales_csh.bindValue(":discount",discount);
						sales_csh.bindValue(":total_before",total_before);
						sales_csh.bindValue(":discount",discount);
						sales_csh.bindValue(":cashier_id",cashier_id);
						sales_csh.bindValue(":stock_id",stock_id);
						sales_csh.bindValue(":branch_id",branch_id);
						sales_csh.bindValue(":payment",payment);
						sales_csh.bindValue(":total",total);
                        sales_csh.bindValue(":cashier",cashieer_id);
                        sales_csh.bindValue(":seller",seller_id);
                        sales_csh.bindValue(":currency",currency_id);
						if(!sales_csh.exec())
							qDebug()<<sales_csh.lastError();
						}
                        else if(ui->credit->isChecked()){
							QSqlQuery sales_crdt(db);
                            sales_crdt.prepare("INSERT INTO sales(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,1,:cashier,:seller,:currency)");
							sales_crdt.bindValue(":rec_no",rec_no);
							sales_crdt.bindValue(":custom_id",custom_id);
							sales_crdt.bindValue(":discount",discount);
							sales_crdt.bindValue(":total_before",total_before);
							sales_crdt.bindValue(":discount",discount);
							sales_crdt.bindValue(":cashier_id",cashier_id);
							sales_crdt.bindValue(":stock_id",stock_id);
							sales_crdt.bindValue(":branch_id",branch_id);
							sales_crdt.bindValue(":payment",payment);
							sales_crdt.bindValue(":total",total);
                            sales_crdt.bindValue(":cashier",cashieer_id);
                            sales_crdt.bindValue(":seller",seller_id);
                            sales_crdt.bindValue(":currency",currency_id);
							if(!sales_crdt.exec())
								qDebug()<<sales_crdt.lastError();
							if(custom_id!=10001){
							QSqlQuery custom_bala(db);
							custom_bala.prepare("UPDATE customers SET balance = balance + :total WHERE id = :custom_id");
							custom_bala.bindValue(":custom_id",custom_id);
							custom_bala.bindValue(":total",total);
							if(!custom_bala.exec())
								qDebug()<<custom_bala.lastError();}
						}
                        else if(ui->credit_card->isChecked()){
							QSqlQuery sales_crdt_crd(db);
                            sales_crdt_crd.prepare("INSERT INTO sales(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,2,:cashier,:seller,:currency)");
							sales_crdt_crd.bindValue(":rec_no",rec_no);
							sales_crdt_crd.bindValue(":custom_id",custom_id);
							sales_crdt_crd.bindValue(":discount",discount);
							sales_crdt_crd.bindValue(":total_before",total_before);
							sales_crdt_crd.bindValue(":discount",discount);
							sales_crdt_crd.bindValue(":cashier_id",cashier_id);
							sales_crdt_crd.bindValue(":stock_id",stock_id);
							sales_crdt_crd.bindValue(":branch_id",branch_id);
							sales_crdt_crd.bindValue(":payment",payment);
							sales_crdt_crd.bindValue(":total",total);
                            sales_crdt_crd.bindValue(":cashier",cashieer_id);
                            sales_crdt_crd.bindValue(":seller",seller_id);
                            sales_crdt_crd.bindValue(":currency",currency_id);
							if(!sales_crdt_crd.exec())
								qDebug()<<sales_crdt_crd.lastError();}
			
			
                     while(rowCount_g>x){
                         item_name = ui->tableWidget->item(x,0)->text();
                         int_code = ui->tableWidget->item(x,1)->text();
                         barcode = ui->tableWidget->item(x,2)->text();
                         quan = ui->tableWidget->item(x,3)->text();
                         price = ui->tableWidget->item(x,4)->text();
                         total_per = ui->tableWidget->item(x,6)->text();
                         discount_per = ui->tableWidget->item(x,5)->text();
                     QSqlQuery bala2(db);
                     bala2.prepare("UPDATE item_balance SET item_balance = item_balance - :quan ,sales_balance = sales_balance + :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
                     bala2.bindValue(":ex_bcode",barcode);
                     bala2.bindValue(":quan",quan);
                     bala2.bindValue(":stock",stock_id);
                     bala2.bindValue(":branch",branch_id);
                     if(!bala2.exec())
                         qDebug()<<bala2.lastError();
                                QSqlQuery salesb(db);
								 salesb.prepare("INSERT INTO sales_items(rec_no,item_name,item_int,exp_barcode,quantity,price,discount_per,total)VALUES( :sales_id, :item_name, :item_int, :barcode, :quan, :price, :discount, :total)");
								 salesb.bindValue(":sales_id",rec_no);
								 salesb.bindValue(":item_name",item_name);
								 salesb.bindValue(":item_int",int_code);
								 salesb.bindValue(":barcode",barcode);
								 salesb.bindValue(":quan",quan);
								 salesb.bindValue(":price",price);
								 salesb.bindValue(":discount",discount_per);
								 salesb.bindValue(":total",total_per);
											 if(!salesb.exec())
                            qDebug()<<salesb.lastError();
                                 x++;
                     }
                            QSqlQuery qry5(db);
                            qry5.prepare("Delete from sales_g where rec_no = "+ui->rec_no->text()+" ");
                            if (qry5.exec())
                            { }
                            QSqlQuery qry6(db);
                            qry6.prepare("Delete from sales_items_g where rec_no = "+ui->rec_no->text()+" ");
                            if (qry6.exec())
                            { }
                 }
                 //
            }



            else if (ui->rec_stat->currentIndex() == 1) {
                int recCount=0;
                 QSqlQuery qry2(db);
                 qry2.prepare("SELECT * FROM sales_return WHERE rec_no = :rec_no");
                 qry2.bindValue(":rec_no",rec_no);
                 if( qry2.exec() ){
                     while (qry2.next())
                     {
                         recCount++;
                     }
                 }
                 else{
                       qDebug() << "Error inserting into the table:\n" << qry2.lastError();
                }
                 if(recCount==0){
                     if(ui->cash->isChecked()){

                                 QSqlQuery salesrt_csh(db);
            salesrt_csh.prepare("INSERT INTO sales_return(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,0,:cashier,:seller,:currency)");
            salesrt_csh.bindValue(":rec_no",rec_no);
            salesrt_csh.bindValue(":custom_id",custom_id);
            salesrt_csh.bindValue(":discount",discount);
            salesrt_csh.bindValue(":total_before",total_before);
            salesrt_csh.bindValue(":discount",discount);
            salesrt_csh.bindValue(":cashier_id",cashier_id);
            salesrt_csh.bindValue(":stock_id",stock_id);
            salesrt_csh.bindValue(":branch_id",branch_id);
            salesrt_csh.bindValue(":payment",payment);
            salesrt_csh.bindValue(":total",total);
            salesrt_csh.bindValue(":cashier",cashieer_id);
            salesrt_csh.bindValue(":seller",seller_id);
            salesrt_csh.bindValue(":currency",currency_id);
            if(!salesrt_csh.exec())
                qDebug()<<salesrt_csh.lastError();
            }
            else if(ui->credit->isChecked()){

                QSqlQuery salesrt_crdt(db);
                salesrt_crdt.prepare("INSERT INTO sales_return(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,1,:cashier,:seller,:currency)");
                salesrt_crdt.bindValue(":rec_no",rec_no);
                salesrt_crdt.bindValue(":custom_id",custom_id);
                salesrt_crdt.bindValue(":discount",discount);
                salesrt_crdt.bindValue(":total_before",total_before);
                salesrt_crdt.bindValue(":discount",discount);
                salesrt_crdt.bindValue(":cashier_id",cashier_id);
                salesrt_crdt.bindValue(":stock_id",stock_id);
                salesrt_crdt.bindValue(":branch_id",branch_id);
                salesrt_crdt.bindValue(":payment",payment);
                salesrt_crdt.bindValue(":total",total);
                salesrt_crdt.bindValue(":cashier",cashieer_id);
                salesrt_crdt.bindValue(":seller",seller_id);
                salesrt_crdt.bindValue(":currency",currency_id);
                if(!salesrt_crdt.exec())
                    qDebug()<<salesrt_crdt.lastError();
                if(custom_id!=10001){
                QSqlQuery custom_bala(db);
                custom_bala.prepare("UPDATE customers SET balance = balance - :total WHERE id = :custom_id");
                custom_bala.bindValue(":custom_id",custom_id);
                custom_bala.bindValue(":total",total);
                if(!custom_bala.exec())
                    qDebug()<<custom_bala.lastError();}
            }
            else if(ui->credit_card->isChecked()){

                QSqlQuery salesrt_crdt_crd(db);
                salesrt_crdt_crd.prepare("INSERT INTO sales_return(rec_no,customer_id,discount,total_before,cash_deposit_id,storage_id,branch_id,payment_method,total,payment_method_id,cashier_id,seller_id,currency_id)VALUES(:rec_no,:custom_id,:discount,:total_before,:cashier_id,:stock_id,:branch_id,:payment,:total,2,:cashier,:seller,:currency)");
                salesrt_crdt_crd.bindValue(":rec_no",rec_no);
                salesrt_crdt_crd.bindValue(":custom_id",custom_id);
                salesrt_crdt_crd.bindValue(":discount",discount);
                salesrt_crdt_crd.bindValue(":total_before",total_before);
                salesrt_crdt_crd.bindValue(":discount",discount);
                salesrt_crdt_crd.bindValue(":cashier_id",cashier_id);
                salesrt_crdt_crd.bindValue(":stock_id",stock_id);
                salesrt_crdt_crd.bindValue(":branch_id",branch_id);
                salesrt_crdt_crd.bindValue(":payment",payment);
                salesrt_crdt_crd.bindValue(":total",total);
                salesrt_crdt_crd.bindValue(":cashier",cashieer_id);
                salesrt_crdt_crd.bindValue(":seller",seller_id);
                salesrt_crdt_crd.bindValue(":currency",currency_id);
                if(!salesrt_crdt_crd.exec())
                    qDebug()<<salesrt_crdt_crd.lastError();
            }
                     while(rowCount_g>x){
                         item_name = ui->tableWidget->item(x,0)->text();
                         int_code = ui->tableWidget->item(x,1)->text();
                         barcode = ui->tableWidget->item(x,2)->text();
                         quan = ui->tableWidget->item(x,3)->text();
                         price = ui->tableWidget->item(x,4)->text();
                         total_per = ui->tableWidget->item(x,6)->text();
                         discount_per = ui->tableWidget->item(x,5)->text();
                                 QSqlQuery bala2(db);
                                 bala2.prepare("UPDATE item_balance SET item_balance = item_balance + :quan ,sales_return_balance = sales_return_balance + :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
                                 bala2.bindValue(":ex_bcode",barcode);
                                 bala2.bindValue(":quan",quan);
                                 bala2.bindValue(":stock",stock_id);
                                 bala2.bindValue(":branch",branch_id);
                                 if(!bala2.exec())
                                     qDebug()<<bala2.lastError();

                                 QSqlQuery sales_return(db);
                                sales_return.prepare("INSERT INTO sales_return_items(rec_no,item_name,item_int,exp_barcode,quantity,price,discount_per,total)VALUES( :sales_id, :item_name, :item_int, :barcode, :quan, :price, :discount, :total)");
                                sales_return.bindValue(":sales_id",rec_no);
                                sales_return.bindValue(":item_name",item_name);
                                sales_return.bindValue(":item_int",int_code);
                                sales_return.bindValue(":barcode",barcode);
                                sales_return.bindValue(":quan",quan);
                                sales_return.bindValue(":price",price);
                                sales_return.bindValue(":discount",discount_per);
                                sales_return.bindValue(":total",total_per);
                                if(!sales_return.exec())
                                qDebug()<<sales_return.lastError();
                                x++;
                            
                     }
							QSqlQuery qry5(db);
                            qry5.prepare("Delete from sales_return_g where rec_no = "+ui->rec_no->text()+" ");
                            if (qry5.exec())
                            { }
                            QSqlQuery qry6(db);
                            qry6.prepare("Delete from sales_return_items_g where rec_no = "+ui->rec_no->text()+" ");
                            if (qry6.exec())
                            { }

          }
    }   
}
    clearAll();
}
void sell_win::print_recp(){
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
    if(ui->tableWidget_2->isVisible()&&ui->tableWidget_2->rowCount()!=0){
    QtRPT *report = new QtRPT(this);
    report->loadReport(":/Rc/Xml/rec.xml");
    report->recordCount.append(ui->tableWidget_2->rowCount());
    QString quan;
    QString price;
    QString total_per;
    QString discount_per;
    QString item_name;
    QString rec = ui->rec_no->text();
    QString custom = ui->custom_name->lineEdit()->text();
    QString cash = ui->cash_name->lineEdit()->text();
    QString total = ui->total_after->text();
    QString discount = ui->dicount->text();
    QString total_before = ui->total_before->text();


       connect(report, &QtRPT::setValue, [&](const int recNo,
                   const QString paramName, QVariant &paramValue,
                   const int reportPage) {
                   (void) reportPage;

               if (paramName == "recNo") {
                   paramValue = rec;
               }
               if (paramName == "custom") {
                   paramValue = custom;
               }
               if (paramName == "cashier") {
                   paramValue = cash;
               }
               if (paramName == "name") {
                   paramValue = ui->tableWidget_2->item(recNo,0)->text();;
               }
               if (paramName == "quan") {
                   paramValue = ui->tableWidget_2->item(recNo,3)->text();;
               }
               if (paramName == "price") {
                   paramValue = ui->tableWidget_2->item(recNo,6)->text();;
               }
               if (paramName == "total") {
                   paramValue = total;
               }
               if (paramName == "discount") {
                   paramValue = discount;
               }
               if (paramName == "total_before") {
                   paramValue = total_before;
               }

       });

    report->printExec(true,true,printer_name);}
    else if(ui->tableWidget->isVisible()&&ui->tableWidget->rowCount()!=0){
        QtRPT *report = new QtRPT(this);
        report->loadReport(":/Rc/Xml/rec.xml");
        report->recordCount.append(ui->tableWidget->rowCount());
        QString quan;
        QString price;
        QString total_per;
        QString discount_per;
        QString item_name;
        QString rec = ui->rec_no->text();
        QString custom = ui->custom_name->lineEdit()->text();
        QString cash = ui->cash_name->lineEdit()->text();
        QString total = ui->total_after->text();
        QString discount = ui->dicount->text();
        QString total_before = ui->total_before->text();


           connect(report, &QtRPT::setValue, [&](const int recNo,
                       const QString paramName, QVariant &paramValue,
                       const int reportPage) {
                       (void) reportPage;

                   if (paramName == "recNo") {
                       paramValue = rec;
                   }
                   if (paramName == "custom") {
                       paramValue = custom;
                   }
                   if (paramName == "cashier") {
                       paramValue = cash;
                   }
                   if (paramName == "name") {
                       paramValue = ui->tableWidget->item(recNo,0)->text();;
                   }
                   if (paramName == "quan") {
                       paramValue = ui->tableWidget->item(recNo,3)->text();;
                   }
                   if (paramName == "price") {
                       paramValue = ui->tableWidget->item(recNo,6)->text();;
                   }
                   if (paramName == "total") {
                       paramValue = total;
                   }
                   if (paramName == "discount") {
                       paramValue = discount;
                   }
                   if (paramName == "total_before") {
                       paramValue = total_before;
                   }

           });

        report->printExec(true,true,printer_name);

    }
    else if(!ui->tableView->isHidden()){
        QtRPT *report = new QtRPT(this);
        report->loadReport(":/Rc/Xml/rec.xml");
        QString quan;
        QString price;
        QString total_per;
        QString discount_per;
        QString item_name;
        QString rec = ui->rec_no->text();
        QString custom = ui->custom_name->lineEdit()->text();
        QString cash = ui->cash_name->lineEdit()->text();
        QString total = ui->total_after->text();
        QString discount = ui->dicount->text();
        QString total_before = ui->total_before->text();
        report->recordCount.append(ui->tableView->model()->rowCount());

        connect(report, &QtRPT::setValue, [&](const int recNo,
                    const QString paramName, QVariant &paramValue,
                    const int reportPage) {
                    (void) reportPage;

                if (paramName == "recNo") {
                    paramValue = rec;
                }
                if (paramName == "custom") {
                    paramValue = custom;
                }
                if (paramName == "cashier") {
                    paramValue = cash;
                }
                if (paramName == "name") {
                    paramValue = ui->tableView->model()->index(recNo,2).data();
                }
                if (paramName == "quan") {
                    paramValue = ui->tableView->model()->index(recNo,6).data();
                }
                if (paramName == "price") {
                    paramValue = ui->tableView->model()->index(recNo,5).data();
                }
                if (paramName == "total") {
                    paramValue = total;
                }
                if (paramName == "discount") {
                    paramValue = discount;
                }
                if (paramName == "total_before") {
                    paramValue = total_before;
                }
        });

        report->printExec(true,true,printer_name);
    }
    else if(!ui->tableView_2->isHidden()){
        QtRPT *report = new QtRPT(this);
        report->loadReport(":/Rc/Xml/rec.xml");
        QString quan;
        QString price;
        QString total_per;
        QString discount_per;
        QString item_name;
        QString rec = ui->rec_no->text();
        QString custom = ui->custom_name->lineEdit()->text();
        QString cash = ui->cash_name->lineEdit()->text();
        QString total = ui->total_after->text();
        QString discount = ui->dicount->text();
        QString total_before = ui->total_before->text();

        report->recordCount.append(ui->tableView_2->model()->rowCount());

        connect(report, &QtRPT::setValue, [&](const int recNo,
                    const QString paramName, QVariant &paramValue,
                    const int reportPage) {
                    (void) reportPage;

                if (paramName == "recNo") {
                    paramValue = rec;
                }
                if (paramName == "custom") {
                    paramValue = custom;
                }
                if (paramName == "cashier") {
                    paramValue = cash;
                }
                if (paramName == "name") {
                    paramValue = ui->tableView_2->model()->index(recNo,2).data();
                }
                if (paramName == "quan") {
                    paramValue = ui->tableView_2->model()->index(recNo,6).data();
                }
                if (paramName == "price") {
                    paramValue = ui->tableView_2->model()->index(recNo,5).data();
                }
                if (paramName == "total") {
                    paramValue = total;
                }
                if (paramName == "discount") {
                    paramValue = discount;
                }
                if (paramName == "total_before") {
                    paramValue = total_before;
                }

        });

        report->printExec(true,true,printer_name);
    }

}
void sell_win::print_save(){
    print_recp();
    save();
}


void sell_win::on_rec_stat_currentIndexChanged(int index)
{

    int saved_count;
    int unsaved_count;
    if(index==0){
        if(ui->tableWidget_2->isVisible()){
        qDebug()<<"Sales";
        db.close();
        db.open();
        QSqlQuery qry(db);
        qry.prepare("SELECT MAX(rec_no) from sales");
        if (qry.exec())
        {
            while (qry.next())
            {
              saved_count = qry.value(0).toInt();
            }
        }
        QSqlQuery qry2(db);
        qry2.prepare("SELECT MAX(rec_no) from sales_g");
        if (qry2.exec())
        {
            while (qry2.next())
            {
              unsaved_count = qry2.value(0).toInt();
            }
        }
        if(saved_count>unsaved_count||(saved_count&&unsaved_count)==0){
            saved_count+=1;
            ui->rec_no->setText(QString::number(saved_count));
        }
        else if(unsaved_count>saved_count){
            unsaved_count+=1;
            ui->rec_no->setText(QString::number(unsaved_count));
        }
    }
        else if(ui->tableView->isVisible()||ui->tableView_2->isVisible()){
            if(ui->tableView_2->isVisible()){
                ui->tableView_2->hide();
                ui->tableView->show();

            }
            db.close();
            db.open();
            QSqlQuery qry(db);
            int test7;
            QString test8;
            qry.prepare("SELECT MAX(rec_no) from sales");
            if (qry.exec())
            {
                while (qry.next())
                {
                    test7 = qry.value(0).toInt();
                    test8 = QString::number(test7);
                    ui->rec_no->setText(test8);

                }
            }

        }
       else if (ui->tableWidget->isVisible()) {
            db.close();
            db.open();
            ui->tableWidget->clear();
            QSqlQuery qry(db);
            int test7;
            QString test8;
            qry.prepare("SELECT MAX(rec_no) from sales_g");
            if (qry.exec())
            {
                while (qry.next())
                {
                    test7 = qry.value(0).toInt();
                    test8 = QString::number(test7);
                    ui->rec_no->setText(test8);

                }
            }


        }
    }
    else if (index==1) {
     if(ui->tableWidget_2->isVisible()){
     qDebug()<<"Salesrt";
     db.close();
     db.open();
     QSqlQuery qry(db);
     qry.prepare("SELECT MAX(rec_no) from sales_return");
     if (qry.exec())
     {
         while (qry.next())
         {
           saved_count = qry.value(0).toInt();
         }
     }
     QSqlQuery qry2(db);
     qry2.prepare("SELECT MAX(rec_no) from sales_return_g");
     if (qry2.exec())
     {
         while (qry2.next())
         {
           unsaved_count = qry2.value(0).toInt();
         }
     }
     if(saved_count>unsaved_count||(saved_count&&unsaved_count)==0){
         saved_count+=1;
         ui->rec_no->setText(QString::number(saved_count));
     }
     else if(unsaved_count>saved_count){
         unsaved_count+=1;
         ui->rec_no->setText(QString::number(unsaved_count));
     }
  }
     else if (ui->tableView_2->isVisible()||ui->tableView->isVisible()){
         if(ui->tableView->isVisible()){
             ui->tableView->hide();
             ui->tableView_2->show();
         }
         db.close();
         db.open();
         QSqlQuery qry(db);
         qry.prepare("SELECT MAX(rec_no) from sales_return");
         if (qry.exec())
         {
             while (qry.next())
             {
                 int test7;
                 test7 = qry.value(0).toInt();
                 QString test6 = QString::number(test7);
                 ui->rec_no->setText(test6);

             }
         }


     }
     else if (ui->tableWidget->isVisible()) {
         db.close();
         db.open();
         ui->tableWidget->clear();
         QSqlQuery qry(db);
         qry.prepare("SELECT MAX(rec_no) from sales_return_g");
         if (qry.exec())
         {
             while (qry.next())
             {
                 int test7;
                 test7 = qry.value(0).toInt();
                 QString test6 = QString::number(test7);
                 ui->rec_no->setText(test6);

             }
         }
      }
    }
}
void sell_win::set_edit(){
    ui->cashier_name->lineEdit()->setReadOnly(false);
    ui->currency_name->lineEdit()->setReadOnly(false);
    ui->cash_name->lineEdit()->setReadOnly(false);
    ui->sell_price->setReadOnly(false);
    ui->discount_per->setReadOnly(false);
    ui->total_per->setReadOnly(false);
    ui->total_before->setReadOnly(false);
    ui->total_items->setReadOnly(false);
    ui->total_after->setReadOnly(false);
    ui->discount_perc->setReadOnly(false);
    ui->dicount->setReadOnly(false);
    ui->custom_name->lineEdit()->setReadOnly(false);
    ui->branch_name->lineEdit()->setReadOnly(false);
    ui->seller_name->lineEdit()->setReadOnly(false);
    ui->stock_name->lineEdit()->setReadOnly(false);
    ui->dateTimeEdit->setReadOnly(false);
    ui->item_name->lineEdit()->setReadOnly(false);
    ui->ex_bcode->lineEdit()->setReadOnly(false);
    ui->item_int->lineEdit()->setReadOnly(false);
    ui->quan->setReadOnly(false);
    ui->cash->setDisabled(false);
    ui->credit->setDisabled(false);
    ui->credit_card->setDisabled(false);
}
void sell_win::set_unedit(){
    ui->cashier_name->lineEdit()->setReadOnly(true);
    ui->currency_name->lineEdit()->setReadOnly(true);
    ui->cash_name->lineEdit()->setReadOnly(true);
    ui->sell_price->setReadOnly(true);
    ui->discount_per->setReadOnly(true);
    ui->total_per->setReadOnly(true);
    ui->total_before->setReadOnly(true);
    ui->total_items->setReadOnly(true);
    ui->total_after->setReadOnly(true);
    ui->discount_perc->setReadOnly(true);
    ui->dicount->setReadOnly(true);
    ui->custom_name->lineEdit()->setReadOnly(true);
    ui->branch_name->lineEdit()->setReadOnly(true);
    ui->seller_name->lineEdit()->setReadOnly(true);
    ui->stock_name->lineEdit()->setReadOnly(true);
    ui->dateTimeEdit->setReadOnly(true);
    ui->item_name->lineEdit()->setReadOnly(true);
    ui->ex_bcode->lineEdit()->setReadOnly(true);
    ui->item_int->lineEdit()->setReadOnly(true);
    ui->quan->setReadOnly(true);
    ui->cash->setDisabled(true);
    ui->credit->setDisabled(true);
    ui->credit_card->setDisabled(true);
}
void sell_win::on_expire_currentIndexChanged(int index)
{
    if(ui->ex_bcode->lineEdit()->text().isEmpty()){
    QString expire = ui->expire->currentText();
    QSqlQuery qry(db);
    qry.prepare("SELECT exp_barcode FROM item_balance WHERE sub_item_id = :id AND expire = :expire");
    qry.bindValue(":id",code_id);
    qry.bindValue(":expire",expire);
    if(qry.exec())
    {
        while (qry.next()) {
            ui->ex_bcode->lineEdit()->setText(qry.value(0).toString());
        }

    }
    else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}
    int bcode = ui->ex_bcode->lineEdit()->text().toInt();
    QString test;
    QSqlQuery qry2(db);
    qry2.prepare("SELECT item_balance FROM item_balance WHERE exp_barcode = :id ");
    qry2.bindValue(":id",bcode);
    if( qry2.exec() ){
        while (qry2.next())
        {
            item_bala = qry2.value(0).toDouble();
        }
    }
    else{qDebug() << "Error inserting into the table:\n" << qry2.lastError();}
    }
    else{
        QString expire = ui->expire->currentText();
        QString code_id1 = ui->ex_bcode->lineEdit()->text();
        int bcode;
        QSqlQuery qry1(db);
        qry1.prepare("SELECT sub_item_id  FROM item_balance WHERE exp_barcode = :id ");
        qry1.bindValue(":id",code_id1);
        if( qry1.exec()){
            while (qry1.next())
            {
               bcode = qry1.value(0).toInt();
            }
        }
        else{qDebug() << "Error inserting into the table:\n" << qry1.lastError();}

        QSqlQuery qry(db);
        qry.prepare("SELECT exp_barcode FROM item_balance WHERE sub_item_id = :id AND expire = :expire");
        qry.bindValue(":id",bcode);
        qry.bindValue(":expire",expire);
        if(qry.exec())
        {
            while (qry.next()) {
                ui->ex_bcode->lineEdit()->setText(qry.value(0).toString());
            }

        }
        else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}
        /*int bcode = ui->ex_bcode->lineEdit()->text().toInt();
        QString test;
        QSqlQuery qry2(db);
        qry2.prepare("SELECT item_balance FROM item_balance WHERE exp_barcode = :id ");
        qry2.bindValue(":id",bcode);
        if( qry2.exec() ){
            while (qry2.next())
            {
                item_bala = qry2.value(0).toDouble();
            }
        }
        else{qDebug() << "Error inserting into the table:\n" << qry2.lastError();}*/
    }
}

void sell_win::on_sell_price_textChanged(const QString &arg1)
{
    double quan = ui->quan->text().toDouble();
    double price = ui->sell_price->text().toDouble();
    double total = quan * price;
    ui->total_per->setText(QString::number(total));
}


void sell_win::on_quan_textChanged(const QString &arg1)
{
    double quan = ui->quan->text().toDouble();
    if(quan<=item_bala){
        double price = ui->sell_price->text().toDouble();
        double total = quan * price;
        ui->total_per->setText(QString::number(total));
    }
    else{
        double price = ui->sell_price->text().toDouble();
        double total = quan * price;
        ui->total_per->setText(QString::number(total));
        qDebug()<<item_bala;
    }

}

void sell_win::on_discount_per_textChanged(const QString &arg1)
{
    double total = ui->quan->text().toDouble() * ui->sell_price->text().toDouble();
    double discount = ui->discount_per->text().toDouble();
    double total_after = total-discount;
     ui->total_per->setText(QString::number(total_after));
}


void sell_win::on_quan_returnPressed()
{
    if(!ui->item_name->lineEdit()->text().isEmpty()){
     setTable();
    }
    else{
        ui->item_name->lineEdit()->setFocus();
    }
}

void sell_win::on_sell_price_returnPressed()
{
    ui->discount_per->setFocus();
    ui->discount_per->selectAll();
}

void sell_win::on_discount_per_returnPressed()
{
    if(!ui->item_name->lineEdit()->text().isEmpty()){
     setTable();
    }
    else{
        ui->item_name->lineEdit()->setFocus();
    }
}


void sell_win::on_save_clicked()
{

  save();

}


void sell_win::on_dicount_returnPressed()
{
    double total_before = ui->total_before->text().toDouble();
    double discount = ui->dicount->text().toDouble();
    double total_after = total_before - discount;
    double perc = (discount / total_before)*100;
    ui->total_after->setText(QString::number(total_after));
    ui->discount_perc->setText(QString::number(perc));
    ui->discount_perc->setFocus();
}

void sell_win::on_discount_perc_editingFinished()
{
    double total_b = ui->total_before->text().toDouble();
    double perc = ui->discount_perc->text().toDouble()/100;
    double disc = perc * total_b;
    double total = total_b - disc;
    ui->total_after->setText(QString::number(total));
    ui->dicount->setText(QString::number(disc));
}

void sell_win::on_discount_perc_returnPressed()
{
    double total_b = ui->total_before->text().toDouble();
    double perc = ui->discount_perc->text().toDouble()/100;
    double disc = perc * total_b;
    double total = total_b - disc;
    ui->total_after->setText(QString::number(total));
    ui->dicount->setText(QString::number(disc));
}

void sell_win::on_total_before_textChanged(const QString &arg1)
{
    double total_before = ui->total_before->text().toDouble();
    double discount = ui->dicount->text().toDouble();
    double total_after = total_before - discount;
    double perc = (discount / total_before)*100;
    ui->total_after->setText(QString::number(total_after));
    ui->discount_perc->setText(QString::number(perc));
}


void sell_win::on_pushButton_6_clicked()
{
    print_recp();
    save();
}

void sell_win::on_ex_bcode_activated(const QString &arg1)
{
    enterBcode();
}

void sell_win::on_item_int_activated(const QString &arg1)
{
    enterInt();
}

void sell_win::on_saved_rec_clicked()
{
    set_unedit();
    if(ui->tableView->isHidden()){
    if(ui->rec_stat->currentIndex()==0){
    QSqlQuery qry(db);
    QSqlQuery qry2(db);
    int test7;
    QString test8;
    QDateTime date;
    QString payment;
    qry.prepare("SELECT MAX(rec_no) from sales");
    if (qry.exec())
    {
        while (qry.next())
        {
            test7 = qry.value(0).toInt();
            test8 = QString::number(test7);

        }
    }
    QSqlTableModel *GetTable = new QSqlTableModel(this,db);
    GetTable->setTable("sales_items");
    GetTable->setFilter("rec_no = "+test8+" ");
    ui->tableWidget_2->hide();
    ui->tableView_2->hide();
    ui->tableView->show();
    ui->tableView->setModel(GetTable);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(9);
    GetTable->select();
    QModelIndex id_in = GetTable->index(0,1);
    QString id = GetTable->data(id_in).toString();
    ui->rec_no->setText(id);
    qry2.prepare("SELECT create_date,payment_method,customer_id,total,discount,cashier_id,storage_id,branch_id,currency_id,seller_id,cash_deposit_id,total_before from sales WHERE rec_no = "+id+" ");
    if (qry2.exec())
    {
        while (qry2.next())
        {
            date = qry2.value(0).toDateTime();
            payment = qry2.value(1).toString();			
            model14->setQuery("SELECT name,id FROM customers WHERE id = "+qry2.value(2).toString()+"",db);
            ui->total_before->setText(qry2.value(11).toString());
            ui->dicount->setText(qry2.value(4).toString());
            model11->setQuery("SELECT name,id FROM employees WHERE appear_as_cashier = 1 AND id ="+qry2.value(5).toString()+"",db);
            model2->setQuery("SELECT name,id FROM storage WHERE id = "+qry2.value(6).toString()+"",db);
            model12->setQuery("SELECT name,id FROM branch WHERE id = "+qry2.value(7).toString()+"",db);
            model13->setQuery("SELECT name,id FROM currency WHERE id = "+qry2.value(8).toString()+"",db);
            model3->setQuery("SELECT name,id FROM employees WHERE id = "+qry2.value(9).toString()+"",db);
            model10->setQuery("SELECT name,id FROM cash_deposit WHERE id = "+qry2.value(10).toString()+"",db);
            ui->total_after->setText(qry2.value(3).toString());
            ui->stock_name->setCurrentIndex(0);
            ui->branch_name->setCurrentIndex(0);
            ui->cash_name->setCurrentIndex(0);
            ui->custom_name->setCurrentIndex(0);
            ui->currency_name->setCurrentIndex(0);
            ui->cashier_name->setCurrentIndex(0);
            ui->seller_name->setCurrentIndex(0);

        }
    }
    ui->dateTimeEdit->setDateTime(date);
    if(payment == "Cash"){
        ui->cash->setChecked(true);

    }
    else if(payment == "Credit"){
        ui->credit->setChecked(true);
    }
    else if (payment == "Credit card") {
        ui->credit_card->setChecked(true);
    }
    }
    }
    if(ui->tableView_2->isHidden()){
     if (ui->rec_stat->currentIndex()==1){
        QSqlQuery qry(db);
        QSqlQuery qry2(db);
        int test7;
        QString test8;
        QDateTime date;
        QString payment;
        qry.prepare("SELECT MAX(rec_no) from sales_return");
        if (qry.exec())
        {
            while (qry.next())
            {
                test7 = qry.value(0).toInt();
                test8 = QString::number(test7);

            }
        }
        QSqlTableModel *GetTable = new QSqlTableModel(this,db);
        GetTable->setTable("sales_return_items");
        GetTable->setFilter("rec_no = "+test8+" ");
        ui->tableWidget_2->hide();
        ui->tableView->hide();
        ui->tableView_2->show();
        ui->tableView_2->setModel(GetTable);
        ui->tableView_2->hideColumn(0);
        ui->tableView_2->hideColumn(1);
        ui->tableView_2->hideColumn(9);
        GetTable->select();
        QModelIndex id_in = GetTable->index(0,1);
        QString id = GetTable->data(id_in).toString();
        ui->rec_no->setText(id);
        qry2.prepare("SELECT create_date,payment_method,customer_id,total,discount,cashier_id,storage_id,branch_id,currency_id,seller_id,cash_deposit_id,total_before from sales_return WHERE rec_no = "+id+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                date = qry2.value(0).toDateTime();
                payment = qry2.value(1).toString();
                model14->setQuery("SELECT name,id FROM customers WHERE id = "+qry2.value(2).toString()+"",db);
                ui->total_before->setText(qry2.value(11).toString());
                ui->dicount->setText(qry2.value(4).toString());
                model11->setQuery("SELECT name,id FROM employees WHERE appear_as_cashier = 1 AND id ="+qry2.value(5).toString()+"",db);
                model2->setQuery("SELECT name,id FROM storage WHERE id = "+qry2.value(6).toString()+"",db);
                model12->setQuery("SELECT name,id FROM branch WHERE id = "+qry2.value(7).toString()+"",db);
                model13->setQuery("SELECT name,id FROM currency WHERE id = "+qry2.value(8).toString()+"",db);
                model3->setQuery("SELECT name,id FROM employees WHERE id = "+qry2.value(9).toString()+"",db);
                model10->setQuery("SELECT name,id FROM cash_deposit WHERE id = "+qry2.value(10).toString()+"",db);
                ui->total_after->setText(qry2.value(3).toString());
                ui->stock_name->setCurrentIndex(0);
                ui->branch_name->setCurrentIndex(0);
                ui->cash_name->setCurrentIndex(0);
                ui->custom_name->setCurrentIndex(0);
                ui->currency_name->setCurrentIndex(0);
                ui->cashier_name->setCurrentIndex(0);
                ui->seller_name->setCurrentIndex(0);

            }
        }
        ui->dateTimeEdit->setDateTime(date);
        if(payment == "Cash"){
            ui->cash->setChecked(true);

        }
        else if(payment == "Credit"){
            ui->credit->setChecked(true);
        }
        else if (payment == "Credit card") {
            ui->credit_card->setChecked(true);
        }
    }
    }

}

void sell_win::on_rec_no_textChanged(const QString &arg1)
{
    if(ui->rec_stat->currentIndex()==0&&!ui->tableView->isHidden()){
        QSqlQuery qry2(db);
        QDateTime date;
        QString payment;
    QSqlTableModel *GetTable = new QSqlTableModel(this,db);
    GetTable->setTable("sales_items");
    GetTable->setFilter("rec_no = "+ui->rec_no->text()+" ");
    ui->tableView->setModel(GetTable);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(9);
    GetTable->select();
    QString id = ui->rec_no->text();
    qry2.prepare("SELECT create_date,payment_method,customer_id,total,discount,cashier_id,storage_id,branch_id,currency_id,seller_id,cash_deposit_id,total_before from sales WHERE rec_no = "+id+" ");
    if (qry2.exec())
    {
        while (qry2.next())
        {
            date = qry2.value(0).toDateTime();
            payment = qry2.value(1).toString();
            model14->setQuery("SELECT name,id FROM customers WHERE id = "+qry2.value(2).toString()+"",db);
            ui->total_before->setText(qry2.value(11).toString());
            ui->dicount->setText(qry2.value(4).toString());
            model11->setQuery("SELECT name,id FROM employees WHERE appear_as_cashier = 1 AND id ="+qry2.value(5).toString()+"",db);
            model2->setQuery("SELECT name,id FROM storage WHERE id = "+qry2.value(6).toString()+"",db);
            model12->setQuery("SELECT name,id FROM branch WHERE id = "+qry2.value(7).toString()+"",db);
            model13->setQuery("SELECT name,id FROM currency WHERE id = "+qry2.value(8).toString()+"",db);
            model3->setQuery("SELECT name,id FROM employees WHERE id = "+qry2.value(9).toString()+"",db);
            model10->setQuery("SELECT name,id FROM cash_deposit WHERE id = "+qry2.value(10).toString()+"",db);
            ui->total_after->setText(qry2.value(3).toString());
            ui->stock_name->setCurrentIndex(0);
            ui->branch_name->setCurrentIndex(0);
            ui->cash_name->setCurrentIndex(0);
            ui->custom_name->setCurrentIndex(0);
            ui->currency_name->setCurrentIndex(0);
            ui->cashier_name->setCurrentIndex(0);
            ui->seller_name->setCurrentIndex(0);

        }
    }
    ui->dateTimeEdit->setDateTime(date);
    if(payment == "Cash"){
        ui->cash->setChecked(true);

    }
    else if(payment == "Credit"){
        ui->credit->setChecked(true);
    }
    else if (payment == "Credit card") {
        ui->credit_card->setChecked(true);
    }
    }
    else if(ui->rec_stat->currentIndex()==0&&ui->tableWidget->isVisible()){
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        QSqlQuery qry1(db);
        QSqlQuery qry2(db);
        QString id = ui->rec_no->text();
        QDateTime date;
        QString payment;
				model14->setQuery("SELECT name,id FROM customers",db);
                model11->setQuery("SELECT name,id FROM employees WHERE appear_as_cashier = 1",db);
                model2->setQuery("SELECT name,id FROM storage",db);
                model12->setQuery("SELECT name,id FROM branch",db);
                model13->setQuery("SELECT name,id FROM currency",db);
                model3->setQuery("SELECT name,id FROM employees",db);
                model10->setQuery("SELECT name,id FROM cash_deposit",db);
        qry1.prepare("SELECT item_name,item_int,exp_barcode,quantity,price,discount_per,total from sales_items_g where rec_no = "+id+" ");
        if (qry1.exec())
        {
            while (qry1.next())
            {
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
                QTableWidgetItem *item_name = new QTableWidgetItem();
                QTableWidgetItem *int_code = new QTableWidgetItem();
                QTableWidgetItem *barcode = new QTableWidgetItem();
                QTableWidgetItem *quan = new QTableWidgetItem();
                QTableWidgetItem *price = new QTableWidgetItem();
                QTableWidgetItem *discount = new QTableWidgetItem();
                QTableWidgetItem *total = new QTableWidgetItem();
                item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);
                int_code->setFlags(int_code->flags() & ~Qt::ItemIsEditable);
                barcode->setFlags(barcode->flags() & ~Qt::ItemIsEditable);
                price->setFlags(price->flags() & ~Qt::ItemIsEditable);
                total->setFlags(total->flags() & ~Qt::ItemIsEditable);
                item_name->setText(qry1.value(0).toString());
                int_code->setText(qry1.value(1).toString());
                barcode->setText(qry1.value(2).toString());
                quan->setText(qry1.value(3).toString());
                price->setText(qry1.value(4).toString());
                discount->setText(qry1.value(5).toString());
                total->setText(qry1.value(6).toString());
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0,item_name);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1,int_code);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2,barcode);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3,quan);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 4,price);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 5,discount);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 6,total);

            }
        }



        qry2.prepare("SELECT create_date,payment_method,customer_id,total,discount,cashier_id,storage_id,branch_id,currency_id,seller_id,cash_deposit_id,total_before from sales_g WHERE rec_no = "+id+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                date = qry2.value(0).toDateTime();
                payment = qry2.value(1).toString();
                ui->total_before->setText(qry2.value(11).toString());
                ui->dicount->setText(qry2.value(4).toString());
                ui->total_after->setText(qry2.value(3).toString());
				
				QList<QModelIndex> modelIndexList;
				modelIndexList.clear();
				modelIndexList = model14->match(model14->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(2).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->custom_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model11->match(model11->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(5).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->cashier_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model2->match(model2->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(6).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->stock_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model12->match(model12->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(7).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->branch_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model13->match(model13->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(8).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->currency_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model3->match(model3->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(9).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->seller_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model10->match(model10->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(10).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->cash_name->setCurrentIndex(modelIndexList.first().row());
				}
            }
        }
        ui->dateTimeEdit->setDateTime(date);
        if(payment == "Cash"){
            ui->cash->setChecked(true);

        }
        else if(payment == "Credit"){
            ui->credit->setChecked(true);
        }
        else if (payment == "Credit card") {
            ui->credit_card->setChecked(true);
        }
    }
    else if (ui->rec_stat->currentIndex()==1&&!ui->tableView_2->isHidden()){
        QSqlQuery qry2(db);
        QDateTime date;
        QString payment;
        QSqlTableModel *GetTable = new QSqlTableModel(this,db);
        GetTable->setTable("sales_return_items");
        GetTable->setFilter("rec_no= "+ui->rec_no->text()+" ");
        ui->tableView_2->setModel(GetTable);
        ui->tableView_2->hideColumn(0);
        ui->tableView_2->hideColumn(1);
        ui->tableView_2->hideColumn(9);
        GetTable->select();
        QString id = ui->rec_no->text();
        qry2.prepare("SELECT create_date,payment_method,customer_id,total,discount,cashier_id,storage_id,branch_id,currency_id,seller_id,cash_deposit_id,total_before from sales_return WHERE rec_no = "+id+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                date = qry2.value(0).toDateTime();
                payment = qry2.value(1).toString();
                model14->setQuery("SELECT name,id FROM customers WHERE id = "+qry2.value(2).toString()+"",db);
                ui->total_before->setText(qry2.value(11).toString());
                ui->dicount->setText(qry2.value(4).toString());
                model11->setQuery("SELECT name,id FROM employees WHERE appear_as_cashier = 1 AND id ="+qry2.value(5).toString()+"",db);
                model2->setQuery("SELECT name,id FROM storage WHERE id = "+qry2.value(6).toString()+"",db);
                model12->setQuery("SELECT name,id FROM branch WHERE id = "+qry2.value(7).toString()+"",db);
                model13->setQuery("SELECT name,id FROM currency WHERE id = "+qry2.value(8).toString()+"",db);
                model3->setQuery("SELECT name,id FROM employees WHERE id = "+qry2.value(9).toString()+"",db);
                model10->setQuery("SELECT name,id FROM cash_deposit WHERE id = "+qry2.value(10).toString()+"",db);
                ui->total_after->setText(qry2.value(3).toString());
                ui->stock_name->setCurrentIndex(0);
                ui->branch_name->setCurrentIndex(0);
                ui->cash_name->setCurrentIndex(0);
                ui->custom_name->setCurrentIndex(0);
                ui->currency_name->setCurrentIndex(0);
                ui->cashier_name->setCurrentIndex(0);
                ui->seller_name->setCurrentIndex(0);
            }
        }
        ui->dateTimeEdit->setDateTime(date);
        if(payment == "Cash"){
            ui->cash->setChecked(true);

        }
        else if(payment == "Credit"){
            ui->credit->setChecked(true);
        }
        else if (payment == "Credit card") {
            ui->credit_card->setChecked(true);
        }
    }
    else if(ui->rec_stat->currentIndex()==1&&ui->tableWidget->isVisible()){
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        QSqlQuery qry1(db);
        QSqlQuery qry2(db);
        QString id = ui->rec_no->text();
        QDateTime date;
        QString payment;
				model14->setQuery("SELECT name,id FROM customers",db);
                model11->setQuery("SELECT name,id FROM employees WHERE appear_as_cashier = 1",db);
                model2->setQuery("SELECT name,id FROM storage",db);
                model12->setQuery("SELECT name,id FROM branch",db);
                model13->setQuery("SELECT name,id FROM currency",db);
                model3->setQuery("SELECT name,id FROM employees",db);
                model10->setQuery("SELECT name,id FROM cash_deposit",db);
        qry1.prepare("SELECT item_name,item_int,exp_barcode,quantity,price,discount_per,total from sales_return_items_g where rec_no = "+id+" ");
        if (qry1.exec())
        {
            while (qry1.next())
            {
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
                QTableWidgetItem *item_name = new QTableWidgetItem();
                QTableWidgetItem *int_code = new QTableWidgetItem();
                QTableWidgetItem *barcode = new QTableWidgetItem();
                QTableWidgetItem *quan = new QTableWidgetItem();
                QTableWidgetItem *price = new QTableWidgetItem();
                QTableWidgetItem *discount = new QTableWidgetItem();
                QTableWidgetItem *total = new QTableWidgetItem();
                item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);
                int_code->setFlags(int_code->flags() & ~Qt::ItemIsEditable);
                barcode->setFlags(barcode->flags() & ~Qt::ItemIsEditable);
                price->setFlags(price->flags() & ~Qt::ItemIsEditable);
                total->setFlags(total->flags() & ~Qt::ItemIsEditable);
                item_name->setText(qry1.value(0).toString());
                int_code->setText(qry1.value(1).toString());
                barcode->setText(qry1.value(2).toString());
                quan->setText(qry1.value(3).toString());
                price->setText(qry1.value(4).toString());
                discount->setText(qry1.value(5).toString());
                total->setText(qry1.value(6).toString());
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0,item_name);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1,int_code);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2,barcode);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3,quan);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 4,price);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 5,discount);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 6,total);

            }
        }


        qry2.prepare("SELECT create_date,payment_method,customer_id,total,discount,cashier_id,storage_id,branch_id,currency_id,seller_id,cash_deposit_id,total_before from sales_return_g WHERE rec_no = "+id+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                date = qry2.value(0).toDateTime();
                payment = qry2.value(1).toString();
                ui->total_before->setText(qry2.value(11).toString());
                ui->dicount->setText(qry2.value(4).toString());
                ui->total_after->setText(qry2.value(3).toString());
				
				QList<QModelIndex> modelIndexList;
				modelIndexList.clear();
				modelIndexList = model14->match(model14->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(2).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->custom_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model11->match(model11->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(5).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->cashier_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model2->match(model2->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(6).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->stock_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model12->match(model12->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(7).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->branch_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model13->match(model13->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(8).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->currency_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model3->match(model3->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(9).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->seller_name->setCurrentIndex(modelIndexList.first().row());
				}
				modelIndexList.clear();
				modelIndexList = model10->match(model10->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(10).toString()), -1, Qt::MatchExactly);
				if(!modelIndexList.isEmpty()){
					ui->cash_name->setCurrentIndex(modelIndexList.first().row());
               
				}
            }
        }
        ui->dateTimeEdit->setDateTime(date);
        if(payment == "Cash"){
            ui->cash->setChecked(true);

        }
        else if(payment == "Credit"){
            ui->credit->setChecked(true);
        }
        else if (payment == "Credit card") {
            ui->credit_card->setChecked(true);
        }

    }
}

void sell_win::on_back_rec_clicked()
{
    if(!searching){

    if(ui->rec_stat->currentIndex()==0){
    QSqlQuery qry(db);
    int save_in;
    int unsave_in;
    QString id = ui->rec_no->text();
    qry.prepare("SELECT MIN(rec_no) from sales");
    if (qry.exec())
    {
        while (qry.next())
        {

            save_in = qry.value(0).toInt();
        }
    }
    qry.prepare("SELECT MIN(rec_no) from sales_g");
    if (qry.exec())
    {
        while (qry.next())
        {

            unsave_in = qry.value(0).toInt();
        }
    }
    if(!ui->tableView->isHidden()&&ui->rec_no->text().toInt()>save_in){
        QSqlQuery qry(db);
        qry.prepare("SELECT rec_no FROM sales where rec_no<"+id+" order by rec_no desc limit 1");
        if (qry.exec())
        {
            while (qry.next())
            {
                ui->rec_no->setText(qry.value(0).toString());
            }
        }

    }
    else if(!ui->tableWidget->isHidden()&&ui->rec_no->text().toInt()>unsave_in){
        QSqlQuery qry(db);
        qry.prepare("SELECT rec_no FROM sales_g where rec_no<"+id+" order by rec_no desc limit 1");
        if (qry.exec())
        {
            while (qry.next())
            {
                ui->rec_no->setText(qry.value(0).toString());
            }
        }

    }
    }
    else if (ui->rec_stat->currentIndex()==1){
        QSqlQuery qry(db);
        int save_in;
        int unsave_in;
        QString id = ui->rec_no->text();
        qry.prepare("SELECT MIN(rec_no) from sales_return");
        if (qry.exec())
        {
            while (qry.next())
            {

                save_in = qry.value(0).toInt();
            }
        }
        qry.prepare("SELECT MIN(rec_no) from sales_return_g");
        if (qry.exec())
        {
            while (qry.next())
            {

                unsave_in = qry.value(0).toInt();
            }
        }
        if(!ui->tableView_2->isHidden()&&ui->rec_no->text().toInt()>save_in){
            QSqlQuery qry(db);
            qry.prepare("SELECT rec_no FROM sales_return where rec_no<"+id+" order by rec_no desc limit 1");
            if (qry.exec())
            {
                while (qry.next())
                {
                    ui->rec_no->setText(qry.value(0).toString());
                }
            }
        }
        else if(!ui->tableWidget->isHidden()&&ui->rec_no->text().toInt()>unsave_in){
            QSqlQuery qry(db);
            qry.prepare("SELECT rec_no FROM sales_return_g where rec_no<"+id+" order by rec_no desc limit 1");
            if (qry.exec())
            {
                while (qry.next())
                {
                    ui->rec_no->setText(qry.value(0).toString());
                }
            }

        }
    }

    }    else if (searching) {
        if(cur_search_index > 0){
            cur_search_index -= 1;
            ui->rec_no->setText(QString::number(search_list.at(cur_search_index)));
        }
    }

}

void sell_win::on_next_rec_clicked()
{
    if(!searching){
    if(ui->rec_stat->currentIndex()==0){
    QSqlQuery qry(db);
    int save_in;
    int unsave_in;
    QString id = ui->rec_no->text();
    qry.prepare("SELECT MAX(rec_no) from sales");
    if (qry.exec())
    {
        while (qry.next())
        {

            save_in = qry.value(0).toInt();
        }
    }
    qry.prepare("SELECT MAX(rec_no) from sales_g");
    if (qry.exec())
    {
        while (qry.next())
        {

            unsave_in = qry.value(0).toInt();
        }
    }
    if(!ui->tableView->isHidden()&&ui->rec_no->text().toInt()<save_in){
        QSqlQuery qry(db);
        qry.prepare("SELECT rec_no FROM sales where rec_no>"+id+" order by rec_no asc limit 1");
        if (qry.exec())
        {
            while (qry.next())
            {
                ui->rec_no->setText(qry.value(0).toString());
            }
        }

    }
    else if(!ui->tableWidget->isHidden()&&ui->rec_no->text().toInt()<unsave_in){
        QSqlQuery qry(db);
        qry.prepare("SELECT rec_no FROM sales_g where rec_no>"+id+" order by rec_no asc limit 1");
        if (qry.exec())
        {
            while (qry.next())
            {
                ui->rec_no->setText(qry.value(0).toString());
            }
        }

    }
    }
    else if (ui->rec_stat->currentIndex()==1){
        QSqlQuery qry(db);
        int save_in;
        int unsave_in;
        QString id = ui->rec_no->text();
        qry.prepare("SELECT MAX(rec_no) from sales_return");
        if (qry.exec())
        {
            while (qry.next())
            {

                save_in = qry.value(0).toInt();
            }
        }
        qry.prepare("SELECT MAX(rec_no) from sales_return_g");
        if (qry.exec())
        {
            while (qry.next())
            {

                unsave_in = qry.value(0).toInt();
            }
        }
        if(!ui->tableView_2->isHidden()&&ui->rec_no->text().toInt()<save_in){
            QSqlQuery qry(db);
            qry.prepare("SELECT rec_no FROM sales_return where rec_no>"+id+" order by rec_no asc limit 1");
            if (qry.exec())
            {
                while (qry.next())
                {
                    ui->rec_no->setText(qry.value(0).toString());
                }
            }
        }
        else if(!ui->tableWidget->isHidden()&&ui->rec_no->text().toInt()<unsave_in){
            QSqlQuery qry(db);
            qry.prepare("SELECT rec_no FROM sales_return_g where rec_no>"+id+" order by rec_no asc limit 1");
            if (qry.exec())
            {
                while (qry.next())
                {
                    ui->rec_no->setText(qry.value(0).toString());
                }
            }

        }
    }
    }
    else if (searching) {
        if(cur_search_index < max_search_index){
            cur_search_index += 1;
            ui->rec_no->setText(QString::number(search_list.at(cur_search_index)));
        }
    }
}

void sell_win::on_new_rec_clicked()
{
    clearAll();
    set_edit();

}

void sell_win::on_dicount_textChanged(const QString &arg1)
{
    double total_before = ui->total_before->text().toDouble();
    double discount = ui->dicount->text().toDouble();
    double total_after = total_before - discount;
    double perc = (discount / total_before)*100;
    ui->total_after->setText(QString::number(total_after));
    ui->discount_perc->setText(QString::number(perc));
}
void sell_win::clear_test(){
    clearAll();
}

void sell_win::on_pushButton_clicked()
{
    QStringList code;
    QStringList quan;
    QString id = ui->rec_no->text();
    if(ui->tableView->isVisible()||ui->tableView_2->isVisible()){    //change this to tableView i think and tableView_2
    if (QMessageBox::question(this, tr("unsave?!!!"),
    tr("Are You Sure You WANT TO unsave!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        set_edit();
        if(ui->rec_stat->currentIndex()==0){
        QSqlQuery qry(db);
        qry.prepare("INSERT INTO sales_g SELECT * FROM sales WHERE rec_no = "+ui->rec_no->text()+" ");
        if (qry.exec())
        {
			QSqlQuery qry2(db);
			qry2.prepare("Delete from sales where rec_no = "+ui->rec_no->text()+" ");
			if (qry2.exec())
			{ } 
		}
        
        QSqlQuery qry3(db);
        qry3.prepare("INSERT INTO sales_items_g SELECT * FROM sales_items WHERE rec_no = "+ui->rec_no->text()+" ");
        if (qry3.exec())
        { }              
        QSqlQuery qry4(db);
        qry4.prepare("SELECT exp_barcode,quantity FROM sales_items WHERE rec_no = "+ui->rec_no->text()+" ");
        if (qry4.exec())
        {
            while (qry4.next())
            {
                 code.append(qry4.value(0).toString());
                 quan.append(qry4.value(1).toString());

            }
        }
        QSqlQuery qry5(db);
        qry5.prepare("Delete from sales_items where rec_no = "+ui->rec_no->text()+" ");
        if (qry5.exec())
        { }



        int x = 0;
        while(x<code.size()){
            QString quan_in = quan.value(x);
            QString code_in = code.value(x);
           qDebug()<< quan_in << code_in;
        QSqlQuery qryx(db);
        qryx.prepare("UPDATE item_balance SET item_balance = item_balance + :quan ,sales_balance = sales_balance - :quan WHERE exp_barcode = :code");
        qryx.bindValue(":quan",quan_in);
        qryx.bindValue(":code",code_in);

        qDebug()<<"bexec";
        if (qryx.exec())
        {
          qDebug()<<"exce";
        }

        else{
            qDebug()<<qryx.lastError();
        }
        qDebug()<<"looping";
        x++;
        }
        clearAll();
        ui->tableWidget_2->hide();
        ui->tableView->hide();
        ui->tableView_2->hide();
        ui->tableWidget->show();
        ui->rec_no->setText(id);
      }
        else if(ui->rec_stat->currentIndex()==1){
            QSqlQuery qry(db);
            qry.prepare("INSERT INTO sales_return_g SELECT * FROM sales_return WHERE rec_no = "+ui->rec_no->text()+" ");
            if (qry.exec())
            { }
            QSqlQuery qry2(db);
            qry2.prepare("Delete from sales_return where rec_no = "+ui->rec_no->text()+" ");
            if (qry2.exec())
            { }
            QSqlQuery qry3(db);
            qry3.prepare("INSERT INTO sales_return_items_g SELECT * FROM sales_return_items WHERE rec_no = "+ui->rec_no->text()+" ");
            if (qry3.exec())
            { }
            QSqlQuery qry4(db);
            qry4.prepare("SELECT barcode,quan FROM sales_return_items WHERE rec_no = "+ui->rec_no->text()+" ");
            if (qry4.exec())
            {
                while (qry4.next())
                {
                     code.append(qry4.value(0).toString());
                     quan.append(qry4.value(1).toString());

                }
            }
            QSqlQuery qry5(db);
            qry5.prepare("Delete from sales_return_items where rec_no = "+ui->rec_no->text()+" ");
            if (qry5.exec())
            { }



            int x = 0;
            while(x<code.size()){
                QString quan_in = quan.value(x);
                QString code_in = code.value(x);
               qDebug()<< quan_in << code_in;
            QSqlQuery qryx(db);
            qryx.prepare("UPDATE item_balance SET item_balance = item_balance - :quan ,sales_return_balance = sales_return_balance - :quan WHERE exp_barcode = :code");
            qryx.bindValue(":quan",quan_in);
            qryx.bindValue(":code",code_in);

            qDebug()<<"bexec";
            if (qryx.exec())
            {
              qDebug()<<"exce";
            }

            else{
                qDebug()<<qryx.lastError();
            }
            qDebug()<<"looping";
            x++;
            }
            clearAll();
            ui->tableWidget_2->hide();
            ui->tableView->hide();
            ui->tableView_2->hide();
            ui->tableWidget->show();
            ui->rec_no->setText(id);

        }



    }
    }
    code.clear();
    quan.clear();

}



void sell_win::on_tableWidget_2_clicked(const QModelIndex &index)
{
    cur_row = index.row();
}
void sell_win::on_tableWidget_clicked(const QModelIndex &index)
{
    cur_row = index.row();
    qDebug()<<cur_row;

}
void sell_win::delete_row()
{
    if(ui->tableWidget_2->isVisible()){
     ui->tableWidget_2->removeRow(cur_row);
     if(ui->tableWidget_2->rowCount()>0){
         getTotal();
         getTotal_items();
     }
     else {
         ui->total_items->setText("0");
         ui->total_before->setText("0");
         ui->total_after->setText("0");
     }
    }
    else if(ui->tableWidget->isVisible()){
        if(ui->rec_stat->currentIndex()==0){
            QString barcode = ui->tableWidget->item(cur_row,2)->text();
            QString quan = ui->tableWidget->item(cur_row,3)->text();
            QString rec = ui->rec_no->text();
            qDebug()<<barcode<<quan<<rec;
            QSqlQuery qry5(db);
            qry5.prepare("Delete from sales_items_g where id IN ( SELECT id from sales_items_g where rec_no = :rec AND exp_barcode = :code AND quantity = :quan LIMIT 1)");
            qry5.bindValue(":code",barcode);
            qry5.bindValue(":rec",rec);
            qry5.bindValue(":quan",quan);
            if (qry5.exec())
            { }
             ui->tableWidget->removeRow(cur_row);
             if(ui->tableWidget->rowCount()>0){
                 getTotal();
                 getTotal_items();
             }
             else {
                 ui->total_items->setText("0");
                 ui->total_before->setText("0");
                 ui->total_after->setText("0");
             }
        }
        else if(ui->rec_stat->currentIndex()==1){
            QString barcode = ui->tableWidget->item(cur_row,2)->text();
            QString quan = ui->tableWidget->item(cur_row,3)->text();
            QString rec = ui->rec_no->text();
            QSqlQuery qry5(db);
            qry5.prepare("Delete from sales_return_items_g where id IN ( SELECT id from sales_return_items_g where rec_no = :rec AND exp_barcode = :code AND quantity = :quan LIMIT 1)");
            qry5.bindValue(":code",barcode);
            qry5.bindValue(":rec",rec);
            qry5.bindValue(":quan",quan);
            if (qry5.exec())
            { }
            ui->tableWidget->removeRow(cur_row);
            if(ui->tableWidget->rowCount()>0){
                getTotal();
                getTotal_items();
            }
            else {
                ui->total_items->setText("0");
                ui->total_before->setText("0");
                ui->total_after->setText("0");
            }
        }

    }
}



void sell_win::on_unsaved_clicked()
{
    if(ui->tableWidget->isHidden()){
        set_edit();

    if(ui->rec_stat->currentIndex()==0){
        ui->tableView->hide();
        ui->tableView_2->hide();
        ui->tableWidget_2->hide();
        ui->tableWidget->show();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Item name"<<"international Barcode"<<"Barcode"<<"Quantity"<<"Price"<<"Discount"<<"Total");

    QSqlQuery qry(db);
    int id;
    QDateTime date;
    QString payment;
    QString test8;
    qry.prepare("SELECT MAX(rec_no) from sales_g");
    if (qry.exec())
    {
        while (qry.next())
        {
            id = qry.value(0).toInt();
        }
    }
    ui->rec_no->setText(QString::number(id));
    }
    else if(ui->rec_stat->currentIndex()==1){
        ui->tableView->hide();
        ui->tableView_2->hide();
        ui->tableWidget_2->hide();
        ui->tableWidget->show();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Item name"<<"international Barcode"<<"Barcode"<<"Quantity"<<"Price"<<"Discount"<<"Total");

    QSqlQuery qry(db);
    int id;
    QDateTime date;
    QString payment;
    QString test8;


    qry.prepare("SELECT MAX(rec_no) from sales_return_g");
    if (qry.exec())
    {
        while (qry.next())
        {
            id = qry.value(0).toInt();
        }
    }
    ui->rec_no->setText(QString::number(id));
   }
  }
}


void sell_win::on_pushButton_2_clicked()
{
    if (QMessageBox::question(this, tr("delete?!!!"),
    tr("Are You Sure You WANT TO delete!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
    if(ui->tableView->isVisible()){
        QSqlQuery qry1(db);
        QSqlQuery qry3(db);
        QSqlQuery qry4(db);
        QString exp_barcode;
        QString quantity;
        QString storage_id;
        QString branch_id;
        double total;
        int payment_method;
        QString customer_id;
        qry3.prepare("SELECT storage_id,branch_id,payment_method_id,total,customer_id from sales where rec_no = "+ui->rec_no->text()+" ");
        if (qry3.exec())
        {
            while (qry3.next())
            {
                storage_id = qry3.value(0).toString();
                branch_id = qry3.value(1).toString();
                payment_method = qry3.value(2).toInt();
                total = qry3.value(3).toDouble();
                customer_id = qry3.value(4).toString();
            }


        }
        if(payment_method == 1){
            qry4.prepare("UPDATE customers SET balance = balance - :total WHERE id = :custom_id");
            qry4.bindValue(":total",total);
            qry4.bindValue(":custom_id",customer_id);
            if (!qry4.exec())
            qDebug()<<qry4.lastError();
        }

        qry1.prepare("SELECT exp_barcode,quantity from sales_items where rec_no = "+ui->rec_no->text()+" ");
        if (qry1.exec())
        {
            while (qry1.next())
            {
                exp_barcode = qry1.value(0).toString();
                quantity = qry1.value(1).toString();
                QSqlQuery bala2(db);
                bala2.prepare("UPDATE item_balance SET item_balance = item_balance + :quan ,sales_balance = sales_balance - :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
                bala2.bindValue(":ex_bcode",exp_barcode);
                bala2.bindValue(":quan",quantity);
                bala2.bindValue(":stock",storage_id);
                bala2.bindValue(":branch",branch_id);
                if(!bala2.exec())
                    qDebug()<<bala2.lastError();
            }
        }

        QSqlQuery qry2(db);
        qry2.prepare("Delete from sales where rec_no = "+ui->rec_no->text()+" ");
        if (qry2.exec())
        { }
        QSqlQuery qry5(db);
        qry5.prepare("Delete from sales_items where rec_no = "+ui->rec_no->text()+" ");
        if (qry5.exec())
        { }

    }
    else if (ui->tableView_2->isVisible()) {
        QSqlQuery qry1(db);
        QSqlQuery qry3(db);
        QSqlQuery qry4(db);
        QString exp_barcode;
        QString quantity;
        QString storage_id;
        QString branch_id;
        double total;
        int payment_method;
        QString customer_id;
        qry3.prepare("SELECT storage_id,branch_id,payment_method_id,total,customer_id from sales_return where rec_no = "+ui->rec_no->text()+" ");
        if (qry3.exec())
        {
            while (qry3.next())
            {
                storage_id = qry3.value(0).toString();
                branch_id = qry3.value(1).toString();
                payment_method = qry3.value(2).toInt();
                total = qry3.value(3).toDouble();
                customer_id = qry3.value(4).toString();
            }


        }
        if(payment_method == 1){
            qry4.prepare("UPDATE customers SET balance = balance + :total WHERE id = :custom_id");
            qry4.bindValue(":total",total);
            qry4.bindValue(":custom_id",customer_id);
            if (!qry4.exec())
            qDebug()<<qry4.lastError();
        }

        qry1.prepare("SELECT exp_barcode,quantity from sales_return_items where rec_no = "+ui->rec_no->text()+" ");
        if (qry1.exec())
        {
            while (qry1.next())
            {
                exp_barcode = qry1.value(0).toString();
                quantity = qry1.value(1).toString();
                QSqlQuery bala2(db);
                bala2.prepare("UPDATE item_balance SET item_balance = item_balance - :quan ,sales_return_balance = sales_return_balance - :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
                bala2.bindValue(":ex_bcode",exp_barcode);
                bala2.bindValue(":quan",quantity);
                bala2.bindValue(":stock",storage_id);
                bala2.bindValue(":branch",branch_id);
                if(!bala2.exec())
                    qDebug()<<bala2.lastError();
            }
        }

        QSqlQuery qry2(db);
        qry2.prepare("Delete from sales_return where rec_no = "+ui->rec_no->text()+" ");
        if (qry2.exec())
        { }
        QSqlQuery qry5(db);
        qry5.prepare("Delete from sales_return_items where rec_no = "+ui->rec_no->text()+" ");
        if (qry5.exec())
        { }

    }
    else if (ui->tableWidget->isVisible()) {
        if(ui->rec_stat->currentIndex()==0){
            QSqlQuery qry2(db);
            qry2.prepare("Delete from sales_g where rec_no = "+ui->rec_no->text()+" ");
            if (qry2.exec())
            { }
            QSqlQuery qry5(db);
            qry5.prepare("Delete from sales_items_g where rec_no = "+ui->rec_no->text()+" ");
            if (qry5.exec())
            { }

        }
        else if (ui->rec_stat->currentIndex()==1) {
            QSqlQuery qry2(db);
            qry2.prepare("Delete from sales_return_g where rec_no = "+ui->rec_no->text()+" ");
            if (qry2.exec())
            { }
            QSqlQuery qry5(db);
            qry5.prepare("Delete from sales_return_items_g where rec_no = "+ui->rec_no->text()+" ");
            if (qry5.exec())
            { }

        }

    }
    }
    clearAll();
}

void sell_win::on_custom_name_currentIndexChanged(int index)
{
    QString custom_id = model14->index(index,1).data().toString();
    QSqlQuery qry(db);
    qry.prepare("SELECT balance FROM customers WHERE id = :id");
    qry.bindValue(":id",custom_id);
    if( qry.exec() ){
        while (qry.next())
        {
            ui->custom_bala->setText(qry.value(0).toString());

        }
    }
    else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}
}



void sell_win::on_pushButton_3_clicked()
{
    QMessageBox print_size;
   QPushButton *large =print_size.addButton("large",QMessageBox::ActionRole);
   QPushButton *small =print_size.addButton("small",QMessageBox::ActionRole);
  QPushButton *close_but=print_size.addButton("Abort",QMessageBox::RejectRole);
   print_size.setText("please select print size");
   print_size.setWindowTitle("Size");
   print_size.exec();
   if(print_size.clickedButton() == large){
       report = new LimeReport::ReportEngine(this);
       LimeReport::ICallbackDatasource *ds =report->dataManager()->createCallbackDatasouce("ds");
       connect(ds,SIGNAL(getCallbackData(const LimeReport::CallbackInfo& , QVariant& )),
               this,SLOT(slotGetCallbackData(const LimeReport::CallbackInfo& , QVariant&)));
       if(!ui->tableWidget_2->isHidden()){
           report->dataManager()->addModel("sale",ui->tableWidget_2->model(),false);
       }
       else if (!ui->tableWidget->isHidden()) {
           ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Item name"<<"international Barcode"<<"Barcode"<<"Quantity"<<"Price"<<"Discount"<<"Total");
           report->dataManager()->addModel("sale",ui->tableWidget->model(),false);
       }else if (ui->tableView->isVisible()) {
           report->dataManager()->addModel("sale",ui->tableView->model(),false);
       }
       else if (ui->tableView_2->isVisible()) {
                  report->dataManager()->addModel("sale",ui->tableView_2->model(),false);
              }
       if(ui->tableWidget->isVisible()||ui->tableWidget_2->isVisible()){
           report->loadFromFile(":/Rc/Xml/sale_rece.lrxml");
       }else {
           report->loadFromFile(":/Rc/Xml/sale_rece_2.lrxml");

       }

       report->previewReport();
   }else if (print_size.clickedButton() == small) {
       print_recp();
   }
   else if (print_size.clickedButton() == close_but) {
       print_size.reject();
   }


}



void sell_win::on_tableWidget_2_itemChanged(QTableWidgetItem *item)
{
   /* if(item->column()==3&&ui->tableWidget_2->rowCount()!=0){
        double quan = ui->tableWidget_2->item(item->row(),3)->text().toDouble();
        double price = ui->tableWidget_2->item(item->row(),4)->text().toDouble();
        double discount = ui->tableWidget_2->item(item->row(),5)->text().toDouble();
        double total = (price * quan)-discount;
        QTableWidgetItem *total1;
        total1 = new QTableWidgetItem;
        total1->setData(0,total);
        ui->tableWidget_2->setItem(item->row(),6,total1);
       // getTotal_items();
       // getTotal();


   }*/
}

void sell_win::slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    QStringList columns;
    columns << "rec_no" << "customer" << "total_before" << "discount"<<"total"<<"date";
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
        if(info.columnName == "rec_no"){
            data = ui->rec_no->text();
        }else if (info.columnName == "customer") {
            data = ui->custom_name->currentText();
        }else if (info.columnName == "total_before") {
           data = ui->total_before->text();
        }else if (info.columnName == "discount") {
           data = ui->dicount->text();
        }else if (info.columnName == "total") {
           data = ui->total_after->text();
        }else if (info.columnName == "date") {
            data = ui->dateTimeEdit->date();
         }
        break;
    default: break;
    }
}

void sell_win::on_search_clicked()
{
    search->show();
}
