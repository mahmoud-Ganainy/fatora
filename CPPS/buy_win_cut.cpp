#include "buy_win_cut.h"
#include "ui_buy_win_cut.h"

buy_win_cut::buy_win_cut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::buy_win_cut)
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
    this->model = new QSqlQueryModel();
    this->model1 = new QSqlQueryModel();
    this->model2 = new QSqlQueryModel();
    this->model3 = new QSqlQueryModel();
    this->model4 = new QSqlQueryModel();
    this->model5 = new QSqlQueryModel();
    this->model6 = new QSqlQueryModel();
    this->model7 = new QSqlQueryModel();
    this->model8 = new QSqlQueryModel();
    this->model10 = new QSqlQueryModel();
    this->model11 = new QSqlQueryModel();
    tv = new QTableView(this);

    search = new item_search(this);
    inv_search = new invoice_search(this);
    ui->supp_name->setModel(model);
    ui->cashier_name->setModel(model1);
    ui->stock_name->setModel(model2);
    ui->cash_depo_name->setModel(model3);
    ui->currency_name->setModel(model8);
    ui->branch_name->setModel(model10);
    ui->item_name->setModel(model4);
    //ui->item_int->setModel(model5);
    ui->item_bcode->setModel(model6);

    tv->setModel(model4);
    ui->item_name->setView(tv);








    connect(tv,SIGNAL(pressed(QModelIndex)),this,SLOT(setData()));
    connect(tv,SIGNAL(activated(QModelIndex)),this,SLOT(setData()));
    connect(ui->item_name->lineEdit(),SIGNAL(returnPressed()),this,SLOT(enter_pressed_1()));
    //connect(ui->item_int->lineEdit(),SIGNAL(returnPressed()),this,SLOT(enter_pressed_2()));
    connect(ui->item_bcode->lineEdit(),SIGNAL(returnPressed()),this,SLOT(enter_pressed_3()));
    connect(inv_search,SIGNAL(accepted()),this,SLOT(search_invoice()));




    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_P), this, SLOT(print_save()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this, SLOT(show_search()));
    new QShortcut(QKeySequence(Qt::ALT + Qt::Key_X), this, SLOT(clearLines()));
    new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(delete_row()));
    QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));
        timer->start(1000);

}

buy_win_cut::~buy_win_cut()
{
    delete ui;
}
void buy_win_cut::showEvent(QShowEvent *event)
{
   QDialog::showEvent(event);
   if(first_open == true){
    clearAll();
    first_open = false;
   }
}
void buy_win_cut::show_search()
{
    inv_search->search_type = 0;
    inv_search->show();
    inv_search->clear_all();
}
void buy_win_cut::search_invoice()
{
    QString search_id = inv_search->id_out;
    int search_type = inv_search->type;
    if(!search_id.isEmpty()){
        if(search_type == 0){
            QSqlQuery qry(db);
            int counter = 0;
           if(!ui->tableWidget_2->isHidden()){
               if(ui->rec_stat->currentIndex() == 0){
                  qry.prepare("SELECT * from purchases_g where rec_no = '"+search_id+"'");
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
                   qry.prepare("SELECT * from purchases_return_g where rec_no = '"+search_id+"'");
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
               qry.prepare("SELECT * from purchases where rec_no = '"+search_id+"'");
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
                qry.prepare("SELECT * from purchases_return where rec_no = '"+search_id+"'");
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
            if(!ui->tableWidget_2->isHidden()){
                if(ui->rec_stat->currentIndex() == 0){
                   qry.prepare("SELECT rec_no from purchases_g where supplier_id = '"+search_id+"'");
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
                    qry.prepare("SELECT rec_no from purchases_return_g where supplier_id = '"+search_id+"'");
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
                   qry.prepare("SELECT rec_no from purchases where supplier_id = '"+search_id+"'");
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
                    qry.prepare("SELECT rec_no from purchases_return where supplier_id = '"+search_id+"'");
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
void buy_win_cut::clearLines()
{
   // ui->item_int->lineEdit()->clear();
    ui->item_name->lineEdit()->clear();
    ui->item_bcode->lineEdit()->clear();
    ui->price_buy->clear();
    ui->quantity->clear();
    ui->price_sell->clear();
    ui->item_bcode->lineEdit()->setFocus();
    //ui->item_int->lineEdit()->setFocus();
}
void buy_win_cut::update_itemsList()
{
    clearLines();
    db.close();
    db.open();
    qDebug()<<"saasas";
    int sup_ind = ui->supp_name->currentIndex();

    model->setQuery("SELECT name ,id FROM supplier",db);
    model1->setQuery("SELECT name ,id FROM employees WHERE appear_as_cashier = 1",db);
    model8->setQuery("SELECT name ,id FROM currency",db);
    model2->setQuery("SELECT name ,id FROM storage",db);
    model3->setQuery("SELECT name ,id FROM cash_deposit",db);
    model4->setQuery("SELECT items.name, sub_items.color ,sub_items.size , sub_items.int_barcode , sub_items.id FROM items JOIN sub_items on items.id = sub_items.item_id ORDER BY items.name ASC",db);
    //model5->setQuery("SELECT int_barcode FROM sub_items ORDER BY id ASC",db);
    model6->setQuery("SELECT id FROM sub_items ORDER BY id ASC",db);
    model7->setQuery("SELECT sub_items.id,items.name,sub_items.int_barcode,sub_items.sell_price,sub_items.buy_price FROM items JOIN sub_items on items.id = sub_items.item_id ORDER BY sub_items.id ASC",db);
    while (model7->canFetchMore()) {
        model7->fetchMore();
    }
    model10->setQuery("SELECT name ,id FROM branch",db);
    ui->supp_name->setCurrentIndex(sup_ind);
    ui->stock_name->setCurrentIndex(0);
    ui->branch_name->setCurrentIndex(0);
    ui->cashier_name->setCurrentIndex(0);
    ui->currency_name->setCurrentIndex(0);
    ui->cash_depo_name->setCurrentIndex(0);

}

void buy_win_cut::update_List()
{
    int sup_ind = ui->supp_name->currentIndex();
    db.close();
    db.open();
    model->setQuery("SELECT name ,id FROM supplier",db);
    ui->supp_name->setCurrentIndex(sup_ind);

}
void buy_win_cut::clearAll()
{
    saved = false;
    searching = false;
    search_list.clear();
    max_search_index = 0;
    cur_search_index = 0;
    clearLines();
    ui->supp_name->lineEdit()->clear();
    ui->stock_name->lineEdit()->clear();
    ui->branch_name->lineEdit()->clear();
    ui->cashier_name->lineEdit()->clear();
    ui->currency_name->lineEdit()->clear();
    ui->cash_depo_name->lineEdit()->clear();
    ui->supp_bala->clear();
    ui->total_rec->clear();
    ui->total_quan->clear();
    ui->total_after->clear();
    ui->discount->clear();
    ui->dicount_perc->clear();
    ui->tableWidget->clear();
    ui->item_bala->clear();
    ui->cash->setChecked(true);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->item_name->completer()->setCompletionMode(QCompleter::PopupCompletion);
    ui->tableView->hide();
    ui->tableView_2->hide();
    ui->tableWidget_2->hide();
    ui->tableWidget->show();
    ui->discount->setText("0");
    ui->dicount_perc->setText("0");
    db.close();
    db.open();
       int saved_count;
   int unsaved_count;
   if(ui->rec_stat->currentIndex()==0){
   QSqlQuery qry(db);
   qry.prepare("SELECT MAX(rec_no) from purchases");
   if (qry.exec())
   {
       while (qry.next())
       {
         saved_count = qry.value(0).toInt();
       }
   }
   QSqlQuery qry2(db);
   qry2.prepare("SELECT MAX(rec_no) from purchases_g");
   if (qry2.exec())
   {
       while (qry2.next())
       {
         unsaved_count = qry2.value(0).toInt();
       }
   }
   qDebug()<<saved_count<<unsaved_count;
   if(saved_count>unsaved_count){
       saved_count+=1;
       ui->rec_no->setText(QString::number(saved_count));
   }
   else if(unsaved_count>saved_count){
       unsaved_count+=1;
       ui->rec_no->setText(QString::number(unsaved_count));
   }else if (unsaved_count==saved_count){
         saved_count+=1;
       ui->rec_no->setText(QString::number(saved_count));
   }else{
            QMessageBox::warning(this,tr("FATEL ERROR"),tr("ERROR IN RECORD NUMBER; FETCHING FAILED"),QMessageBox::Ok);
        }

 }
   else if(ui->rec_stat->currentIndex()==1){
       QSqlQuery qry(db);
       qry.prepare("SELECT MAX(rec_no) from purchases_return");
       if (qry.exec())
       {
           while (qry.next())
           {
                saved_count = qry.value(0).toInt();

           }
       }
       QSqlQuery qry2(db);
       qry2.prepare("SELECT MAX(rec_no) from purchases_return_g");
       if (qry2.exec())
       {
           while (qry2.next())
           {
             unsaved_count = qry2.value(0).toInt();
           }
       }
       if(saved_count>unsaved_count){
           saved_count+=1;
           ui->rec_no->setText(QString::number(saved_count));
       }
       else if(unsaved_count>saved_count){
           unsaved_count+=1;
           ui->rec_no->setText(QString::number(unsaved_count));
       }
       else if (unsaved_count==saved_count){
            saved_count+=1;
            ui->rec_no->setText(QString::number(saved_count));
        }else{
            QMessageBox::warning(this,tr("FATEL ERROR"),tr("ERROR IN RECORD NUMBER; FETCHING FAILED"),QMessageBox::Ok);
        }

   }

   model->setQuery("SELECT name ,id FROM supplier",db);
   model1->setQuery("SELECT name ,id FROM employees WHERE appear_as_cashier = 1",db);
   model8->setQuery("SELECT name ,id FROM currency",db);
   model2->setQuery("SELECT name ,id FROM storage",db);
   model3->setQuery("SELECT name ,id FROM cash_deposit",db);
   model10->setQuery("SELECT name ,id FROM branch",db);
   model4->setQuery("SELECT items.name, sub_items.color ,sub_items.size , sub_items.int_barcode , sub_items.id FROM items JOIN sub_items on items.id = sub_items.item_id ORDER BY items.name ASC",db);
   tv->hideColumn(3);
   tv->hideColumn(4);
   //model5->setQuery("SELECT int_barcode FROM sub_items ORDER BY id ASC",db);
   model6->setQuery("SELECT id FROM sub_items ORDER BY id ASC",db);
   model7->setQuery("SELECT sub_items.id,items.name,sub_items.int_barcode,sub_items.sell_price,sub_items.buy_price FROM items JOIN sub_items on items.id = sub_items.item_id ORDER BY sub_items.id ASC",db);
   while (model7->canFetchMore()) {
       model7->fetchMore();
   }


   ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"باركود"<<"اسم الصنف"<<"سعر الشراء"<<"كمية"<<"مجموع"<<"سعرالبيع");
   ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"باركود"<<"اسم الصنف"<<"سعر الشراء"<<"كمية"<<"مجموع"<<"سعرالبيع");
   ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   ui->tableWidget->setColumnCount(6);
   ui->tableWidget_2->setColumnCount(6);

   tv->horizontalHeader()->setVisible(false);
   tv->verticalHeader()->setVisible(false);
   tv->resizeColumnsToContents();
   tv->setMinimumWidth(400);
   tv->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->supp_name->setCurrentIndex(0);
   ui->stock_name->setCurrentIndex(0);
   ui->branch_name->setCurrentIndex(0);
   ui->cashier_name->setCurrentIndex(0);
   ui->currency_name->setCurrentIndex(0);
   ui->cash_depo_name->setCurrentIndex(0);
   set_edit();
}
void buy_win_cut::on_item_name_currentIndexChanged(int index)
{
    ui->item_bala->clear();
     int code_id = model4->index(ui->item_name->currentIndex(),4).data().toInt();

     qDebug()<<code_id<<ui->item_name->currentIndex();
     QList<QModelIndex> modelIndexList;
     modelIndexList.clear();
     modelIndexList = model6->match(model6->index(0, 0), Qt::DisplayRole, QVariant::fromValue(code_id), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
         ui->item_bcode->setCurrentIndex(modelIndexList.first().row());
         //ui->item_int->setCurrentIndex(modelIndexList.first().row());
         ui->price_buy->setText(model7->index(modelIndexList.first().row(),4).data().toString());
         ui->price_sell->setText(model7->index(modelIndexList.first().row(),3).data().toString());
     }
      ui->item_bcode->setCurrentText(QString::number(code_id));


     QString ex_code = ui->item_bcode->currentText();
     QSqlQuery qry2(db);
     qry2.prepare("SELECT item_balance FROM item_balance WHERE exp_barcode = :id ");
     qry2.bindValue(":id",ex_code);
     if( qry2.exec() ){
         while (qry2.next())
         {
             ui->item_bala->setText(qry2.value(0).toString());
         }
     }
     else{qDebug() << "Error inserting into the table:\n" << qry2.lastError();}


     ui->quantity->setFocus();
     ui->quantity->setText("1");
}
void buy_win_cut::setData()
{
   /* ui->item_bala->clear();
     int code_id = model4->index(ui->item_name->currentIndex(),4).data().toInt();

     qDebug()<<code_id<<ui->item_name->currentIndex();
     QList<QModelIndex> modelIndexList;
     modelIndexList.clear();
     modelIndexList = model6->match(model6->index(0, 0), Qt::DisplayRole, QVariant::fromValue(code_id), -1, Qt::MatchExactly);
     if(!modelIndexList.isEmpty()){
         ui->item_bcode->setCurrentIndex(modelIndexList.first().row());
         ui->item_int->setCurrentIndex(modelIndexList.first().row());
         ui->price_buy->setText(model7->index(modelIndexList.first().row(),4).data().toString());
         ui->price_sell->setText(model7->index(modelIndexList.first().row(),3).data().toString());
     }

     //expire handle
     if(!ui->checkBox->isChecked()){
        int month = ui->dateEdit->date().month();
        int year =  ui->dateEdit->date().year()-2000;
        QString bcode_expire =  QString::number(code_id)+"-"+QString::number(month)+"/"+QString::number(year);
        ui->b_code->setText(bcode_expire);
     }
     else{
         ui->b_code->setText(QString::number(code_id));
     }
     QString ex_code = ui->b_code->text();
     QSqlQuery qry2(db);
     qry2.prepare("SELECT item_balance FROM item_balance WHERE exp_barcode = :id ");
     qry2.bindValue(":id",ex_code);
     if( qry2.exec() ){
         while (qry2.next())
         {
             ui->item_bala->setText(qry2.value(0).toString());
         }
     }
     else{qDebug() << "Error inserting into the table:\n" << qry2.lastError();}


     ui->item_int->lineEdit()->setFocus();
     ui->quantity->setText("1");*/

}
/*void buy_win_cut::setData_int()
{
    if(!ui->item_int->lineEdit()->text().isEmpty()){
    ui->item_bala->clear();
       int row = ui->item_int->currentIndex();
       QString item_int = ui->item_int->lineEdit()->text();
       QList<QModelIndex> modelIndexList;
       modelIndexList.clear();
       modelIndexList = model4->match(model4->index(0, 3), Qt::DisplayRole, QVariant::fromValue(item_int), -1, Qt::MatchExactly);
       if(!modelIndexList.isEmpty()){
           ui->item_name->setCurrentIndex(modelIndexList.first().row());
       }
         ui->item_bcode->setCurrentIndex(row);
         ui->price_buy->setText(model7->index(row,4).data().toString());
         ui->price_sell->setText(model7->index(row,3).data().toString());
     if(!ui->checkBox->isChecked()){
        int month = ui->dateEdit->date().month();
        int year =  ui->dateEdit->date().year()-2000;
        QString bcode_expire =  ui->item_bcode->lineEdit()->text()+"-"+QString::number(month)+"/"+QString::number(year);
        ui->b_code->setText(bcode_expire);
    }
     else{
         int bcode_test = ui->item_bcode->lineEdit()->text().toInt();
         ui->b_code->setText(QString::number(bcode_test));
     }

     QString ex_code = ui->b_code->text();
     QSqlQuery qry2(db);
     qry2.prepare("SELECT item_balance FROM item_balance WHERE exp_barcode = :id");
     qry2.bindValue(":id",ex_code);
     if( qry2.exec() ){
         while (qry2.next())
         {
             ui->item_bala->setText(qry2.value(0).toString());
         }
     }
     else{qDebug() << "Error inserting into the table:\n" << qry2.lastError();}
     ui->item_bcode->lineEdit()->setFocus();
    ui->quantity->setText("1");}
    else{
        ui->item_bcode->lineEdit()->setFocus();}

}*/
void buy_win_cut::setData_bcode()
{
    if(!ui->item_bcode->lineEdit()->text().isEmpty()){
    ui->item_bala->clear();
    int row = ui->item_bcode->currentIndex();
    int barcode = ui->item_bcode->lineEdit()->text().toInt();
    QList<QModelIndex> modelIndexList;
    modelIndexList.clear();
    modelIndexList = model4->match(model4->index(0, 4), Qt::DisplayRole, QVariant::fromValue(barcode), -1, Qt::MatchExactly);
    if(!modelIndexList.isEmpty()){
        ui->item_name->setCurrentIndex(modelIndexList.first().row());
    }
      //ui->item_int->setCurrentIndex(row);
      ui->price_buy->setText(model7->index(row,4).data().toString());
      ui->price_sell->setText(model7->index(row,3).data().toString());
    QString ex_code = ui->item_bcode->lineEdit()->text();
    QSqlQuery qry2(db);
    qry2.prepare("SELECT item_balance FROM item_balance WHERE exp_barcode = :id");
    qry2.bindValue(":id",ex_code);
    if( qry2.exec() ){
        while (qry2.next())
        {
            ui->item_bala->setText(qry2.value(0).toString());
        }
    }
    else{qDebug() << "Error inserting into the table:\n" << qry2.lastError();}
    ui->item_bcode->lineEdit()->setFocus();
    ui->quantity->setText("1");
    }
    else {
        ui->item_name->lineEdit()->setFocus();
    }
}
void buy_win_cut::setTable()
{
    if(ui->tableWidget->isVisible()&&ui->tableWidget_2->isHidden()){
    int CurrentRow = ui->tableWidget->rowCount();

    double quan_s = ui->quantity->text().toDouble();
    double buy_s = ui->price_buy->text().toDouble();
    double total_s = quan_s * buy_s ;


    QTableWidgetItem *item_name = new QTableWidgetItem();
    //QTableWidgetItem *int_code = new QTableWidgetItem();
    QTableWidgetItem *barcode = new QTableWidgetItem();
    QTableWidgetItem *quan = new QTableWidgetItem();
    QTableWidgetItem *buy = new QTableWidgetItem();
    QTableWidgetItem *sell = new QTableWidgetItem();
    QTableWidgetItem *total = new QTableWidgetItem();

    item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);
    item_name->setCheckState(Qt::Unchecked);
    //int_code->setFlags(int_code->flags() & ~Qt::ItemIsEditable);
    barcode->setFlags(barcode->flags() & ~Qt::ItemIsEditable);
    buy->setFlags(buy->flags() & ~Qt::ItemIsEditable);
    sell->setFlags(sell->flags() & ~Qt::ItemIsEditable);
    total->setFlags(total->flags() & ~Qt::ItemIsEditable);
    quan->setFlags(total->flags() & Qt::ItemIsEditable);

    item_name->setText(" "+model4->index(ui->item_name->currentIndex(),0).data().toString()+" "+model4->index(ui->item_name->currentIndex(),1).data().toString()+" "+model4->index(ui->item_name->currentIndex(),2).data().toString()+" ");
    //int_code->setText(ui->item_int->lineEdit()->text());
    barcode->setText(ui->item_bcode->lineEdit()->text());
    buy->setText(ui->price_buy->text());
    sell->setText(ui->price_sell->text());
    total->setText(QString::number(total_s));
    quan->setText(ui->quantity->text());



    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(CurrentRow, 0, barcode);
    ui->tableWidget->setItem(CurrentRow, 1, item_name);
    //ui->tableWidget->setItem(CurrentRow, 2, int_code);
    ui->tableWidget->setItem(CurrentRow, 2, buy);
    ui->tableWidget->setItem(CurrentRow, 3, quan);
    ui->tableWidget->setItem(CurrentRow, 4, total);
    ui->tableWidget->setItem(CurrentRow, 5, sell);
    getTotal();
    getTotal_items();
    QModelIndex stock_index = model2->index(ui->stock_name->currentIndex(),1);
    QModelIndex branch_index = model10->index(ui->branch_name->currentIndex(),1);
    QString stock_id = model2->data(stock_index).toString();
    QString branch_id = model10->data(branch_index).toString();
    //purch
    QString purch_id = ui->rec_no->text();
    QString rec_no = ui->custom_id->text();
    QModelIndex supp_index = model->index(ui->supp_name->currentIndex(),1);
    QString supp_id = model->data(supp_index).toString();
    QModelIndex cash_depo_index = model3->index(ui->cash_depo_name->currentIndex(),1);
    QString cash_depo_id = model3->data(cash_depo_index).toString();
    QModelIndex cashier_index = model1->index(ui->cashier_name->currentIndex(),1);
    QString cashier_id = model1->data(cashier_index).toString();
    QModelIndex currency_index = model8->index(ui->currency_name->currentIndex(),1);
    QString currency_id = model8->data(currency_index).toString();
    QString total_g = ui->total_rec->text();
    QString total_after = ui->total_after->text();
    QString discount = ui->discount->text();
    QString pay_meth;
    int payment_methode_n;
    if(ui->cash->isChecked()){
      pay_meth = "Cash";
    payment_methode_n = 0;
    }
    else if(ui->credit->isChecked()){
       pay_meth = "Credit";
        payment_methode_n = 1;
    }
    else if ( ui->cheque->isChecked()) {
       pay_meth = "Cheque";
        payment_methode_n = 2;

   }

    if(ui->rec_stat->currentIndex() == 0){

    int recCount2=0;
       QSqlQuery qry2(db);
       qry2.prepare("SELECT * FROM purchases_g WHERE rec_no = :purch_id");
       qry2.bindValue(":purch_id",purch_id);
       if( qry2.exec() ){
           while (qry2.next())
           {
               recCount2++;
           }
       }

       else{
             qDebug() << "Error inserting into the table:\n" << qry2.lastError();
      }
       if(recCount2==0){
       if(ui->cash->isChecked()){
       QSqlQuery purch_csh(db);
       purch_csh.prepare("INSERT INTO purchases_g(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,0,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
       purch_csh.bindValue(":purch_id",purch_id);
       purch_csh.bindValue(":rec_no",rec_no);
       purch_csh.bindValue(":supp_id",supp_id);
       purch_csh.bindValue(":total",total_g);
       purch_csh.bindValue(":discount",discount);
       purch_csh.bindValue(":total_after",total_after);
       purch_csh.bindValue(":payment",pay_meth);
       purch_csh.bindValue(":stock_id",stock_id);
       purch_csh.bindValue(":branch",branch_id);
       purch_csh.bindValue(":cash_depo",cash_depo_id);
       purch_csh.bindValue(":currency",currency_id);
       purch_csh.bindValue(":employee",cashier_id);
       if(!purch_csh.exec())
           qDebug()<<purch_csh.lastError();
       }
       else if (ui->credit->isChecked()) {
        QSqlQuery purch_crdt(db);
        purch_crdt.prepare("INSERT INTO purchases_g(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,1,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
        purch_crdt.bindValue(":purch_id",purch_id);
        purch_crdt.bindValue(":rec_no",rec_no);
        purch_crdt.bindValue(":supp_id",supp_id);
        purch_crdt.bindValue(":total",total_g);
        purch_crdt.bindValue(":discount",discount);
        purch_crdt.bindValue(":total_after",total_after);
        purch_crdt.bindValue(":payment",pay_meth);
        purch_crdt.bindValue(":stock_id",stock_id);
        purch_crdt.bindValue(":branch",branch_id);
        purch_crdt.bindValue(":cash_depo",cash_depo_id);
        purch_crdt.bindValue(":currency",currency_id);
        purch_crdt.bindValue(":employee",cashier_id);
        if(!purch_crdt.exec())
            qDebug()<<purch_crdt.lastError();
       }
       else if(ui->cheque->isChecked()){
        QSqlQuery purch_chqs(db);
        purch_chqs.prepare("INSERT INTO purchases_g(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,2,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
        purch_chqs.bindValue(":purch_id",purch_id);
        purch_chqs.bindValue(":rec_no",rec_no);
        purch_chqs.bindValue(":supp_id",supp_id);
        purch_chqs.bindValue(":total",total_g);
        purch_chqs.bindValue(":discount",discount);
        purch_chqs.bindValue(":total_after",total_after);
        purch_chqs.bindValue(":payment",pay_meth);
        purch_chqs.bindValue(":stock_id",stock_id);
        purch_chqs.bindValue(":branch",branch_id);
        purch_chqs.bindValue(":cash_depo",cash_depo_id);
        purch_chqs.bindValue(":currency",currency_id);
        purch_chqs.bindValue(":employee",cashier_id);
        if(!purch_chqs.exec())
            qDebug()<<purch_chqs.lastError();
       }
    }
        QSqlQuery salesb(db);
        salesb.prepare("INSERT INTO purchases_items_g(rec_no,item_name,exp_barcode,quantity,buy_price,sell_price,total)VALUES( :rec_no, :item_name, :barcode, :quan, :buy,:sell,:total)");
        salesb.bindValue(":rec_no",ui->rec_no->text());
        salesb.bindValue(":item_name",ui->item_name->lineEdit()->text());
        //salesb.bindValue(":item_int",ui->item_int->lineEdit()->text());
        salesb.bindValue(":barcode",ui->item_bcode->lineEdit()->text());
        salesb.bindValue(":quan",ui->quantity->text().toDouble());
        salesb.bindValue(":buy",ui->price_buy->text().toDouble());
        salesb.bindValue(":sell",ui->price_sell->text().toDouble());
        salesb.bindValue(":total",ui->price_buy->text().toDouble()*ui->quantity->text().toDouble());
         if(!salesb.exec())
                qDebug()<<salesb.lastError();

    }
    else if(ui->rec_stat->currentIndex() == 1){
        int recCount2=0;
       QSqlQuery qry2(db);
       qry2.prepare("SELECT * FROM purchases_return_g WHERE rec_no = :purch_id");
       qry2.bindValue(":purch_id",purch_id);
       if( qry2.exec() ){
           while (qry2.next())
           {
               recCount2++;
           }
       }

       else{
             qDebug() << "Error inserting into the table:\n" << qry2.lastError();
      }
       if(recCount2==0){
       if(ui->cash->isChecked()){
       QSqlQuery purch_csh(db);
       purch_csh.prepare("INSERT INTO purchases_return_g(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,0,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
       purch_csh.bindValue(":purch_id",purch_id);
       purch_csh.bindValue(":rec_no",rec_no);
       purch_csh.bindValue(":supp_id",supp_id);
       purch_csh.bindValue(":total",total_g);
       purch_csh.bindValue(":discount",discount);
       purch_csh.bindValue(":total_after",total_after);
       purch_csh.bindValue(":payment",pay_meth);
       purch_csh.bindValue(":stock_id",stock_id);
       purch_csh.bindValue(":branch",branch_id);
       purch_csh.bindValue(":cash_depo",cash_depo_id);
       purch_csh.bindValue(":currency",currency_id);
       purch_csh.bindValue(":employee",cashier_id);
       if(!purch_csh.exec())
           qDebug()<<purch_csh.lastError();
       }
       else if (ui->credit->isChecked()) {
        QSqlQuery purch_crdt(db);
        purch_crdt.prepare("INSERT INTO purchases_return_g(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,1,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
        purch_crdt.bindValue(":purch_id",purch_id);
        purch_crdt.bindValue(":rec_no",rec_no);
        purch_crdt.bindValue(":supp_id",supp_id);
        purch_crdt.bindValue(":total",total_g);
        purch_crdt.bindValue(":discount",discount);
        purch_crdt.bindValue(":total_after",total_after);
        purch_crdt.bindValue(":payment",pay_meth);
        purch_crdt.bindValue(":stock_id",stock_id);
        purch_crdt.bindValue(":branch",branch_id);
        purch_crdt.bindValue(":cash_depo",cash_depo_id);
        purch_crdt.bindValue(":currency",currency_id);
        purch_crdt.bindValue(":employee",cashier_id);
        if(!purch_crdt.exec())
            qDebug()<<purch_crdt.lastError();
       }
       else if(ui->cheque->isChecked()){
        QSqlQuery purch_chqs(db);
        purch_chqs.prepare("INSERT INTO purchases_return_g(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,2,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
        purch_chqs.bindValue(":purch_id",purch_id);
        purch_chqs.bindValue(":rec_no",rec_no);
        purch_chqs.bindValue(":supp_id",supp_id);
        purch_chqs.bindValue(":total",total_g);
        purch_chqs.bindValue(":discount",discount);
        purch_chqs.bindValue(":total_after",total_after);
        purch_chqs.bindValue(":payment",pay_meth);
        purch_chqs.bindValue(":stock_id",stock_id);
        purch_chqs.bindValue(":branch",branch_id);
        purch_chqs.bindValue(":cash_depo",cash_depo_id);
        purch_chqs.bindValue(":currency",currency_id);
        purch_chqs.bindValue(":employee",cashier_id);
        if(!purch_chqs.exec())
            qDebug()<<purch_chqs.lastError();
       }
    }
        QSqlQuery salesb(db);
        salesb.prepare("INSERT INTO purchases_return_items_g(rec_no,item_name,exp_barcode,quantity,buy_price,sell_price,total)VALUES( :rec_no, :item_name, :barcode, :quan, :buy,:sell,:total)");
        salesb.bindValue(":rec_no",ui->rec_no->text());
        salesb.bindValue(":item_name",ui->item_name->lineEdit()->text());
        //salesb.bindValue(":item_int",ui->item_int->lineEdit()->text());
        salesb.bindValue(":barcode",ui->item_bcode->lineEdit()->text());
        salesb.bindValue(":quan",ui->quantity->text().toDouble());
        salesb.bindValue(":buy",ui->price_buy->text().toDouble());
        salesb.bindValue(":sell",ui->price_sell->text().toDouble());
        salesb.bindValue(":total",ui->price_buy->text().toDouble()*ui->quantity->text().toDouble());

             if(!salesb.exec())
                qDebug()<<salesb.lastError();


        }



    }
    else if(ui->tableWidget_2->isVisible()&&ui->tableWidget->isHidden()){
    int CurrentRow = ui->tableWidget_2->rowCount();

    double quan_s = ui->quantity->text().toDouble();
    double buy_s = ui->price_buy->text().toDouble();
    double total_s = quan_s * buy_s ;


    QTableWidgetItem *item_name = new QTableWidgetItem();
    //QTableWidgetItem *int_code = new QTableWidgetItem();
    QTableWidgetItem *barcode = new QTableWidgetItem();
    QTableWidgetItem *quan = new QTableWidgetItem();
    QTableWidgetItem *buy = new QTableWidgetItem();
    QTableWidgetItem *sell = new QTableWidgetItem();
    QTableWidgetItem *total = new QTableWidgetItem();

    item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);
    item_name->setCheckState(Qt::Unchecked);
    //int_code->setFlags(int_code->flags() & ~Qt::ItemIsEditable);
    barcode->setFlags(barcode->flags() & ~Qt::ItemIsEditable);
    buy->setFlags(buy->flags() & ~Qt::ItemIsEditable);
    sell->setFlags(sell->flags() & ~Qt::ItemIsEditable);
    total->setFlags(total->flags() & ~Qt::ItemIsEditable);
    quan->setFlags(total->flags() & Qt::ItemIsEditable);

    item_name->setText(" "+model4->index(ui->item_name->currentIndex(),0).data().toString()+" "+model4->index(ui->item_name->currentIndex(),1).data().toString()+" "+model4->index(ui->item_name->currentIndex(),2).data().toString()+" ");
    //int_code->setText(ui->item_int->lineEdit()->text());
    barcode->setText(ui->item_bcode->lineEdit()->text());
    buy->setText(ui->price_buy->text());
    sell->setText(ui->price_sell->text());
    total->setText(QString::number( total_s));
    quan->setText(ui->quantity->text());

    ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
    ui->tableWidget_2->setItem(CurrentRow, 0, barcode);
    ui->tableWidget_2->setItem(CurrentRow, 1, item_name);
   // ui->tableWidget_2->setItem(CurrentRow, 2, int_code);
    ui->tableWidget_2->setItem(CurrentRow, 2, buy);
    ui->tableWidget_2->setItem(CurrentRow,3, quan);
    ui->tableWidget_2->setItem(CurrentRow, 4, total);
    ui->tableWidget_2->setItem(CurrentRow, 5, sell);
    if(ui->rec_stat->currentIndex()==0){
        QSqlQuery salesb(db);
        // create unsaved table and change it's name
        salesb.prepare("INSERT INTO purchases_items_g(rec_no,item_name,exp_barcode,quantity,buy_price,sell_price,total)VALUES( :rec_no, :item_name, :barcode, :quan, :buy,:sell,:total)");
        salesb.bindValue(":rec_no",ui->rec_no->text());
        salesb.bindValue(":item_name",ui->item_name->lineEdit()->text());
       // salesb.bindValue(":item_int",ui->item_int->lineEdit()->text());
        salesb.bindValue(":barcode",ui->item_bcode->currentText());
        salesb.bindValue(":quan",ui->quantity->text().toDouble());
        salesb.bindValue(":buy",ui->price_buy->text().toDouble());
        salesb.bindValue(":sell",ui->price_sell->text().toDouble());
        salesb.bindValue(":total",ui->price_buy->text().toDouble()*ui->quantity->text().toDouble());

         if(!salesb.exec())
            qDebug()<<salesb.lastError()<<"kljlkjljk";
    }
        else if(ui->rec_stat->currentIndex()==1){
            QSqlQuery salesb(db);
            salesb.prepare("INSERT INTO purchases_return_items_g(rec_no,item_name,exp_barcode,quantity,buy_price,sell_price,total)VALUES( :rec_no, :item_int, :barcode, :quan, :buy,:sell,:total)");
        salesb.bindValue(":rec_no",ui->rec_no->text());
        salesb.bindValue(":item_name",ui->item_name->lineEdit()->text());
        //salesb.bindValue(":item_int",ui->item_int->lineEdit()->text());
        salesb.bindValue(":barcode",ui->item_bcode->currentText());
        salesb.bindValue(":quan",ui->quantity->text().toDouble());
        salesb.bindValue(":buy",ui->price_buy->text().toDouble());
        salesb.bindValue(":sell",ui->price_sell->text().toDouble());
        salesb.bindValue(":total",ui->price_buy->text().toDouble()*ui->quantity->text().toDouble());

             if(!salesb.exec())
                qDebug()<<salesb.lastError();
        }
    getTotal();
    getTotal_items();

    }

}
void buy_win_cut::getTotal()
{
    if(ui->tableWidget->isVisible()){
    int column = 4;
    double total = 0 ;
    for(int row = 0; row < ui->tableWidget->rowCount(); row++){
        total +=  ui->tableWidget->item(row,column)->text().toDouble();
    }
    ui->total_rec->setText(QString::number(total));}
    else if (ui->tableWidget_2->isVisible()){
        int column = 4;
        double total = 0 ;
        for(int row = 0; row < ui->tableWidget_2->rowCount(); row++){
        total +=  ui->tableWidget_2->item(row,column)->text().toDouble();
        }
        ui->total_rec->setText(QString::number(total));
        if(ui->rec_stat->currentIndex()==0){
           QSqlQuery qry(db);
           //check to use total before or after
           qry.prepare("UPDATE purchases_g SET total = :total , discount = :discount WHERE rec_no = :rec ");
           qry.bindValue(":total",ui->total_rec->text());
           qry.bindValue(":discount",ui->discount->text());
           qry.bindValue(":rec",ui->rec_no->text());
           if(!qry.exec())
               qDebug()<<qry.lastError();
                qDebug()<<"asddasdsadsa";

           }
           else if(ui->rec_stat->currentIndex()==1){
               QSqlQuery qry(db);
               qry.prepare("UPDATE purchases_return_g SET total = :total , discount = :discount WHERE rec_no = :rec ");
               qry.bindValue(":total",ui->total_rec->text());
               qry.bindValue(":discount",ui->discount->text());
               qry.bindValue(":rec",ui->rec_no->text());
               if(!qry.exec())
                   qDebug()<<qry.lastError();
           }




    }
}
void buy_win_cut::getTotal_items()
{
    if(ui->tableWidget->isVisible()){
    int column = 3;
    double total = 0 ;
    for(int row = 0; row < ui->tableWidget->rowCount(); row++){
        total +=  ui->tableWidget->item(row,column)->text().toDouble();
    }
    ui->total_quan->setText(QString::number(total));}
    else if(ui->tableWidget_2->isVisible()){
    int column = 3;
    double total = 0 ;
    for(int row = 0; row < ui->tableWidget_2->rowCount(); row++){
        total +=  ui->tableWidget_2->item(row,column)->text().toDouble();
    }
    ui->total_quan->setText(QString::number(total));

    }
}
void buy_win_cut::enter_pressed_1(){
    if(!ui->item_name->lineEdit()->text().isEmpty()){
        ui->item_bala->clear();
         int code_id = model4->index(ui->item_name->currentIndex(),4).data().toInt();

         qDebug()<<code_id<<ui->item_name->currentIndex();
         QList<QModelIndex> modelIndexList;
         modelIndexList.clear();
         modelIndexList = model6->match(model6->index(0, 0), Qt::DisplayRole, QVariant::fromValue(code_id), -1, Qt::MatchExactly);
         if(!modelIndexList.isEmpty()){
             ui->item_bcode->setCurrentIndex(modelIndexList.first().row());
             //ui->item_int->setCurrentIndex(modelIndexList.first().row());
             ui->price_buy->setText(model7->index(modelIndexList.first().row(),4).data().toString());
             ui->price_sell->setText(model7->index(modelIndexList.first().row(),3).data().toString());
         }



             ui->item_bcode->setCurrentText((QString::number(code_id)));

         QString ex_code = ui->item_bcode->currentText();
         QSqlQuery qry2(db);
         qry2.prepare("SELECT item_balance FROM item_balance WHERE exp_barcode = :id ");
         qry2.bindValue(":id",ex_code);
         if( qry2.exec() ){
             while (qry2.next())
             {
                 ui->item_bala->setText(qry2.value(0).toString());
             }
         }
         else{qDebug() << "Error inserting into the table:\n" << qry2.lastError();}
         ui->quantity->setText("1");
    }
    else{
        ui->quantity->setFocus();
    }
}
void buy_win_cut::enter_pressed_2(){
  //  setData_int();
}
void buy_win_cut::enter_pressed_3(){
    setData_bcode();
    ui->quantity->setFocus();
    ui->quantity->selectAll();
}
void buy_win_cut::enter_pressed_4(){
    ui->price_sell->setFocus();
    ui->price_sell->selectAll();
}
void buy_win_cut::slotGetPrintCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    QStringList columns;
    columns << "Barcode" << "Item_name" << "Sell_Price";
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
        if(info.columnName == "Barcode"){
            data = print_barcode;
        }else if (info.columnName == "Item_name") {
            data = print_item_name;
        }
        else if (info.columnName == "Sell_Price") {
            data = print_sell;
        }
        break;
        default: break;


     }



}
void buy_win_cut::print_bcode(){
    QString printer_name;
    QSqlQuery qry(db2);
    qry.prepare("SELECT print_name from print_info WHERE print_type = 1");
    if (qry.exec())
    {while (qry.next())
        {
           printer_name = qry.value(0).toString();
        }
    }

    report = new LimeReport::ReportEngine(this);
    QPrinter *bcode_printer;
    bcode_printer = new QPrinter();
    bcode_printer->setPrinterName(printer_name);
    QString loc = QCoreApplication::applicationDirPath()+"/large_bcode.lrxml";
    report->loadFromFile(loc);
    LimeReport::ICallbackDatasource *ds_1 =report->dataManager()->createCallbackDatasouce("bcode");
    connect(ds_1,SIGNAL(getCallbackData(const LimeReport::CallbackInfo& , QVariant& )),
            this,SLOT(slotGetPrintCallbackData(const LimeReport::CallbackInfo& , QVariant&)));
    int x = 0;
    int rowCount = ui->tableWidget->rowCount();
    int rowCount_g = ui->tableWidget_2->rowCount();
    QSqlQuery qry1(db);
    QSqlQuery qry2(db);
    if(!ui->tableWidget->isHidden()&&ui->tableWidget->rowCount()!=0){
         while (x<rowCount) {
                print_item_name = ui->tableWidget->item(x,1)->text();
                print_barcode = ui->tableWidget->item(x,0)->text();
                print_sell = ui->tableWidget->item(x,5)->text();
                bcode_printer->setCopyCount(ui->tableWidget->item(x,3)->text().toInt());
                report->printReport(bcode_printer);
             x++;
            }
    }
    else if (!ui->tableWidget_2->isHidden()&&ui->tableWidget_2->rowCount()!=0) {
        while (x<rowCount_g) {
                print_item_name = ui->tableWidget_2->item(x,1)->text();
                print_barcode = ui->tableWidget_2->item(x,0)->text();
                print_sell = ui->tableWidget_2->item(x,5)->text();
                bcode_printer->setCopyCount(ui->tableWidget_2->item(x,3)->text().toInt());
                report->printReport(bcode_printer);
          x++;
         }
    }
    else if(!ui->tableView->isHidden()){
        qry1.prepare("SELECT item_name,exp_barcode,quantity,sell_price from purchases_items WHERE rec_no = :id");
        qry1.bindValue(":id",ui->rec_no->text());
        if (qry1.exec())
        {
            while (qry1.next())
            {
                print_item_name = qry1.value(0).toString();
                print_barcode = qry1.value(1).toString();
                print_sell = qry1.value(3).toString();
                bcode_printer->setCopyCount(qry1.value(2).toInt());
                report->printReport(bcode_printer);

            }
        }else{
        qDebug() << "Error inserting into the table:\n" << qry.lastError();}
    }
    else if(!ui->tableView_2->isHidden()){
        qry2.prepare("SELECT item_name,exp_barcode,quantity,sell_price from purchases_return_items WHERE rec_no = :id");
        qry2.bindValue(":id",ui->rec_no->text());
        if (qry2.exec())
        {
            while (qry2.next())
            {
                print_item_name = qry1.value(0).toString();
                print_barcode = qry1.value(1).toString();
                print_sell = qry1.value(3).toString();
                bcode_printer->setCopyCount(qry1.value(2).toInt());
                report->printReport(bcode_printer);
            }
        }else{
        qDebug() << "Error inserting into the table:\n" << qry.lastError();}

    }


    /*int x = 0;
    int y = 0;
    int rowCount = ui->tableWidget->rowCount();
    int rowCount_g = ui->tableWidget_2->rowCount();
    QtRPT *report = new QtRPT(this);
    report->loadReport(":/Rc/Xml/large.xml");
    report->recordCount.append(3);



    QString item_name;
    QString bcode;
    int quan;
    QString expire;
    QString sell_price;
    QSqlQuery qry1(db);
    QSqlQuery qry2(db);
    if(!ui->tableWidget->isHidden()&&ui->tableWidget->rowCount()!=0){
     while (x<rowCount) {
       item_name = ui->tableWidget->item(x,1)->text();
       bcode = ui->tableWidget->item(x,3)->text();
       expire = ui->tableWidget->item(x,7)->text();
       sell_price = ui->tableWidget->item(x,8)->text();
       quan = ui->tableWidget->item(x,5)->text().toInt();

       connect(report, &QtRPT::setValue, [&](const int recNo,
                   const QString paramName, QVariant &paramValue,
                   const int reportPage) {
                   (void) reportPage;
               if (paramName == "code") {
                   paramValue = bcode;
               }
               if (paramName == "expire") {
                   paramValue = expire;
               }
               if (paramName == "item_name") {
                   paramValue = item_name;
               }
               if (paramName == "price") {
                   paramValue = sell_price;
               }

       });
           report->printExec(true,true,printer_name,quan);

       x++;
      }
    }
    else if (!ui->tableWidget_2->isHidden()&&ui->tableWidget_2->rowCount()!=0) {
        while (x<rowCount_g) {
          item_name = ui->tableWidget_2->item(x,1)->text();
          bcode = ui->tableWidget_2->item(x,3)->text();
          sell_price = ui->tableWidget_2->item(x,8)->text();
          quan = ui->tableWidget_2->item(x,5)->text().toInt();

          connect(report, &QtRPT::setValue, [&](const int recNo,
                      const QString paramName, QVariant &paramValue,
                      const int reportPage) {
                      (void) reportPage;
                  if (paramName == "code") {
                      paramValue = bcode;
                  }
                  if (paramName == "item_name") {
                      paramValue = item_name;
                  }
                  if (paramName == "price") {
                      paramValue = sell_price;
                  }

          });
              report->printExec(true,true,printer_name,quan);

          x++;
         }
    }
    else if(!ui->tableView->isHidden()){
        qry1.prepare("SELECT item_name,exp_barcode,quantity,sell_price from purchases_items WHERE rec_no = :id");
        qry1.bindValue(":id",ui->rec_no->text());
        if (qry1.exec())
        {
            while (qry1.next())
            {
               item_name = qry1.value(0).toString();
               bcode = qry1.value(1).toString();
               sell_price = qry1.value(3).toString();
               quan = qry1.value(2).toInt();
               connect(report, &QtRPT::setValue, [&](const int recNo,
                       const QString paramName, QVariant &paramValue,
                       const int reportPage) {
                       (void) reportPage;
                   if (paramName == "code") {
                       paramValue = bcode;
                   }
                   if (paramName == "item_name") {
                       paramValue = item_name;
                   }
                   if (paramName == "price") {
                       paramValue = sell_price;
                   }

                });

                    report->printExec(true,true,printer_name,quan);
            }
        }else{
        qDebug() << "Error inserting into the table:\n" << qry.lastError();}
    }
    else if(!ui->tableView_2->isHidden()){
        qry2.prepare("SELECT item_name,exp_barcode,quantity,sell_price from purchases_return_items WHERE rec_no = :id");
        qry2.bindValue(":id",ui->rec_no->text());
        if (qry2.exec())
        {
            while (qry2.next())
            {
               item_name = qry2.value(0).toString();
               bcode = qry2.value(1).toString();
               sell_price = qry2.value(3).toString();
               quan = qry2.value(2).toInt();
               connect(report, &QtRPT::setValue, [&](const int recNo,
                       const QString paramName, QVariant &paramValue,
                       const int reportPage) {
                       (void) reportPage;
                   if (paramName == "code") {
                       paramValue = bcode;
                   }
                   if (paramName == "item_name") {
                       paramValue = item_name;
                   }
                   if (paramName == "price") {
                       paramValue = sell_price;
                   }

                });

                    report->printExec(true,true,printer_name,quan);
            }
        }else{
        qDebug() << "Error inserting into the table:\n" << qry.lastError();}

    }*/
}

void buy_win_cut::print_save(){
    print_bcode();
    save_items();

}
void buy_win_cut::save_items(){
    int rowCount = ui->tableWidget->rowCount();
    int rowCount_g= ui->tableWidget_2->rowCount();

    int x = 0;
    //bala
    QString id;
    QString quan;
    QModelIndex stock_index = model2->index(ui->stock_name->currentIndex(),1);
    QModelIndex branch_index = model10->index(ui->branch_name->currentIndex(),1);
    QString stock_id = model2->data(stock_index).toString();
    QString branch_id = model10->data(branch_index).toString();
    //purch
    QString purch_id = ui->rec_no->text();
    QString rec_no = ui->custom_id->text();
    QModelIndex supp_index = model->index(ui->supp_name->currentIndex(),1);
    QString supp_id = model->data(supp_index).toString();
    QModelIndex cash_depo_index = model3->index(ui->cash_depo_name->currentIndex(),1);
    QString cash_depo_id = model3->data(cash_depo_index).toString();
    QModelIndex cashier_index = model1->index(ui->cashier_name->currentIndex(),1);
    QString cashier_id = model1->data(cashier_index).toString();
    QModelIndex currency_index = model8->index(ui->currency_name->currentIndex(),1);
    QString currency_id = model8->data(currency_index).toString();
    QString total = ui->total_rec->text();
    QString total_after = ui->total_after->text();
    QString discount = ui->discount->text();
    QString pay_meth;
    int payment_methode_n;
    if(ui->cash->isChecked()){
      pay_meth = "Cash";
    payment_methode_n = 0;
    }
    else if(ui->credit->isChecked()){
       pay_meth = "Credit";
        payment_methode_n = 1;
    }
    else if ( ui->cheque->isChecked()) {
       pay_meth = "Cheque";
        payment_methode_n = 2;

   }

    //purch_b
    QString buy_price;
    QString sell_price;
    QString total_price;
    QString item_name;
    QString item_int;


    if(ui->tableWidget->isVisible() && rowCount > 0){
    if(ui->rec_stat->currentIndex() == 0){

    int recCount2=0;
       QSqlQuery qry2(db);
       qry2.prepare("SELECT * FROM purchases WHERE rec_no = :purch_id");
       qry2.bindValue(":purch_id",purch_id);
       if( qry2.exec() ){
           while (qry2.next())
           {
               recCount2++;
           }
       }

       else{
           QMessageBox::warning(this,tr("error Updating"),qry2.lastError().text(),QMessageBox::Ok);
             qDebug() << "Error inserting into the table:\n" << qry2.lastError();
      }
       if(recCount2==0){
       if(ui->cash->isChecked()){
       QSqlQuery purch_csh(db);
       purch_csh.prepare("INSERT INTO purchases(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,0,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
       purch_csh.bindValue(":purch_id",purch_id);
       purch_csh.bindValue(":rec_no",rec_no);
       purch_csh.bindValue(":supp_id",supp_id);
       purch_csh.bindValue(":total",total);
       purch_csh.bindValue(":discount",discount);
       purch_csh.bindValue(":total_after",total_after);
       purch_csh.bindValue(":payment",pay_meth);
       purch_csh.bindValue(":stock_id",stock_id);
       purch_csh.bindValue(":branch",branch_id);
       purch_csh.bindValue(":cash_depo",cash_depo_id);
       purch_csh.bindValue(":currency",currency_id);
       purch_csh.bindValue(":employee",cashier_id);
       if(!purch_csh.exec()){
           QMessageBox::warning(this,tr("error Updating"),purch_csh.lastError().text(),QMessageBox::Ok);
       qDebug()<<purch_csh.lastError();}
       }
       else if (ui->credit->isChecked()) {
        QSqlQuery purch_crdt(db);
        purch_crdt.prepare("INSERT INTO purchases(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,1,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
        purch_crdt.bindValue(":purch_id",purch_id);
        purch_crdt.bindValue(":rec_no",rec_no);
        purch_crdt.bindValue(":supp_id",supp_id);
        purch_crdt.bindValue(":total",total);
        purch_crdt.bindValue(":discount",discount);
        purch_crdt.bindValue(":total_after",total_after);
        purch_crdt.bindValue(":payment",pay_meth);
        purch_crdt.bindValue(":stock_id",stock_id);
        purch_crdt.bindValue(":branch",branch_id);
        purch_crdt.bindValue(":cash_depo",cash_depo_id);
        purch_crdt.bindValue(":currency",currency_id);
        purch_crdt.bindValue(":employee",cashier_id);
        if(!purch_crdt.exec()){
            QMessageBox::warning(this,tr("error Updating"),purch_crdt.lastError().text(),QMessageBox::Ok);
        qDebug()<<purch_crdt.lastError();}
       QSqlQuery supp_bala(db);
       supp_bala.prepare("UPDATE supplier SET balance = balance + :total WHERE id = :supp_id");
       supp_bala.bindValue(":supp_id",supp_id);
       supp_bala.bindValue(":total",total_after);
       if(!supp_bala.exec()){
           QMessageBox::warning(this,tr("error Updating"),supp_bala.lastError().text(),QMessageBox::Ok);
       qDebug()<<supp_bala.lastError();}

       }
       else if(ui->cheque->isChecked()){
        QSqlQuery purch_chqs(db);
        purch_chqs.prepare("INSERT INTO purchases(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,2,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
        purch_chqs.bindValue(":purch_id",purch_id);
        purch_chqs.bindValue(":rec_no",rec_no);
        purch_chqs.bindValue(":supp_id",supp_id);
        purch_chqs.bindValue(":total",total);
        purch_chqs.bindValue(":discount",discount);
        purch_chqs.bindValue(":total_after",total_after);
        purch_chqs.bindValue(":payment",pay_meth);
        purch_chqs.bindValue(":stock_id",stock_id);
        purch_chqs.bindValue(":branch",branch_id);
        purch_chqs.bindValue(":cash_depo",cash_depo_id);
        purch_chqs.bindValue(":currency",currency_id);
        purch_chqs.bindValue(":employee",cashier_id);
        if(!purch_chqs.exec()){
            QMessageBox::warning(this,tr("error Updating"),purch_chqs.lastError().text(),QMessageBox::Ok);
        qDebug()<<purch_chqs.lastError();}
        QSqlQuery supp_bala(db);
        supp_bala.prepare("UPDATE supplier SET balance = balance + :total WHERE id = :supp_id");
        supp_bala.bindValue(":supp_id",supp_id);
        supp_bala.bindValue(":total",total_after);
        if(!supp_bala.exec()){
            QMessageBox::warning(this,tr("error Updating"),supp_bala.lastError().text(),QMessageBox::Ok);
        qDebug()<<supp_bala.lastError();}
       }
       }

    while (x<rowCount) {
       id = ui->tableWidget->item(x,0)->text();
       item_name = ui->tableWidget->item(x,1)->text();
       //item_int = ui->tableWidget->item(x,2)->text();
      // ex_bcode = ui->tableWidget->item(x,3)->text();
       buy_price = ui->tableWidget->item(x,2)->text();
       quan = ui->tableWidget->item(x,3)->text();
       total_price = ui->tableWidget->item(x,4)->text();
       //expire = ui->tableWidget->item(x,7)->text();
       sell_price = ui->tableWidget->item(x,5)->text();


       int recCount = 0;
       //item_balance
       QSqlQuery qry(db);
       qry.prepare("SELECT * FROM item_balance WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
       qry.bindValue(":ex_bcode",id);
       qry.bindValue(":stock",stock_id);
       qry.bindValue(":branch",branch_id);

       if( qry.exec() ){
           while (qry.next())
           {
               recCount++;
           }
       }

       else{
           QMessageBox::warning(this,tr("error 245245245 Updating"),qry.lastError().text(),QMessageBox::Ok);
             qDebug() << "Error inserting into the table:\n" << qry.lastError();
      }
       if(recCount==0){

       QSqlQuery bala(db);
       bala.prepare("INSERT INTO item_balance(sub_item_id,exp_barcode,storage_id,branch_id, item_balance,purchase_balance)VALUES(:id,:ex_bcode,:stock_id,:branch,:quan,:quan)");
       bala.bindValue(":id",id);
       bala.bindValue(":ex_bcode",id);
       bala.bindValue(":quan",quan);
       bala.bindValue(":branch",branch_id);
       bala.bindValue(":stock_id",stock_id);
       if(!bala.exec())
           QMessageBox::warning(this,tr("error 245245 Updating"),bala.lastError().text(),QMessageBox::Ok);
       }
       else{
       QSqlQuery bala2(db);
       bala2.prepare("UPDATE item_balance SET item_balance = item_balance + :quan ,purchase_balance = purchase_balance + :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
       bala2.bindValue(":ex_bcode",id);
       bala2.bindValue(":quan",quan);
       bala2.bindValue(":stock",stock_id);
       bala2.bindValue(":branch",branch_id);

       if(!bala2.exec())
           QMessageBox::warning(this,tr("error Updating"),bala2.lastError().text(),QMessageBox::Ok);
       }

                    QSqlQuery purchb(db);
                    purchb.prepare("INSERT INTO purchases_items(rec_no,item_name,exp_barcode,buy_price,quantity,total,sell_price)VALUES(:purch_id,:item_name,:ex_code,:prch_price,:prch_quan,:prch_total,:sell_price)");
                    purchb.bindValue(":purch_id",purch_id);
                    purchb.bindValue(":item_name",item_name);
                   // purchb.bindValue(":item_int",item_int);
                    purchb.bindValue(":ex_code",id);
                    purchb.bindValue(":prch_price",buy_price);
                    purchb.bindValue(":prch_quan",quan);
                    purchb.bindValue(":prch_total",total_price);
                    purchb.bindValue(":sell_price",sell_price);
                     if(!purchb.exec())
                        QMessageBox::warning(this,tr("error 245 Updating"),purchb.lastError().text(),QMessageBox::Ok);

        x++;
       }
       QSqlQuery qry5(db);
        qry5.prepare("Delete from purchases_g where rec_no = "+ui->rec_no->text()+" ");
        if (qry5.exec())
        { }else{
            QMessageBox::warning(this,tr("error Updating"),qry5.lastError().text(),QMessageBox::Ok);
        }
        QSqlQuery qry6(db);
        qry6.prepare("Delete from purchases_items_g where rec_no = "+ui->rec_no->text()+" ");
        if (qry6.exec())
        { }else{
            QMessageBox::warning(this,tr("error Updating"),qry6.lastError().text(),QMessageBox::Ok);
        }



    }
    else if(ui->rec_stat->currentIndex() == 1){



           qDebug()<<"return";
        int recCount2=0;
         QSqlQuery qry2(db);
         qry2.prepare("SELECT * FROM purchases_return WHERE rec_no = :purchrt_id");
         qry2.bindValue(":purchrt_id",purch_id);
         if( qry2.exec() ){
             while (qry2.next())
             {
                 recCount2++;
             }
         }

         else{
               QMessageBox::warning(this,tr("error Updating"),qry2.lastError().text(),QMessageBox::Ok);
        }
         if(recCount2==0){
             if(ui->cash->isChecked()){
             QSqlQuery purch_csh(db);
             purch_csh.prepare("INSERT INTO purchases_return(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,0,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
             purch_csh.bindValue(":purch_id",purch_id);
             purch_csh.bindValue(":rec_no",rec_no);
             purch_csh.bindValue(":supp_id",supp_id);
             purch_csh.bindValue(":total",total);
             purch_csh.bindValue(":discount",discount);
             purch_csh.bindValue(":total_after",total_after);
             purch_csh.bindValue(":payment",pay_meth);
             purch_csh.bindValue(":stock_id",stock_id);
             purch_csh.bindValue(":branch",branch_id);
             purch_csh.bindValue(":cash_depo",cash_depo_id);
             purch_csh.bindValue(":currency",currency_id);
             purch_csh.bindValue(":employee",cashier_id);
             if(!purch_csh.exec())
                 QMessageBox::warning(this,tr("error Updating"),purch_csh.lastError().text(),QMessageBox::Ok);
             }
             else if (ui->credit->isChecked()) {
              QSqlQuery purch_crdt(db);
              purch_crdt.prepare("INSERT INTO purchases_return(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,1,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
              purch_crdt.bindValue(":purch_id",purch_id);
              purch_crdt.bindValue(":rec_no",rec_no);
              purch_crdt.bindValue(":supp_id",supp_id);
              purch_crdt.bindValue(":total",total);
              purch_crdt.bindValue(":discount",discount);
              purch_crdt.bindValue(":total_after",total_after);
              purch_crdt.bindValue(":payment",pay_meth);
              purch_crdt.bindValue(":stock_id",stock_id);
              purch_crdt.bindValue(":branch",branch_id);
              purch_crdt.bindValue(":cash_depo",cash_depo_id);
              purch_crdt.bindValue(":currency",currency_id);
              purch_crdt.bindValue(":employee",cashier_id);
              if(!purch_crdt.exec())
                  QMessageBox::warning(this,tr("error Updating"),purch_crdt.lastError().text(),QMessageBox::Ok);
             QSqlQuery supp_bala(db);
             supp_bala.prepare("UPDATE supplier SET balance = balance - :total WHERE id = :supp_id");
             supp_bala.bindValue(":supp_id",supp_id);
             supp_bala.bindValue(":total",total_after);
             if(!supp_bala.exec())
                 QMessageBox::warning(this,tr("error Updating"),supp_bala.lastError().text(),QMessageBox::Ok);

             }
             else if(ui->cheque->isChecked()){
              QSqlQuery purch_chqs(db);
              purch_chqs.prepare("INSERT INTO purchases_return(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,2,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
              purch_chqs.bindValue(":purch_id",purch_id);
              purch_chqs.bindValue(":rec_no",rec_no);
              purch_chqs.bindValue(":supp_id",supp_id);
              purch_chqs.bindValue(":total",total);
              purch_chqs.bindValue(":discount",discount);
              purch_chqs.bindValue(":total_after",total_after);
              purch_chqs.bindValue(":payment",pay_meth);
              purch_chqs.bindValue(":stock_id",stock_id);
              purch_chqs.bindValue(":branch",branch_id);
              purch_chqs.bindValue(":cash_depo",cash_depo_id);
              purch_chqs.bindValue(":currency",currency_id);
              purch_chqs.bindValue(":employee",cashier_id);
              if(!purch_chqs.exec())
                  QMessageBox::warning(this,tr("error Updating"),purch_chqs.lastError().text(),QMessageBox::Ok);

              QSqlQuery supp_bala(db);
              supp_bala.prepare("UPDATE supplier SET balance = balance - :total WHERE id = :supp_id");
              supp_bala.bindValue(":supp_id",supp_id);
              supp_bala.bindValue(":total",total_after);
              if(!supp_bala.exec())
                  QMessageBox::warning(this,tr("error Updating"),supp_bala.lastError().text(),QMessageBox::Ok);
             }

          while (x<rowCount) {
               id = ui->tableWidget->item(x,0)->text();
               item_name = ui->tableWidget->item(x,1)->text();
               //item_int = ui->tableWidget->item(x,2)->text();
               buy_price = ui->tableWidget->item(x,2)->text();
               quan = ui->tableWidget->item(x,3)->text();
               total_price = ui->tableWidget->item(x,4)->text();
               sell_price = ui->tableWidget->item(x,5)->text();

               QSqlQuery bala2(db);
                   bala2.prepare("UPDATE item_balance SET item_balance = item_balance - :quan ,purchase_return_balance = purchase_return_balance + :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
                   bala2.bindValue(":ex_bcode",id);
                   bala2.bindValue(":quan",quan);
                   bala2.bindValue(":stock",stock_id);
                   bala2.bindValue(":branch",branch_id);
                   if(!bala2.exec())
                       QMessageBox::warning(this,tr("error Updating"),bala2.lastError().text(),QMessageBox::Ok);
                    QSqlQuery purchb(db);
                    purchb.prepare("INSERT INTO purchases_return_items(rec_no,item_name,exp_barcode,price,quantity,total)VALUES(:purch_id,:item_name,:ex_code,:prch_price,:prch_quan,:prch_total)");
                    purchb.bindValue(":purch_id",purch_id);
                    purchb.bindValue(":item_name",item_name);
                    //purchb.bindValue(":item_int",item_int);
                    purchb.bindValue(":ex_code",id);
                    purchb.bindValue(":prch_price",buy_price);
                    purchb.bindValue(":prch_quan",quan);
                    purchb.bindValue(":prch_total",total_price);
                     if(!purchb.exec())
                        QMessageBox::warning(this,tr("error Updating"),purchb.lastError().text(),QMessageBox::Ok);
                x++;
            }
                QSqlQuery qry5(db);
                qry5.prepare("Delete from purchases_return_g where rec_no = "+ui->rec_no->text()+" ");
                if (qry5.exec())
                { }else{
                    QMessageBox::warning(this,tr("error Updating"),qry5.lastError().text(),QMessageBox::Ok);
                }
                QSqlQuery qry6(db);
                qry6.prepare("Delete from purchases_return_items_g where rec_no = "+ui->rec_no->text()+" ");
                if (qry6.exec())
                { }else{
                    QMessageBox::warning(this,tr("error Updating"),qry6.lastError().text(),QMessageBox::Ok);
                }
         }
       }
        clearAll();
    }
    else if(ui->tableWidget_2->isVisible() && rowCount_g > 0){
    if(ui->rec_stat->currentIndex() == 0){

    int recCount2=0;
       QSqlQuery qry2(db);
       qry2.prepare("SELECT * FROM purchases WHERE rec_no = :purch_id");
       qry2.bindValue(":purch_id",purch_id);
       if( qry2.exec() ){
           while (qry2.next())
           {
               recCount2++;
           }
       }

       else{
             QMessageBox::warning(this,tr("error Updating"),qry2.lastError().text(),QMessageBox::Ok);
      }
       if(recCount2==0){
       if(ui->cash->isChecked()){
       QSqlQuery purch_csh(db);
       purch_csh.prepare("INSERT INTO purchases(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,0,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
       purch_csh.bindValue(":purch_id",purch_id);
       purch_csh.bindValue(":rec_no",rec_no);
       purch_csh.bindValue(":supp_id",supp_id);
       purch_csh.bindValue(":total",total);
       purch_csh.bindValue(":discount",discount);
       purch_csh.bindValue(":total_after",total_after);
       purch_csh.bindValue(":payment",pay_meth);
       purch_csh.bindValue(":stock_id",stock_id);
       purch_csh.bindValue(":branch",branch_id);
       purch_csh.bindValue(":cash_depo",cash_depo_id);
       purch_csh.bindValue(":currency",currency_id);
       purch_csh.bindValue(":employee",cashier_id);
       if(!purch_csh.exec())
           QMessageBox::warning(this,tr("error Updating"),purch_csh.lastError().text(),QMessageBox::Ok);
       }
       else if (ui->credit->isChecked()) {
        QSqlQuery purch_crdt(db);
        purch_crdt.prepare("INSERT INTO purchases(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,1,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
        purch_crdt.bindValue(":purch_id",purch_id);
        purch_crdt.bindValue(":rec_no",rec_no);
        purch_crdt.bindValue(":supp_id",supp_id);
        purch_crdt.bindValue(":total",total);
        purch_crdt.bindValue(":discount",discount);
        purch_crdt.bindValue(":total_after",total_after);
        purch_crdt.bindValue(":payment",pay_meth);
        purch_crdt.bindValue(":stock_id",stock_id);
        purch_crdt.bindValue(":branch",branch_id);
        purch_crdt.bindValue(":cash_depo",cash_depo_id);
        purch_crdt.bindValue(":currency",currency_id);
        purch_crdt.bindValue(":employee",cashier_id);
        if(!purch_crdt.exec())
            QMessageBox::warning(this,tr("error Updating"),purch_crdt.lastError().text(),QMessageBox::Ok);
       QSqlQuery supp_bala(db);
       supp_bala.prepare("UPDATE supplier SET balance = balance + :total WHERE id = :supp_id");
       supp_bala.bindValue(":supp_id",supp_id);
       supp_bala.bindValue(":total",total_after);
       if(!supp_bala.exec())
           QMessageBox::warning(this,tr("error Updating"),supp_bala.lastError().text(),QMessageBox::Ok);

       }
       else if(ui->cheque->isChecked()){
        QSqlQuery purch_chqs(db);
        purch_chqs.prepare("INSERT INTO purchases(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,2,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
        purch_chqs.bindValue(":purch_id",purch_id);
        purch_chqs.bindValue(":rec_no",rec_no);
        purch_chqs.bindValue(":supp_id",supp_id);
        purch_chqs.bindValue(":total",total);
        purch_chqs.bindValue(":discount",discount);
        purch_chqs.bindValue(":total_after",total_after);
        purch_chqs.bindValue(":payment",pay_meth);
        purch_chqs.bindValue(":stock_id",stock_id);
        purch_chqs.bindValue(":branch",branch_id);
        purch_chqs.bindValue(":cash_depo",cash_depo_id);
        purch_chqs.bindValue(":currency",currency_id);
        purch_chqs.bindValue(":employee",cashier_id);
        if(!purch_chqs.exec())
            QMessageBox::warning(this,tr("error Updating"),purch_chqs.lastError().text(),QMessageBox::Ok);
        QSqlQuery supp_bala(db);
        supp_bala.prepare("UPDATE supplier SET balance = balance + :total WHERE id = :supp_id");
        supp_bala.bindValue(":supp_id",supp_id);
        supp_bala.bindValue(":total",total_after);
        if(!supp_bala.exec())
            QMessageBox::warning(this,tr("error Updating"),supp_bala.lastError().text(),QMessageBox::Ok);
       }
       }

    while (x<rowCount_g) {
       id = ui->tableWidget_2->item(x,0)->text();
       item_name = ui->tableWidget_2->item(x,1)->text();
       //item_int = ui->tableWidget_2->item(x,2)->text();
       buy_price = ui->tableWidget_2->item(x,2)->text();
       quan = ui->tableWidget_2->item(x,3)->text();
       total_price = ui->tableWidget_2->item(x,4)->text();
       sell_price = ui->tableWidget_2->item(x,5)->text();


       int recCount = 0;
       //item_balance
       QSqlQuery qry(db);
       qry.prepare("SELECT * FROM item_balance WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
       qry.bindValue(":ex_bcode",id);
       qry.bindValue(":stock",stock_id);
       qry.bindValue(":branch",branch_id);

       if( qry.exec() ){
           while (qry.next())
           {
               recCount++;
           }
       }

       else{
             QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
      }
       if(recCount==0){

       QSqlQuery bala(db);
       bala.prepare("INSERT INTO item_balance(sub_item_id,exp_barcode,storage_id,branch_id, item_balance,purchase_balance)VALUES(:id,:ex_bcode,:stock_id,:branch,:quan,:quan)");
       bala.bindValue(":id",id);
       bala.bindValue(":ex_bcode",id);
       bala.bindValue(":quan",quan);
       bala.bindValue(":branch",branch_id);
       bala.bindValue(":stock_id",stock_id);
       if(!bala.exec())
           QMessageBox::warning(this,tr("error Updating"),bala.lastError().text(),QMessageBox::Ok);
       }
       else{
       QSqlQuery bala2(db);
       bala2.prepare("UPDATE item_balance SET item_balance = item_balance + :quan ,purchase_balance = purchase_balance + :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
       bala2.bindValue(":ex_bcode",id);
       bala2.bindValue(":quan",quan);
       bala2.bindValue(":stock",stock_id);
       bala2.bindValue(":branch",branch_id);

       if(!bala2.exec())
           QMessageBox::warning(this,tr("error Updating"),bala2.lastError().text(),QMessageBox::Ok);
       }
       //purch

       //else

                    QSqlQuery purchb(db);
                    purchb.prepare("INSERT INTO purchases_items(rec_no,item_name,exp_barcode,buy_price,quantity,total,sell_price)VALUES(:purch_id,:item_name,:ex_code,:prch_price,:prch_quan,:prch_total,:sell_price)");
                    purchb.bindValue(":purch_id",purch_id);
                    purchb.bindValue(":item_name",item_name);
                    //purchb.bindValue(":item_int",item_int);
                    purchb.bindValue(":ex_code",id);
                    purchb.bindValue(":prch_price",buy_price);
                    purchb.bindValue(":prch_quan",quan);
                    purchb.bindValue(":prch_total",total_price);
                    purchb.bindValue(":sell_price",sell_price);
                     if(!purchb.exec())
                        QMessageBox::warning(this,tr("error Updating345"),purchb.lastError().text(),QMessageBox::Ok);

        x++;
       }
                            QSqlQuery qry5(db);
                            qry5.prepare("Delete from purchases_g where rec_no = "+ui->rec_no->text()+" ");
                            if (qry5.exec())
                            { }else{
                                QMessageBox::warning(this,tr("error Updating"),qry5.lastError().text(),QMessageBox::Ok);
                            }
                            QSqlQuery qry6(db);
                            qry6.prepare("Delete from purchases_items_g where rec_no = "+ui->rec_no->text()+" ");
                            if (qry6.exec())
                            { }else{
                                QMessageBox::warning(this,tr("error Updating"),qry6.lastError().text(),QMessageBox::Ok);
                            }
    }
    else if(ui->rec_stat->currentIndex() == 1){



           qDebug()<<"return";
        int recCount2=0;
         QSqlQuery qry2(db);
         qry2.prepare("SELECT * FROM purchases_return WHERE rec_no = :purchrt_id");
         qry2.bindValue(":purchrt_id",purch_id);
         if( qry2.exec() ){
             while (qry2.next())
             {
                 recCount2++;
             }
         }

         else{
               QMessageBox::warning(this,tr("error Updating"),qry2.lastError().text(),QMessageBox::Ok);
        }
         if(recCount2==0){
             if(ui->cash->isChecked()){
             QSqlQuery purch_csh(db);
             purch_csh.prepare("INSERT INTO purchases_return(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,0,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
             purch_csh.bindValue(":purch_id",purch_id);
             purch_csh.bindValue(":rec_no",rec_no);
             purch_csh.bindValue(":supp_id",supp_id);
             purch_csh.bindValue(":total",total);
             purch_csh.bindValue(":discount",discount);
             purch_csh.bindValue(":total_after",total_after);
             purch_csh.bindValue(":payment",pay_meth);
             purch_csh.bindValue(":stock_id",stock_id);
             purch_csh.bindValue(":branch",branch_id);
             purch_csh.bindValue(":cash_depo",cash_depo_id);
             purch_csh.bindValue(":currency",currency_id);
             purch_csh.bindValue(":employee",cashier_id);
             if(!purch_csh.exec())
                 QMessageBox::warning(this,tr("error Updating"),purch_csh.lastError().text(),QMessageBox::Ok);
             }
             else if (ui->credit->isChecked()) {
              QSqlQuery purch_crdt(db);
              purch_crdt.prepare("INSERT INTO purchases_return(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,1,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
              purch_crdt.bindValue(":purch_id",purch_id);
              purch_crdt.bindValue(":rec_no",rec_no);
              purch_crdt.bindValue(":supp_id",supp_id);
              purch_crdt.bindValue(":total",total);
              purch_crdt.bindValue(":discount",discount);
              purch_crdt.bindValue(":total_after",total_after);
              purch_crdt.bindValue(":payment",pay_meth);
              purch_crdt.bindValue(":stock_id",stock_id);
              purch_crdt.bindValue(":branch",branch_id);
              purch_crdt.bindValue(":cash_depo",cash_depo_id);
              purch_crdt.bindValue(":currency",currency_id);
              purch_crdt.bindValue(":employee",cashier_id);
              if(!purch_crdt.exec())
                  QMessageBox::warning(this,tr("error Updating"),purch_crdt.lastError().text(),QMessageBox::Ok);
             QSqlQuery supp_bala(db);
             supp_bala.prepare("UPDATE supplier SET balance = balance - :total WHERE id = :supp_id");
             supp_bala.bindValue(":supp_id",supp_id);
             supp_bala.bindValue(":total",total_after);
             if(!supp_bala.exec())
                 QMessageBox::warning(this,tr("error Updating"),supp_bala.lastError().text(),QMessageBox::Ok);

             }
             else if(ui->cheque->isChecked()){
              QSqlQuery purch_chqs(db);
              purch_chqs.prepare("INSERT INTO purchases_return(rec_no,rec_custom,supplier_id,total,discount,payment_method_id,payment_method,storage_id,branch_id,cash_deposit_id,currency_id,employee_id)VALUES(:purch_id,:rec_no,:supp_id,:total_after,:discount,2,:payment,:stock_id,:branch,:cash_depo,:currency,:employee)");
              purch_chqs.bindValue(":purch_id",purch_id);
              purch_chqs.bindValue(":rec_no",rec_no);
              purch_chqs.bindValue(":supp_id",supp_id);
              purch_chqs.bindValue(":total",total);
              purch_chqs.bindValue(":discount",discount);
              purch_chqs.bindValue(":total_after",total_after);
              purch_chqs.bindValue(":payment",pay_meth);
              purch_chqs.bindValue(":stock_id",stock_id);
              purch_chqs.bindValue(":branch",branch_id);
              purch_chqs.bindValue(":cash_depo",cash_depo_id);
              purch_chqs.bindValue(":currency",currency_id);
              purch_chqs.bindValue(":employee",cashier_id);
              if(!purch_chqs.exec())
                  QMessageBox::warning(this,tr("error Updating"),purch_chqs.lastError().text(),QMessageBox::Ok);
              QSqlQuery supp_bala(db);
              supp_bala.prepare("UPDATE supplier SET balance = balance - :total WHERE id = :supp_id");
              supp_bala.bindValue(":supp_id",supp_id);
              supp_bala.bindValue(":total",total_after);
              if(!supp_bala.exec())
                  QMessageBox::warning(this,tr("error Updating"),supp_bala.lastError().text(),QMessageBox::Ok);
             }

          while (x<rowCount_g) {
               id = ui->tableWidget_2->item(x,0)->text();
               item_name = ui->tableWidget_2->item(x,1)->text();
               //item_int = ui->tableWidget_2->item(x,2)->text();
               buy_price = ui->tableWidget_2->item(x,4)->text();
               quan = ui->tableWidget_2->item(x,5)->text();
               total_price = ui->tableWidget_2->item(x,6)->text();
               sell_price = ui->tableWidget_2->item(x,8)->text();

               QSqlQuery bala2(db);
                   bala2.prepare("UPDATE item_balance SET item_balance = item_balance - :quan ,purchase_return_balance = purchase_return_balance + :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
                   bala2.bindValue(":ex_bcode",id);
                   bala2.bindValue(":quan",quan);
                   bala2.bindValue(":stock",stock_id);
                   bala2.bindValue(":branch",branch_id);
                   if(!bala2.exec())
                       QMessageBox::warning(this,tr("error Updating"),bala2.lastError().text(),QMessageBox::Ok);
                    QSqlQuery purchb(db);
                    purchb.prepare("INSERT INTO purchases_return_items(rec_no,item_name,exp_barcode,price,quantity,total)VALUES(:purch_id,:item_name,:ex_code,:prch_price,:prch_quan,:prch_total)");
                    purchb.bindValue(":purch_id",purch_id);
                    purchb.bindValue(":item_name",item_name);
                    //purchb.bindValue(":item_int",item_int);
                    purchb.bindValue(":ex_code",id);
                    purchb.bindValue(":prch_price",buy_price);
                    purchb.bindValue(":prch_quan",quan);
                    purchb.bindValue(":prch_total",total_price);
                     if(!purchb.exec())
                        QMessageBox::warning(this,tr("error Updating"),purchb.lastError().text(),QMessageBox::Ok);
                x++;
            }
                            QSqlQuery qry5(db);
                            qry5.prepare("Delete from purchases_return_g where rec_no = "+ui->rec_no->text()+" ");
                            if (qry5.exec())
                            { }else{
                                QMessageBox::warning(this,tr("error Updating"),qry5.lastError().text(),QMessageBox::Ok);
                            }
                            QSqlQuery qry6(db);
                            qry6.prepare("Delete from purchases_return_items_g where rec_no = "+ui->rec_no->text()+" ");
                            if (qry6.exec())
                            { }else{
                                QMessageBox::warning(this,tr("error Updating"),qry6.lastError().text(),QMessageBox::Ok);

                            }
         }
       }
        clearAll();


    }
}

void buy_win_cut::set_edit(){
    ui->price_sell->setReadOnly(false);
    ui->price_buy->setReadOnly(false);
    ui->total_rec->setReadOnly(false);
    ui->dicount_perc->setReadOnly(false);
    ui->discount->setReadOnly(false);
    ui->supp_name->lineEdit()->setReadOnly(false);
    ui->branch_name->lineEdit()->setReadOnly(false);
    ui->stock_name->lineEdit()->setReadOnly(false);
    ui->currency_name->lineEdit()->setReadOnly(false);
    ui->cashier_name->lineEdit()->setReadOnly(false);
    ui->cash_depo_name->lineEdit()->setReadOnly(false);
    ui->dateTimeEdit->setReadOnly(false);
    ui->item_name->lineEdit()->setReadOnly(false);
    ui->item_bcode->lineEdit()->setReadOnly(false);
    //ui->item_int->lineEdit()->setReadOnly(false);
    ui->quantity->setReadOnly(false);
    ui->cash->setDisabled(false);
    ui->credit->setDisabled(false);
    ui->cheque->setDisabled(false);

}
void buy_win_cut::set_unedit(){
    ui->price_sell->setReadOnly(true);
    ui->price_buy->setReadOnly(true);
    ui->total_rec->setReadOnly(true);
    ui->total_quan->setReadOnly(true);
    ui->total_after->setReadOnly(true);
    ui->dicount_perc->setReadOnly(true);
    ui->discount->setReadOnly(true);
    ui->supp_name->lineEdit()->setReadOnly(true);
    ui->branch_name->lineEdit()->setReadOnly(true);
    ui->stock_name->lineEdit()->setReadOnly(true);
    ui->currency_name->lineEdit()->setReadOnly(true);
    ui->cashier_name->lineEdit()->setReadOnly(true);
    ui->cash_depo_name->lineEdit()->setReadOnly(true);
    ui->dateTimeEdit->setReadOnly(true);
    ui->item_name->lineEdit()->setReadOnly(true);
    ui->item_bcode->lineEdit()->setReadOnly(true);
    //ui->item_int->lineEdit()->setReadOnly(true);
    ui->quantity->setReadOnly(true);
    ui->cash->setDisabled(true);
    ui->credit->setDisabled(true);
    ui->cheque->setDisabled(true);
}

void buy_win_cut::on_quantity_returnPressed()
{

    if(!ui->item_name->lineEdit()->text().isEmpty()){
    setTable();
    clearLines();}
    else{
        ui->item_name->lineEdit()->setFocus();
    }


}

void buy_win_cut::on_price_buy_returnPressed()
{
    ui->price_sell->setFocus();
    ui->price_sell->selectAll();
}

void buy_win_cut::on_price_sell_returnPressed()
{
    if(!ui->item_name->lineEdit()->text().isEmpty()){
    setTable();
    clearLines();}
    else{
        ui->item_name->lineEdit()->setFocus();
    }
}



void buy_win_cut::on_discount_editingFinished()
{
    double total_b = ui->total_rec->text().toDouble();
    double disc = ui->discount->text().toDouble();
    double total = total_b - disc;
    double perc = (disc / total_b)*100;
    ui->total_after->setText(QString::number(total));
    ui->dicount_perc->setText(QString::number(perc));
}
void buy_win_cut::on_total_rec_textChanged(const QString &arg1)
{
    double total_b = ui->total_rec->text().toDouble();
    double disc = ui->discount->text().toDouble();
    double total = total_b - disc;
    double perc = (disc / total_b)*100;
    ui->total_after->setText(QString::number(total));
    ui->dicount_perc->setText(QString::number(perc));
}

void buy_win_cut::on_discount_returnPressed()
{
    double total_b = ui->total_rec->text().toDouble();
    double disc = ui->discount->text().toDouble();
    double total = total_b - disc;
    double perc = (disc / total_b)*100;
    ui->total_after->setText(QString::number(total));
    ui->dicount_perc->setText(QString::number(perc));
}

void buy_win_cut::on_dicount_perc_editingFinished()
{
    double total_b = ui->total_rec->text().toDouble();
    double perc = ui->dicount_perc->text().toDouble()/100;
    double disc = perc * total_b;
    double total = total_b - disc;
    ui->total_after->setText(QString::number(total));
    ui->discount->setText(QString::number(disc));
}

void buy_win_cut::on_dicount_perc_returnPressed()
{
    double total_b = ui->total_rec->text().toDouble();
    double perc = ui->dicount_perc->text().toDouble()/100;
    double disc = perc * total_b;
    double total = total_b - disc;
    ui->total_after->setText(QString::number(total));
    ui->discount->setText(QString::number(disc));
}


void buy_win_cut::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    /*if(item->column()==3&&ui->tableWidget->rowCount()!=0){
        getTotal_items();
        double price = ui->tableWidget->item(item->row(),2)->text().toDouble();
        double quan = ui->tableWidget->item(item->row(),3)->text().toDouble();
        double total = price * quan;
        QTableWidgetItem *total1;
        total1 = new QTableWidgetItem;
        total1->setData(0,total);
        ui->tableWidget->setItem(item->row(),5,total1);
        getTotal();
    }*/
}

void buy_win_cut::on_save_clicked()
{
    save_items();
}

void buy_win_cut::on_saved_clicked()
{
    set_unedit();
    saved = true;
    if(ui->rec_stat->currentIndex()==0){
    QSqlQuery qry(db);
    QSqlQuery qry1(db);
    QSqlQuery qry2(db);
    QString test8;
    QDateTime date;
    QString payment;
    qry.prepare("SELECT MAX(rec_no) from purchases");
    if (qry.exec())
    {
        while (qry.next())
        {
            test8 = QString::number(qry.value(0).toInt());

        }
    }
    QSqlTableModel *GetTable = new QSqlTableModel(this,db);
    GetTable->setTable("purchases_items");
    GetTable->setFilter("rec_no = "+test8+" ");

    ui->tableWidget->hide();
    ui->tableWidget_2->hide();
    ui->tableView_2->hide();
    ui->tableView->show();
    ui->tableView->setModel(GetTable);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(3);
    ui->tableView->hideColumn(9);
    GetTable->select();

    ui->rec_no->setText(test8);
    qry2.prepare("SELECT create_date,payment_method,supplier_id,total,discount,storage_id,branch_id,currency_id,employee_id,cash_deposit_id,rec_custom from purchases WHERE rec_no = "+test8+" ");
    if (qry2.exec())
    {
        while (qry2.next())
        {
            date = qry2.value(0).toDateTime();
            payment = qry2.value(1).toString();
            model->setQuery("SELECT name ,id FROM supplier WHERE id = "+qry2.value(2).toString()+" ",db);
            ui->discount->setText(qry2.value(4).toString());
            ui->total_after->setText(qry2.value(3).toString());
            ui->total_rec->setText(QString::number(qry2.value(3).toDouble()+qry2.value(4).toDouble()));
            model2->setQuery("SELECT name ,id FROM storage WHERE id = "+qry2.value(5).toString()+" ",db);
            model10->setQuery("SELECT name ,id FROM branch WHERE id = "+qry2.value(6).toString()+" ",db);
            model8->setQuery("SELECT name ,id FROM currency WHERE id = "+qry2.value(7).toString()+" ",db);
            model1->setQuery("SELECT name ,id FROM employees WHERE id = "+qry2.value(8).toString()+" ",db);
            model3->setQuery("SELECT name ,id FROM cash_deposit WHERE id = "+qry2.value(9).toString()+" ",db);
            ui->custom_id->setText(qry2.value(10).toString());
               ui->supp_name->setCurrentIndex(0);
               ui->stock_name->setCurrentIndex(0);
               ui->branch_name->setCurrentIndex(0);
               ui->cashier_name->setCurrentIndex(0);
               ui->currency_name->setCurrentIndex(0);
               ui->cash_depo_name->setCurrentIndex(0);

        }
    }
    else{qDebug() << "Error selectttt1 into the table:\n" << qry2.lastError();}
    ui->dateTimeEdit->setDateTime(date);
    if(payment == "Cash"){
        ui->cash->setChecked(true);

    }
    else if(payment == "Credit"){
        ui->credit->setChecked(true);
    }
    else if (payment == "Cheque") {
        ui->cheque->setChecked(true);
    }
    double item_count = 0;
    qry1.prepare("SELECT quantity FROM purchases_items WHERE rec_no = "+test8+"");
    if (qry1.exec())
    {
        while (qry1.next())
        {
            item_count += qry1.value(0).toDouble();
        }
    }
    ui->total_quan->setText(QString::number(item_count));


    }
    else if (ui->rec_stat->currentIndex()==1){
        QSqlQuery qry(db);
        QSqlQuery qry2(db);
        int test7;
        QString test8;
        QDateTime date;
        QString payment;
        qry.prepare("SELECT MAX(rec_no) from purchases_return");
        if (qry.exec())
        {
            while (qry.next())
            {
                test7 = qry.value(0).toInt();
                test8 = QString::number(test7);

            }
        }
        QSqlTableModel *GetTable = new QSqlTableModel(this,db);
        GetTable->setTable("purchases_return_items");
        GetTable->setFilter("rec_no = "+test8+" ");
        ui->tableWidget->hide();
        ui->tableWidget_2->hide();
        ui->tableView->hide();
        ui->tableView_2->show();
        ui->tableView_2->setModel(GetTable);
        GetTable->select();
        ui->rec_no->setText(test8);
        ui->tableView_2->hideColumn(0);
        ui->tableView_2->hideColumn(1);
        ui->tableView_2->hideColumn(3);
        ui->tableView_2->hideColumn(8);
        qry2.prepare("SELECT create_date,payment_method,supplier_id,total,discount,storage_id,branch_id,currency_id,employee_id,cash_deposit_id,rec_custom from purchases_return WHERE rec_no = "+test8+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                date = qry2.value(0).toDateTime();
                payment = qry2.value(1).toString();
                model->setQuery("SELECT name ,id FROM supplier WHERE id = "+qry2.value(2).toString()+" ",db);
                ui->discount->setText(qry2.value(4).toString());
                ui->total_after->setText(qry2.value(3).toString());
                ui->total_rec->setText(QString::number(qry2.value(3).toDouble()+qry2.value(4).toDouble()));
                model2->setQuery("SELECT name ,id FROM storage WHERE id = "+qry2.value(5).toString()+" ",db);
                model10->setQuery("SELECT name ,id FROM branch WHERE id = "+qry2.value(6).toString()+" ",db);
                model8->setQuery("SELECT name ,id FROM currency WHERE id = "+qry2.value(7).toString()+" ",db);
                model1->setQuery("SELECT name ,id FROM employees WHERE id = "+qry2.value(8).toString()+" ",db);
                model3->setQuery("SELECT name ,id FROM cash_deposit WHERE id = "+qry2.value(9).toString()+" ",db);
                ui->custom_id->setText(qry2.value(10).toString());
                   ui->supp_name->setCurrentIndex(0);
                   ui->stock_name->setCurrentIndex(0);
                   ui->branch_name->setCurrentIndex(0);
                   ui->cashier_name->setCurrentIndex(0);
                   ui->currency_name->setCurrentIndex(0);
                   ui->cash_depo_name->setCurrentIndex(0);
            }
        }
        else{
            qDebug()<<qry2.lastError();
        }
        ui->dateTimeEdit->setDateTime(date);
        if(payment == "Cash"){
            ui->cash->setChecked(true);

        }
        else if(payment == "Credit"){
            ui->credit->setChecked(true);
        }
        else if (payment == "Cheque") {
            ui->cheque->setChecked(true);
        }
        double item_count = 0;
        QSqlQuery qry1(db);
        qry1.prepare("SELECT quantity FROM purchases_return_items WHERE rec_no = "+test8+"");
        if (qry1.exec())
        {
            while (qry1.next())
            {
                item_count += qry1.value(0).toDouble();
            }
        }
        ui->total_quan->setText(QString::number(item_count));


    }
}

void buy_win_cut::on_rec_no_textChanged(const QString &arg1)
{
    if(ui->rec_stat->currentIndex()==0&&!ui->tableView->isHidden()){
        QSqlQuery qry2(db);
        QDateTime date;
        QString payment;
    QSqlTableModel *GetTable = new QSqlTableModel(this,db);
    GetTable->setTable("purchases_items");
    GetTable->setFilter("rec_no = "+ui->rec_no->text()+" ");
    ui->tableView->setModel(GetTable);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(3);
    ui->tableView->hideColumn(9);
    GetTable->select();
    QString id = ui->rec_no->text();
    qDebug()<<id;
    qry2.prepare("SELECT create_date,payment_method,supplier_id,total,discount,storage_id,branch_id,currency_id,employee_id,cash_deposit_id,rec_custom from purchases WHERE rec_no = "+id+" ");
    if (qry2.exec())
    {
        while (qry2.next())
        {
            date = qry2.value(0).toDateTime();
            payment = qry2.value(1).toString();
            model->setQuery("SELECT name ,id FROM supplier WHERE id = "+qry2.value(2).toString()+" ",db);
            ui->discount->setText(qry2.value(4).toString());
            ui->total_after->setText(qry2.value(3).toString());
            ui->total_rec->setText(QString::number(qry2.value(3).toDouble()+qry2.value(4).toDouble()));
            model2->setQuery("SELECT name ,id FROM storage WHERE id = "+qry2.value(5).toString()+" ",db);
            model10->setQuery("SELECT name ,id FROM branch WHERE id = "+qry2.value(6).toString()+" ",db);
            model8->setQuery("SELECT name ,id FROM currency WHERE id = "+qry2.value(7).toString()+" ",db);
            model1->setQuery("SELECT name ,id FROM employees WHERE id = "+qry2.value(8).toString()+" ",db);
            model3->setQuery("SELECT name ,id FROM cash_deposit WHERE id = "+qry2.value(9).toString()+" ",db);
            ui->custom_id->setText(qry2.value(10).toString());
               ui->supp_name->setCurrentIndex(0);
               ui->stock_name->setCurrentIndex(0);
               ui->branch_name->setCurrentIndex(0);
               ui->cashier_name->setCurrentIndex(0);
               ui->currency_name->setCurrentIndex(0);
               ui->cash_depo_name->setCurrentIndex(0);
        }
    }
    else{qDebug() << "Error inserting into the table6969:\n" << qry2.lastError();}
    ui->dateTimeEdit->setDateTime(date);
    if(payment == "Cash"){
        ui->cash->setChecked(true);

    }
    else if(payment == "Credit"){
        ui->credit->setChecked(true);
    }
    else if (payment == "Cheque") {
        ui->cheque->setChecked(true);
    }
    double item_count = 0;
    QSqlQuery qry1(db);
    qry1.prepare("SELECT quantity FROM purchases_items WHERE rec_no = "+id+"");
    if (qry1.exec())
    {
        while (qry1.next())
        {
            item_count += qry1.value(0).toDouble();
        }
    }
    ui->total_quan->setText(QString::number(item_count));
    }
    else if(ui->rec_stat->currentIndex()==0&&ui->tableWidget_2->isVisible()){
        ui->tableWidget_2->clear();
        ui->tableWidget_2->setRowCount(0);
        QSqlQuery qry1(db);
        QSqlQuery qry2(db);
        QString id = ui->rec_no->text();
        QDateTime date;
        int payment;


        model->setQuery("SELECT name,id FROM supplier",db);
        model1->setQuery("SELECT name,id FROM employees WHERE appear_as_cashier = 1",db);
        model2->setQuery("SELECT name,id FROM storage ",db);
        model10->setQuery("SELECT name,id FROM branch ",db);
        model8->setQuery("SELECT name,id FROM currency ",db);
        model3->setQuery("SELECT name,id FROM cash_deposit",db);
        qry1.prepare("SELECT item_name,exp_barcode,quantity,buy_price,total,sell_price from purchases_items_g where rec_no = "+id+" ");
        if (qry1.exec())
        {
            while (qry1.next())
            {
                QTableWidgetItem *item_name = new QTableWidgetItem();
                //QTableWidgetItem *int_code = new QTableWidgetItem();
                QTableWidgetItem *barcode = new QTableWidgetItem();
                QTableWidgetItem *quan = new QTableWidgetItem();
                QTableWidgetItem *buy = new QTableWidgetItem();
                QTableWidgetItem *sell = new QTableWidgetItem();
                QTableWidgetItem *total = new QTableWidgetItem();

                item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);
                item_name->setCheckState(Qt::Unchecked);
                //int_code->setFlags(int_code->flags() & ~Qt::ItemIsEditable);
                barcode->setFlags(barcode->flags() & ~Qt::ItemIsEditable);
                buy->setFlags(buy->flags() & ~Qt::ItemIsEditable);
                sell->setFlags(sell->flags() & ~Qt::ItemIsEditable);
                total->setFlags(total->flags() & ~Qt::ItemIsEditable);
                quan->setFlags(total->flags() & Qt::ItemIsEditable);

                item_name->setText(qry1.value(0).toString());
                //int_code->setText(qry1.value(1).toString());
                barcode->setText(qry1.value(1).toString());
                quan->setText(qry1.value(2).toString());
                buy->setText(qry1.value(3).toString());
                total->setText(qry1.value(4).toString());
                sell->setText(qry1.value(5).toString());

                ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 0, barcode);
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 1, item_name);
                //ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 2, int_code);
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 2, buy);
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 3, quan);
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 4, total);
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 5, sell);
            }
        }
        ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"باركود"<<"اسم الصنف"<<"سعر الشراء"<<"كمية"<<"مجموع"<<"سعرالبيع");
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        qry2.prepare("SELECT create_date,payment_method,supplier_id,total,discount,employee_id,storage_id,branch_id,currency_id,cash_deposit_id,rec_custom from purchases_g WHERE rec_no = "+id+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                date = qry2.value(0).toDateTime();
                payment = qry2.value(1).toInt();
                ui->discount->setText(qry2.value(4).toString());
                ui->total_after->setText(qry2.value(3).toString());
                ui->total_rec->setText(QString::number(qry2.value(3).toDouble()+qry2.value(4).toDouble()));
                ui->custom_id->setText(qry2.value(10).toString());
                QList<QModelIndex> modelIndexList;
                modelIndexList.clear();
                modelIndexList = model->match(model->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(2).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->supp_name->setCurrentIndex(modelIndexList.first().row());
                }
                modelIndexList.clear();
                modelIndexList = model1->match(model1->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(5).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->cashier_name->setCurrentIndex(modelIndexList.first().row());
                }
                modelIndexList.clear();
                modelIndexList = model2->match(model2->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(6).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->stock_name->setCurrentIndex(modelIndexList.first().row());
                }
                modelIndexList.clear();
                modelIndexList = model10->match(model10->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(7).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->branch_name->setCurrentIndex(modelIndexList.first().row());
                }
                modelIndexList.clear();
                modelIndexList = model8->match(model8->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(8).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->currency_name->setCurrentIndex(modelIndexList.first().row());
                }
                modelIndexList.clear();
                modelIndexList = model3->match(model3->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(9).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->cash_depo_name->setCurrentIndex(modelIndexList.first().row());
                }

            }
        }
        ui->dateTimeEdit->setDateTime(date);
        if(payment == 0){
            ui->cash->setChecked(true);

        }
        else if(payment == 1){
            ui->credit->setChecked(true);
        }
        else if (payment == 2) {
            ui->cheque->setChecked(true);
        }
        getTotal_items();
        getTotal();



    }
    else if (ui->rec_stat->currentIndex()==1&&!ui->tableView_2->isHidden()){
        QSqlQuery qry2(db);
        QDateTime date;
        QString payment;
        QSqlTableModel *GetTable = new QSqlTableModel(this,db);
        GetTable->database();
        GetTable->setTable("purchases_return_items");
        GetTable->setFilter("rec_no = "+ui->rec_no->text()+" ");
        ui->tableView_2->setModel(GetTable);
        ui->tableView_2->hideColumn(0);
        ui->tableView_2->hideColumn(1);
        ui->tableView_2->hideColumn(2);
        ui->tableView_2->hideColumn(8);
        GetTable->select();
        QString id = ui->rec_no->text();
        qry2.prepare("SELECT create_date,payment_method,supplier_id,total,discount,storage_id,branch_id,currency_id,employee_id,cash_deposit_id,rec_custom from purchases_return WHERE rec_no = "+id+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                date = qry2.value(0).toDateTime();
                payment = qry2.value(1).toString();
                model->setQuery("SELECT name ,id FROM supplier WHERE id = "+qry2.value(2).toString()+" ",db);
                ui->discount->setText(qry2.value(4).toString());
                ui->total_after->setText(qry2.value(3).toString());
                ui->total_rec->setText(QString::number(qry2.value(3).toDouble()+qry2.value(4).toDouble()));
                model2->setQuery("SELECT name ,id FROM storage WHERE id = "+qry2.value(5).toString()+" ",db);
                model10->setQuery("SELECT name ,id FROM branch WHERE id = "+qry2.value(6).toString()+" ",db);
                model8->setQuery("SELECT name ,id FROM currency WHERE id = "+qry2.value(7).toString()+" ",db);
                model1->setQuery("SELECT name ,id FROM employees WHERE id = "+qry2.value(8).toString()+" ",db);
                model3->setQuery("SELECT name ,id FROM cash_deposit WHERE id = "+qry2.value(9).toString()+" ",db);
                ui->custom_id->setText(qry2.value(10).toString());
                   ui->supp_name->setCurrentIndex(0);
                   ui->stock_name->setCurrentIndex(0);
                   ui->branch_name->setCurrentIndex(0);
                   ui->cashier_name->setCurrentIndex(0);
                   ui->currency_name->setCurrentIndex(0);
                   ui->cash_depo_name->setCurrentIndex(0);
            }
        }
        else{
            qDebug()<<"here"<<qry2.lastError();
        }
        ui->dateTimeEdit->setDateTime(date);
        if(payment == "Cash"){
            ui->cash->setChecked(true);

        }
        else if(payment == "Credit"){
            ui->credit->setChecked(true);
        }
        else if (payment == "Cheque") {
            ui->cheque->setChecked(true);
        }
        double item_count = 0;
        QSqlQuery qry1(db);
        qry1.prepare("SELECT quantity FROM purchases_return_items WHERE rec_no = "+id+"");
        if (qry1.exec())
        {
            while (qry1.next())
            {
                item_count += qry1.value(0).toDouble();
            }
        }
        ui->total_quan->setText(QString::number(item_count));
    }
    else if(ui->rec_stat->currentIndex()==1&&ui->tableWidget_2->isVisible()){
        ui->tableWidget_2->clear();
        ui->tableWidget_2->setRowCount(0);
        QSqlQuery qry1(db);
        QSqlQuery qry2(db);
        QString id = ui->rec_no->text();
        QDateTime date;
        int payment;
        model->setQuery("SELECT name,id FROM supplier",db);
        model1->setQuery("SELECT name,id FROM employees WHERE appear_as_cashier = 1",db);
        model2->setQuery("SELECT name,id FROM storage ",db);
        model10->setQuery("SELECT name,id FROM branch ",db);
        model8->setQuery("SELECT name,id FROM currency ",db);
        model3->setQuery("SELECT name,id FROM cash_deposit",db);
        qry1.prepare("SELECT item_name,exp_barcode,quantity,price,total from purchases_return_items_g where rec_no = "+id+" ");
        if (qry1.exec())
        {
            while (qry1.next())
            {

                QTableWidgetItem *item_name = new QTableWidgetItem();
                //QTableWidgetItem *int_code = new QTableWidgetItem();
                QTableWidgetItem *barcode = new QTableWidgetItem();
                QTableWidgetItem *quan = new QTableWidgetItem();
                QTableWidgetItem *buy = new QTableWidgetItem();
                QTableWidgetItem *total = new QTableWidgetItem();

                item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);
                item_name->setCheckState(Qt::Unchecked);
                //int_code->setFlags(int_code->flags() & ~Qt::ItemIsEditable);
                barcode->setFlags(barcode->flags() & ~Qt::ItemIsEditable);
                buy->setFlags(buy->flags() & ~Qt::ItemIsEditable);
                total->setFlags(total->flags() & ~Qt::ItemIsEditable);
                quan->setFlags(total->flags() & Qt::ItemIsEditable);



                item_name->setText(qry1.value(0).toString());
                //int_code->setText(qry1.value(1).toString());
                barcode->setText(qry1.value(1).toString());
                quan->setText(qry1.value(2).toString());
                buy->setText(qry1.value(3).toString());
                total->setText(qry1.value(4).toString());

                ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 0, barcode);
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 1, item_name);
                //ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 2, int_code);
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 2, buy);
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 3, quan);
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 4, total);

            }
        }
        else{qDebug()<<qry1.lastError();}
        ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"باركود"<<"اسم الصنف"<<"سعر الشراء"<<"كمية"<<"مجموع");
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        qry2.prepare("SELECT create_date,payment_method,supplier_id,total,discount,employee_id,storage_id,branch_id,currency_id,cash_deposit_id,rec_custom from purchases_return_g WHERE rec_no = "+id+" ");
        if (qry2.exec())
        {
            while (qry2.next())
            {
                date = qry2.value(0).toDateTime();
                payment = qry2.value(1).toInt();
                ui->discount->setText(qry2.value(4).toString());
                ui->total_after->setText(qry2.value(3).toString());
                ui->total_rec->setText(QString::number(qry2.value(3).toDouble()+qry2.value(4).toDouble()));
                ui->custom_id->setText(qry2.value(10).toString());
                QList<QModelIndex> modelIndexList;
                modelIndexList.clear();
                modelIndexList = model->match(model->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(2).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->supp_name->setCurrentIndex(modelIndexList.first().row());
                }
                modelIndexList.clear();
                modelIndexList = model1->match(model1->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(5).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->cashier_name->setCurrentIndex(modelIndexList.first().row());
                }
                modelIndexList.clear();
                modelIndexList = model2->match(model2->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(6).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->stock_name->setCurrentIndex(modelIndexList.first().row());
                }
                modelIndexList.clear();
                modelIndexList = model10->match(model10->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(7).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->branch_name->setCurrentIndex(modelIndexList.first().row());
                }
                modelIndexList.clear();
                modelIndexList = model8->match(model8->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(8).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->currency_name->setCurrentIndex(modelIndexList.first().row());
                }
                modelIndexList.clear();
                modelIndexList = model3->match(model3->index(0, 1), Qt::DisplayRole, QVariant::fromValue(qry2.value(9).toString()), -1, Qt::MatchExactly);
                if(!modelIndexList.isEmpty()){
                    ui->cash_depo_name->setCurrentIndex(modelIndexList.first().row());
                }

            }
        }
        ui->dateTimeEdit->setDateTime(date);
        if(payment == 0){
            ui->cash->setChecked(true);

        }
        else if(payment == 1){
            ui->credit->setChecked(true);
        }
        else if (payment == 2) {
            ui->cheque->setChecked(true);
        }
        getTotal_items();
        getTotal();


    }

}

void buy_win_cut::on_pushButton_clicked()
{
    if(!searching){
    if(ui->rec_stat->currentIndex()==0){
    QSqlQuery qry(db);
    int save_in;
    int unsave_in;
    QString id = ui->rec_no->text();
    qry.prepare("SELECT MIN(rec_no) from purchases");
    if (qry.exec())
    {
        while (qry.next())
        {

            save_in = qry.value(0).toInt();
        }
    }
    qry.prepare("SELECT MIN(rec_no) from purchases_g");
    if (qry.exec())
    {
        while (qry.next())
        {

            unsave_in = qry.value(0).toInt();
        }
    }
    if(!ui->tableView->isHidden()&&ui->rec_no->text().toInt()>save_in){
        QSqlQuery qry(db);
        qry.prepare("SELECT rec_no FROM purchases where rec_no<"+id+" order by rec_no desc limit 1");
        if (qry.exec())
        {
            while (qry.next())
            {
                ui->rec_no->setText(qry.value(0).toString());
            }
        }

    }
    else if(!ui->tableWidget_2->isHidden()&&ui->rec_no->text().toInt()>unsave_in){
        QSqlQuery qry(db);
        qry.prepare("SELECT rec_no FROM purchases_g where rec_no<"+id+" order by rec_no desc limit 1");
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
        qry.prepare("SELECT MIN(rec_no) from purchases_return");
        if (qry.exec())
        {
            while (qry.next())
            {

                save_in = qry.value(0).toInt();
            }
        }
        qry.prepare("SELECT MIN(rec_no) from purchases_return_g");
        if (qry.exec())
        {
            while (qry.next())
            {

                unsave_in = qry.value(0).toInt();
            }
        }
        if(!ui->tableView_2->isHidden()&&ui->rec_no->text().toInt()>save_in){
            QSqlQuery qry(db);
            qry.prepare("SELECT rec_no FROM purchases_return where rec_no<"+id+" order by rec_no desc limit 1");
            if (qry.exec())
            {
                while (qry.next())
                {
                    ui->rec_no->setText(qry.value(0).toString());
                }
            }
        }
        else if(!ui->tableWidget_2->isHidden()&&ui->rec_no->text().toInt()>unsave_in){
            QSqlQuery qry(db);
            qry.prepare("SELECT rec_no FROM purchases_return_g where rec_no<"+id+" order by rec_no desc limit 1");
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
        if(cur_search_index > 0){
            cur_search_index -= 1;
            ui->rec_no->setText(QString::number(search_list.at(cur_search_index)));
        }
    }


}
void buy_win_cut::on_pushButton_2_clicked()
{
   if(!searching){
    if(ui->rec_stat->currentIndex()==0){
    QSqlQuery qry(db);
    int save_in;
    int unsave_in;
    QString id = ui->rec_no->text();
    qry.prepare("SELECT MAX(rec_no) from purchases");
    if (qry.exec())
    {
        while (qry.next())
        {

            save_in = qry.value(0).toInt();
        }
    }
    qry.prepare("SELECT MAX(rec_no) from purchases_g");
    if (qry.exec())
    {
        while (qry.next())
        {

            unsave_in = qry.value(0).toInt();
        }
    }
    if(!ui->tableView->isHidden()&&ui->rec_no->text().toInt()<save_in){
        QSqlQuery qry(db);
        qry.prepare("SELECT rec_no FROM purchases where rec_no>"+id+" order by rec_no asc limit 1");
        if (qry.exec())
        {
            while (qry.next())
            {
                ui->rec_no->setText(qry.value(0).toString());
            }
        }

    }
    else if(!ui->tableWidget_2->isHidden()&&ui->rec_no->text().toInt()<unsave_in){
        QSqlQuery qry(db);
        qry.prepare("SELECT rec_no FROM purchases_g where rec_no>"+id+" order by rec_no asc limit 1");
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
        qry.prepare("SELECT MAX(rec_no) from purchases_return");
        if (qry.exec())
        {
            while (qry.next())
            {

                save_in = qry.value(0).toInt();
            }
        }
        qry.prepare("SELECT MAX(rec_no) from purchases_return_g");
        if (qry.exec())
        {
            while (qry.next())
            {

                unsave_in = qry.value(0).toInt();
            }
        }
        if(!ui->tableView_2->isHidden()&&ui->rec_no->text().toInt()<save_in){
            QSqlQuery qry(db);
            qry.prepare("SELECT rec_no FROM purchases_return where rec_no>"+id+" order by rec_no asc limit 1");
            if (qry.exec())
            {
                while (qry.next())
                {
                    ui->rec_no->setText(qry.value(0).toString());
                }
            }
        }
        else if(!ui->tableWidget_2->isHidden()&&ui->rec_no->text().toInt()<unsave_in){
            QSqlQuery qry(db);
            qry.prepare("SELECT rec_no FROM purchases_return_g where rec_no>"+id+" order by rec_no asc limit 1");
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
void buy_win_cut::on_pushButton_4_clicked()
{
    clearAll();

}

void buy_win_cut::on_pushButton_5_clicked()
{
    print_save();
}




void buy_win_cut::on_rec_stat_currentIndexChanged(int index)
{
    int saved_count;
    int unsaved_count;
    if(index==0){
        if(ui->tableWidget->isVisible()){
        db.close();
        db.open();
        QSqlQuery qry(db);
        qry.prepare("SELECT MAX(rec_no) from purchases");
        if (qry.exec())
        {
            while (qry.next())
            {
              saved_count = qry.value(0).toInt();
            }
        }
        QSqlQuery qry2(db);
        qry2.prepare("SELECT MAX(rec_no) from purchases_g");
        if (qry2.exec())
        {
            while (qry2.next())
            {
              unsaved_count = qry2.value(0).toInt();
            }
        }
        if(saved_count>unsaved_count){
            saved_count+=1;
            ui->rec_no->setText(QString::number(saved_count));
        }
        else if(unsaved_count>saved_count){
            unsaved_count+=1;
            ui->rec_no->setText(QString::number(unsaved_count));
        }
        else if (unsaved_count==saved_count){
             saved_count+=1;
             ui->rec_no->setText(QString::number(saved_count));
         }else{
             QMessageBox::warning(this,tr("FATEL ERROR"),tr("ERROR IN RECORD NUMBER; FETCHING FAILED"),QMessageBox::Ok);
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
            qry.prepare("SELECT MAX(rec_no) from purchases");
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
       else if (ui->tableWidget_2->isVisible()) {
            db.close();
            db.open();
            ui->tableWidget->clear();
            QSqlQuery qry(db);
            int test7;
            QString test8;
            qry.prepare("SELECT MAX(rec_no) from purchases_g");
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
     if(ui->tableWidget->isVisible()){
     qDebug()<<"Salesrt";
     db.close();
     db.open();
     QSqlQuery qry(db);
     qry.prepare("SELECT MAX(rec_no) from purchases_return");
     if (qry.exec())
     {
         while (qry.next())
         {
           saved_count = qry.value(0).toInt();
         }
     }
     QSqlQuery qry2(db);
     qry2.prepare("SELECT MAX(rec_no) from purchases_return_g");
     if (qry2.exec())
     {
         while (qry2.next())
         {
           unsaved_count = qry2.value(0).toInt();
         }
     }
     if(saved_count>unsaved_count){
         saved_count+=1;
         ui->rec_no->setText(QString::number(saved_count));
     }
     else if(unsaved_count>saved_count){
         unsaved_count+=1;
         ui->rec_no->setText(QString::number(unsaved_count));
     }
     else if (unsaved_count==saved_count){
          saved_count+=1;
          ui->rec_no->setText(QString::number(saved_count));
      }else{
          QMessageBox::warning(this,tr("FATEL ERROR"),tr("ERROR IN RECORD NUMBER; FETCHING FAILED"),QMessageBox::Ok);
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
         qry.prepare("SELECT MAX(rec_no) from purchases_return");
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
     else if (ui->tableWidget_2->isVisible()) {
         db.close();
         db.open();
         ui->tableWidget->clear();
         QSqlQuery qry(db);
         qry.prepare("SELECT MAX(rec_no) from purchases_return_g");
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


void buy_win_cut::on_item_bcode_activated(const QString &arg1)
{
    setData_bcode();
}

void buy_win_cut::on_item_int_activated(const QString &arg1)
{
    //setData_int();
}

void buy_win_cut::on_un_save_clicked()
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
            QString quantity;
            QString storage_id;
            QString branch_id;
            QString supplier_id;
            double total;
            int payment_method;
            QSqlQuery qry3(db);
            QSqlQuery qry4(db);
            qry3.prepare("SELECT storage_id,branch_id,payment_method_id,total,supplier_id from purchases where rec_no = "+ui->rec_no->text()+" ");
            if (qry3.exec())
            {
                while (qry3.next())
                {
                    storage_id = qry3.value(0).toString();
                    branch_id = qry3.value(1).toString();
                    payment_method = qry3.value(2).toInt();
                    total = qry3.value(3).toDouble();
                    supplier_id = qry3.value(4).toString();
                }


            }else{
                qDebug()<<qry3.lastError().text()+"345343455";
            }
            if(payment_method == 1){
                qry4.prepare("UPDATE supplier SET balance = balance - :total WHERE id = :supp_id");
                qry4.bindValue(":total",total);
                qry4.bindValue(":supp_id",supplier_id);
                if (qry4.exec())
                qDebug()<<qry4.lastError();
            }
        QSqlQuery qry(db);
        qry.prepare("INSERT INTO purchases_g SELECT * FROM purchases WHERE rec_no = "+ui->rec_no->text()+" ");
        if (qry.exec())
        {
                        QSqlQuery qry2(db);
                        qry2.prepare("Delete from purchases where rec_no = "+ui->rec_no->text()+" ");
                        if (qry2.exec())
                        { }
        }
        else{
                        qDebug()<<qry.lastError().text()+"334545";
                    }
        QSqlQuery qry6(db);
        qry6.prepare("SELECT exp_barcode,quantity FROM purchases_items WHERE rec_no = "+ui->rec_no->text()+" ");
        if (qry6.exec())
        {
            while (qry6.next())
            {
                 code.append(qry6.value(0).toString());
                 quan.append(qry6.value(1).toString());

            }
        }
        else{
                        qDebug()<<qry6.lastError().text()+"345345";
                    }
        QSqlQuery qry5(db);
        qry5.prepare("INSERT INTO purchases_items_g SELECT NULL,`rec_no`,`item_name`,`item_int`,`exp_barcode`,`buy_price`,`quantity`,`total`,`sell_price`,`create` FROM purchases_items WHERE rec_no = "+ui->rec_no->text()+" ");
        if (qry5.exec())
        {
            QSqlQuery qry7(db);
            qry7.prepare("Delete from purchases_items where rec_no = "+ui->rec_no->text()+" ");
            if (qry7.exec())
            { }

        }
        else{
                        qDebug()<<qry5.lastError().text()+"345";
                    }




        int x = 0;
        while(x<code.size()){
            QString quan_in = quan.value(x);
            QString code_in = code.value(x);
           qDebug()<< quan_in << code_in;
        QSqlQuery qryx(db);
        qryx.prepare("UPDATE item_balance SET item_balance = item_balance - :quan ,purchase_balance = purchase_balance - :quan WHERE exp_barcode = :code");
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
        ui->tableWidget->hide();
        ui->tableView->hide();
        ui->tableView_2->hide();
        ui->tableWidget_2->show();
        ui->rec_no->setText(id);
      }
        else if(ui->rec_stat->currentIndex()==1){
            QString quantity;
            QString storage_id;
            QString branch_id;
            QString supplier_id;
            double total;
            int payment_method;
            QSqlQuery qry6(db);
            QSqlQuery qry7(db);
            qry6.prepare("SELECT storage_id,branch_id,payment_method_id,total,supplier_id from purchases where rec_no = "+ui->rec_no->text()+" ");
            if (qry6.exec())
            {
                while (qry6.next())
                {
                    storage_id = qry6.value(0).toString();
                    branch_id = qry6.value(1).toString();
                    payment_method = qry6.value(2).toInt();
                    total = qry6.value(3).toDouble();
                    supplier_id = qry6.value(4).toString();
                }


            }
            if(payment_method == 1){
                qry7.prepare("UPDATE supplier SET balance = balance + :total WHERE id = :supp_id");
                qry7.bindValue(":total",total);
                qry7.bindValue(":supp_id",supplier_id);
                if (qry7.exec())
                qDebug()<<qry7.lastError();
            }


            QSqlQuery qry(db);
            qry.prepare("INSERT INTO purchases_return_g SELECT * FROM purchases_return WHERE rec_no = "+ui->rec_no->text()+" ");
            if (qry.exec())
            {
                QSqlQuery qry2(db);
                qry2.prepare("Delete from purchases_return where rec_no = "+ui->rec_no->text()+" ");
                if (qry2.exec())
                { }

            }
            QSqlQuery qry4(db);
            qry4.prepare("SELECT barcode,quan FROM purchases_return_items WHERE rec_no = "+ui->rec_no->text()+" ");
            if (qry4.exec())
            {
                while (qry4.next())
                {
                     code.append(qry4.value(0).toString());
                     quan.append(qry4.value(1).toString());

                }
            }
            QSqlQuery qry3(db);
            qry3.prepare("INSERT INTO purchases_return_items_g SELECT NULL,`rec_no`,`item_name`,`item_int`,`exp_barcode`,`price`,`quantity`,`total`,`create` FROM purchases_return_items WHERE rec_no = "+ui->rec_no->text()+" ");
            if (qry3.exec())
            {

                QSqlQuery qry5(db);
                qry5.prepare("Delete from purchases_return_items where rec_no = "+ui->rec_no->text()+" ");
                if (qry5.exec())
                {}
            }
            int x = 0;
            while(x<code.size()){
                QString quan_in = quan.value(x);
                QString code_in = code.value(x);
               qDebug()<< quan_in << code_in;
            QSqlQuery qryx(db);
            qryx.prepare("UPDATE item_balance SET item_balance = item_balance + :quan ,purchase_return_balance = purchase_return_balance - :quan WHERE exp_barcode = :code");
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
            ui->tableWidget->hide();
            ui->tableView->hide();
            ui->tableView_2->hide();
            ui->tableWidget_2->show();
            ui->rec_no->setText(id);

        }



    }
    }
    code.clear();
    quan.clear();
}

void buy_win_cut::on_tableWidget_clicked(const QModelIndex &index)
{
    cur_row = index.row();
    qDebug()<<cur_row;
}

void buy_win_cut::on_tableWidget_2_clicked(const QModelIndex &index)
{
    cur_row = index.row();
    qDebug()<<cur_row;
}

void buy_win_cut::delete_row()
    {
    if(ui->tableWidget->isVisible()){
     ui->tableWidget->removeRow(cur_row);
     if(ui->tableWidget->rowCount()>0){
        getTotal();
        getTotal_items();
     }else {
         ui->total_rec->setText("0");
         ui->total_quan->setText("0");
         ui->total_after->setText("0");
        }
    }
    else if(ui->tableWidget_2->isVisible()){
        if(ui->rec_stat->currentIndex()==0){

            QString barcode = ui->tableWidget_2->item(cur_row,3)->text();
            QString quan = ui->tableWidget_2->item(cur_row,5)->text();
            QString rec = ui->rec_no->text();
            qDebug()<<barcode<<quan<<rec;
            QSqlQuery qry5(db);
            qry5.prepare("Delete from purchases_items_g where id IN ( SELECT id from purchases_items_g where rec_no = :rec AND exp_barcode = :code AND quantity = :quan LIMIT 1)");
            qry5.bindValue(":code",barcode);
            qry5.bindValue(":rec",rec);
            qry5.bindValue(":quan",quan);
            if (qry5.exec())
            { }
             ui->tableWidget_2->removeRow(cur_row);
             if(ui->tableWidget_2->rowCount()>0){
                 getTotal();
                 getTotal_items();
             }
             else {
                 ui->total_quan->setText("0");
                 ui->total_rec->setText("0");
                 ui->total_after->setText("0");
             }
        }
        else if(ui->rec_stat->currentIndex()==1){
            QString barcode = ui->tableWidget_2->item(cur_row,3)->text();
            QString quan = ui->tableWidget_2->item(cur_row,5)->text();
            QString rec = ui->rec_no->text();
            QSqlQuery qry5(db);
            qry5.prepare("Delete from purchases_return_items_g where id IN ( SELECT id from purchases_return_items_g where rec_no = :rec AND exp_barcode = :code AND quantity = :quan LIMIT 1)");
            qry5.bindValue(":code",barcode);
            qry5.bindValue(":rec",rec);
            qry5.bindValue(":quan",quan);
            if (qry5.exec())
            { }
            ui->tableWidget_2->removeRow(cur_row);
            if(ui->tableWidget_2->rowCount()>0){
                getTotal();
                getTotal_items();
            }
            else {
                ui->total_rec->setText("0");
                ui->total_quan->setText("0");
                ui->total_after->setText("0");
            }
        }

    }
}

void buy_win_cut::update_time()
{
    if(saved == false){
    ui->dateTimeEdit->setTime( QTime::currentTime() );
    }
}





void buy_win_cut::on_pushButton_3_clicked()
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
            QString supplier_id;
            double total;
            int payment_method;
            qry3.prepare("SELECT storage_id,branch_id,payment_method_id,total,supplier_id from purchases where rec_no = "+ui->rec_no->text()+" ");
            if (qry3.exec())
            {
                while (qry3.next())
                {
                    storage_id = qry3.value(0).toString();
                    branch_id = qry3.value(1).toString();
                    payment_method = qry3.value(2).toInt();
                    total = qry3.value(3).toDouble();
                    supplier_id = qry3.value(4).toString();
                }


            }
            if(payment_method == 1){
                qry4.prepare("UPDATE supplier SET balance = balance - :total WHERE id = :supp_id");
                qry4.bindValue(":total",total);
                qry4.bindValue(":supp_id",supplier_id);
                if (qry4.exec())
                qDebug()<<qry4.lastError();
            }

            qry1.prepare("SELECT exp_barcode,quantity from purchases_items where rec_no = "+ui->rec_no->text()+" ");
            if (qry1.exec())
            {
                while (qry1.next())
                {
                    exp_barcode = qry1.value(0).toString();
                    quantity = qry1.value(1).toString();
                    QSqlQuery bala2(db);
                    bala2.prepare("UPDATE item_balance SET item_balance = item_balance - :quan ,purchase_balance = purchase_balance - :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
                    bala2.bindValue(":ex_bcode",exp_barcode);
                    bala2.bindValue(":quan",quantity);
                    bala2.bindValue(":stock",storage_id);
                    bala2.bindValue(":branch",branch_id);
                    if(!bala2.exec())
                        qDebug()<<bala2.lastError();
                }
            }

            QSqlQuery qry2(db);
            qry2.prepare("Delete from purchases where rec_no = "+ui->rec_no->text()+" ");
            if (qry2.exec())
            { }
            QSqlQuery qry5(db);
            qry5.prepare("Delete from purchases_items where rec_no = "+ui->rec_no->text()+" ");
            if (qry5.exec())
            { }
            clearAll();

        }
        else if (ui->tableView_2->isVisible()) {

            QSqlQuery qry1(db);
            QSqlQuery qry3(db);
            QSqlQuery qry4(db);
            QString exp_barcode;
            QString quantity;
            QString storage_id;
            QString branch_id;
            QString supplier_id;
            double total;
            int payment_method;

            qry3.prepare("SELECT storage_id,branch_id,payment_method_id,total,supplier_id from purchases_return where rec_no = "+ui->rec_no->text()+" ");
            if (qry3.exec())
            {
                while (qry3.next())
                {
                    storage_id = qry3.value(0).toString();
                    branch_id = qry3.value(1).toString();
                    payment_method = qry3.value(2).toInt();
                    total = qry3.value(3).toDouble();
                    supplier_id = qry3.value(4).toString();
                }


            }
            if(payment_method == 1){
                qry4.prepare("UPDATE supplier SET balance = balance + :total WHERE id = :supp_id");
                qry4.bindValue(":total",total);
                qry4.bindValue(":supp_id",supplier_id);
                if (qry4.exec()){}else {
                    qDebug()<<"11"<<qry4.lastError();
                    }
            }

            qry1.prepare("SELECT exp_barcode,quantity from purchases_return_items where rec_no = "+ui->rec_no->text()+" ");
            if (qry1.exec())
            {
                while (qry1.next())
                {
                    exp_barcode = qry1.value(0).toString();
                    quantity = qry1.value(1).toString();
                    QSqlQuery bala2(db);
                    bala2.prepare("UPDATE item_balance SET item_balance = item_balance + :quan ,purchase_return_balance = purchase_return_balance - :quan WHERE exp_barcode = :ex_bcode AND storage_id = :stock AND branch_id = :branch");
                    bala2.bindValue(":ex_bcode",exp_barcode);
                    bala2.bindValue(":quan",quantity);
                    bala2.bindValue(":stock",storage_id);
                    bala2.bindValue(":branch",branch_id);
                    if(!bala2.exec())
                        qDebug()<<"12"<<bala2.lastError();
                }
            }

            QSqlQuery qry2(db);
            qry2.prepare("Delete from purchases_return where rec_no = "+ui->rec_no->text()+" ");
            if (qry2.exec())
            { }
            QSqlQuery qry5(db);
            qry5.prepare("Delete from purchases_return_items where rec_no = "+ui->rec_no->text()+" ");
            if (qry5.exec())
            { }
            clearAll();

        }
        else if (ui->tableWidget->isVisible()) {
            delete_row();
        }else if (ui->tableWidget_2->isVisible()) {
            delete_row();
        }

    }
}

void buy_win_cut::on_supp_name_currentIndexChanged(int index)
{
    QString supplier_id = model->index(index,1).data().toString();
        QSqlQuery qry(db);
        qry.prepare("SELECT balance FROM supplier WHERE id = :id");
        qry.bindValue(":id",supplier_id);
        if( qry.exec() ){
            while (qry.next())
            {
                ui->supp_bala->setText(qry.value(0).toString());

            }
        }
        else{qDebug() << "Error inserting into the table:\n" << qry.lastError();}
}

void buy_win_cut::on_pushButton_6_clicked()
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
           report->dataManager()->addModel("buy",ui->tableWidget_2->model(),false);
       }
       else if (!ui->tableWidget->isHidden()) {

           report->dataManager()->addModel("buy",ui->tableWidget->model(),false);
       }else if (ui->tableView->isVisible()) {
           report->dataManager()->addModel("buy",ui->tableView->model(),false);
       }
       else if (ui->tableView_2->isVisible()) {
                  report->dataManager()->addModel("buy",ui->tableView_2->model(),false);
              }
       if(ui->tableWidget->isVisible()||ui->tableWidget_2->isVisible()){
           report->loadFromFile(":/Rc/Xml/buy_rece.lrxml");
       }else {
           report->loadFromFile(":/Rc/Xml/buy_rece_2.lrxml");

       }

       report->previewReport();
   }else if (print_size.clickedButton() == small) {
    print_bcode();
   }
   else if (print_size.clickedButton() == close_but) {
       print_size.reject();
   }



}
void buy_win_cut::slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    QStringList columns;
    columns << "rec_no" << "supplier" << "total_before" << "discount"<<"total"<<"date";
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
        }else if (info.columnName == "supplier") {
            data = ui->supp_name->currentText();
        }else if (info.columnName == "total_before") {
           data = ui->total_rec->text();
        }else if (info.columnName == "discount") {
           data = ui->discount->text();
        }else if (info.columnName == "total") {
           data = ui->total_after->text();
        }else if (info.columnName == "date") {
            data = ui->dateTimeEdit->date();
         }
        break;
    default: break;
    }
}
void buy_win_cut::on_pushButton_7_clicked()
{
    /*report = new LimeReport::ReportEngine(this);
    report->dataManager()->addModel("bcode",ui->tableWidget->model(),false);
    report->designReport();*/
   /* QString printer_name;
    QSqlQuery qry(db2);
    qry.prepare("SELECT print_name from print_info WHERE print_type = 1");
    if (qry.exec())
    {while (qry.next())
        {
           printer_name = qry.value(0).toString();
        }
    }
    report = new LimeReport::ReportEngine(this);
    QPrinter *bcode_printer;
    bcode_printer = new QPrinter();
    bcode_printer->setPrinterName(printer_name);
    //QString file = ui->lineEdit->text();
    //report->loadFromFile(file);
    LimeReport::ICallbackDatasource *ds_1 =report->dataManager()->createCallbackDatasouce("bcode");
    connect(ds_1,SIGNAL(getCallbackData(const LimeReport::CallbackInfo& , QVariant& )),
            this,SLOT(slotGetPrintCallbackData(const LimeReport::CallbackInfo& , QVariant&)));
    int x = 0;
    int y = 0;
    int rowCount = ui->tableWidget->rowCount();
    int rowCount_g = ui->tableWidget_2->rowCount();
    QSqlQuery qry1(db);
    QSqlQuery qry2(db);
    if(!ui->tableWidget->isHidden()&&ui->tableWidget->rowCount()!=0){
         while (x<rowCount) {
                print_item_name = ui->tableWidget->item(x,1)->text();
                print_barcode = ui->tableWidget->item(x,3)->text();
                print_sell = ui->tableWidget->item(x,8)->text();
                bcode_printer->setCopyCount(ui->tableWidget->item(x,5)->text().toInt());
                report->loadFromFile(file);
                report->printReport(bcode_printer);
             x++;
            }
    }
    else if (!ui->tableWidget_2->isHidden()&&ui->tableWidget_2->rowCount()!=0) {
        while (x<rowCount_g) {
                print_item_name = ui->tableWidget_2->item(x,1)->text();
                print_barcode = ui->tableWidget_2->item(x,3)->text();
                print_sell = ui->tableWidget_2->item(x,8)->text();
                bcode_printer->setCopyCount(ui->tableWidget_2->item(x,5)->text().toInt());
                report->loadFromFile(file);
                report->printReport(bcode_printer);
          x++;
         }
    }
    else if(!ui->tableView->isHidden()){
        qry1.prepare("SELECT item_name,exp_barcode,quantity,sell_price from purchases_items WHERE rec_no = :id");
        qry1.bindValue(":id",ui->rec_no->text());
        if (qry1.exec())
        {
            while (qry1.next())
            {
                print_item_name = qry1.value(0).toString();
                print_barcode = qry1.value(1).toString();
                print_sell = qry1.value(3).toString();
                bcode_printer->setCopyCount(qry1.value(2).toInt());
                report->loadFromFile(file);
                report->printReport(bcode_printer);

            }
        }else{
        qDebug() << "Error inserting into the table:\n" << qry.lastError();}
    }
    else if(!ui->tableView_2->isHidden()){
        qry2.prepare("SELECT item_name,exp_barcode,quantity,sell_price from purchases_return_items WHERE rec_no = :id");
        qry2.bindValue(":id",ui->rec_no->text());
        if (qry2.exec())
        {
            while (qry2.next())
            {
                print_item_name = qry1.value(0).toString();
                print_barcode = qry1.value(1).toString();
                print_sell = qry1.value(3).toString();
                bcode_printer->setCopyCount(qry1.value(2).toInt());
                report->loadFromFile(file);
                report->printReport(bcode_printer);
            }
        }else{
        qDebug() << "Error inserting into the table:\n" << qry.lastError();}

    }*/


}


void buy_win_cut::on_pushButton_9_clicked()
{
    if(ui->tableWidget_2->isHidden()){
        set_edit();
    if(ui->rec_stat->currentIndex()==0){
        ui->tableView->hide();
        ui->tableView_2->hide();
        ui->tableWidget->hide();
        ui->tableWidget_2->show();
        ui->tableWidget_2->setRowCount(0);

    QSqlQuery qry(db);
    int id;
    QDateTime date;
    QString payment;
    QString test8;
    qry.prepare("SELECT MAX(rec_no) from purchases_g");
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
        ui->tableWidget->hide();
        ui->tableWidget_2->show();
        ui->tableWidget_2->setRowCount(0);
        ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"باركود"<<"اسم الصنف"<<"سعر الشراء"<<"كمية"<<"مجموع");
    QSqlQuery qry(db);
    int id;
    QDateTime date;
    QString payment;
    QString test8;


    qry.prepare("SELECT MAX(rec_no) from purchases_return_g");
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


void buy_win_cut::on_pushButton_8_clicked()
{
   /* QString printer_name;
    QSqlQuery qry(db2);
    qry.prepare("SELECT print_name from print_info WHERE print_type = 1");
    if (qry.exec())
    {while (qry.next())
        {
           printer_name = qry.value(0).toString();
        }
    }
    QtRPT *report = new QtRPT(this);
    report->loadReport(":/Rc/Xml/rpt (2).xml");*/

    //report->setSqlQuery("SELECT * FROM purchases_items ");
    //report->printExec();

    search->show();




}

void buy_win_cut::on_pushButton_10_clicked()
{
    qDebug()<<ui->item_name->currentIndex();
}


