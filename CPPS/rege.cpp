#include "rege.h"
#include "ui_rege.h"

rege::rege(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::rege)
{
    ui->setupUi(this);
    connect(ui->item_Color->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test1()));
    connect(ui->item_size->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test2()));
    connect(ui->cat_name->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test3()));
    connect(ui->sea_name->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test4()));
    connect(ui->class_name->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test5()));
    connect(ui->item_descrip->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test6()));
    connect(ui->supp_name->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test7()));



    db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    if(db.open()){

        ui->statusbar->showMessage("DataBase Connected!",2000);


    }
    else {
        ui->statusbar->showMessage("Error Error",2000);
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

    pri_up = new price_update(this);

    ui->supp_name->setModel(model);
    ui->cat_name->setModel(model3);
    ui->class_name->setModel(model4);
    ui->sea_name->setModel(model5);

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this, SLOT(find_items()));
    search1 = new search_items(this);
    connect(search1,SIGNAL(accepted()),this,SLOT(rec_item()));
    connect(pri_up,SIGNAL(accepted()),this,SLOT(update_price()));

    clear_all();




}
rege::~rege()
{
    delete ui;
}
void rege::update_combo()
{
    db.close();
    db.open();
    int sup_ind = ui->supp_name->currentIndex();
    int cat_ind = ui->cat_name->currentIndex();
    int sea_ind = ui->sea_name->currentIndex();
    int clas_ind = ui->class_name->currentIndex();
    model->setQuery("SELECT name , id FROM supplier",db);
    model3->setQuery("SELECT name , id FROM category",db);
    model4->setQuery("SELECT name , id FROM class",db);
    model5->setQuery("SELECT name, id FROM season",db);
    ui->supp_name->setCurrentIndex(sup_ind);
    ui->cat_name->setCurrentIndex(cat_ind);
    ui->sea_name->setCurrentIndex(sea_ind);
    ui->class_name->setCurrentIndex(clas_ind);
}
void rege::update_price(){
   int state = pri_up->update_state;
   double buy = pri_up->buy;
   double sell = pri_up->sell;
   double profit_prec= pri_up->profit_perc;
   QSqlQuery qry(db);
   if(buy>0&&sell>0&&profit_prec>0){
   if(state == 0){
       qry.prepare("UPDATE sub_items SET buy_price = :buy ,sell_price =:sell,profit = :profit WHERE id = "+in_code+"");
       qry.bindValue(":buy",buy);
       qry.bindValue(":sell",sell);
       qry.bindValue(":profit",profit_prec);
       if( qry.exec()){
       }
       else{
           QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
             qDebug() << "Error inserting into the ta111ble:\n" << qry.lastError();
      }

   }else if (state == 1) {
       qry.prepare("UPDATE sub_items SET buy_price = :buy ,sell_price =:sell,profit = :profit WHERE item_id = "+ui->item_id->text()+"");
       qry.bindValue(":buy",buy);
       qry.bindValue(":sell",sell);
       qry.bindValue(":profit",profit_prec);
       if( qry.exec()){
       }
       else{
           QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
             qDebug() << "Error inserting into the table:\n" << qry.lastError();
      }
   }
   }
   load_table();
   emit addeditems();



}



void rege::get_sub_det(QString table){
    QSqlQuery qry(db);
    QStringList inlist;
    qry.prepare("SELECT DISTINCT "+table+" FROM sub_items");
    if (qry.exec())
    {
        while (qry.next())
        {
            if(table == "size"){
                ui->item_size->addItem(qry.value(0).toString());
            }else if (table == "color") {
                ui->item_Color->addItem(qry.value(0).toString());

            }else if (table == "description") {
                ui->item_descrip->addItem(qry.value(0).toString());
            }

        }
    }
    else{
        QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
          qDebug() << "Error getting shit:\n" << qry.lastError();
   }

}
void rege::clear_sub()
{
    ui->item_size->lineEdit()->clear();
    ui->item_Color->lineEdit()->clear();
    ui->item_int->clear();
    ui->item_buy->clear();
    ui->item_sell->clear();
    ui->item_profit->clear();
    ui->order_point->clear();
    ui->checkBox->setChecked(false);

}
void rege::clear_all()
{
    ui->item_name->clear();
    ui->supp_name->lineEdit()->clear();
    ui->cat_name->lineEdit()->clear();
    ui->sea_name->lineEdit()->clear();
    ui->class_name->lineEdit()->clear();
    ui->item_size->lineEdit()->clear();
    ui->item_Color->lineEdit()->clear();
    ui->item_int->clear();
    ui->item_buy->clear();
    ui->item_sell->clear();
    ui->item_profit->clear();
    ui->order_point->clear();
    model8->clear();
    db.close();
    db.open();

    QSqlQuery qry(db);
    qry.prepare("SELECT MAX(id) from items");
    if (qry.exec())
    {
        while (qry.next())
        {
            int test7;
            test7 = qry.value(0).toInt();
            test7+=1;
            QString test6 = QString::number(test7);
            ui->item_id->setText(test6);

        }
    }

    get_sub_det("size");
    get_sub_det("color");
    get_sub_det("description");
    model->setQuery("SELECT name , id FROM supplier",db);
    model3->setQuery("SELECT name , id FROM category",db);
    model4->setQuery("SELECT name , id FROM class",db);
    model5->setQuery("SELECT name, id FROM season",db);

    ui->supp_name->lineEdit()->setReadOnly(false);
    ui->class_name->lineEdit()->setReadOnly(false);
    ui->cat_name->lineEdit()->setReadOnly(false);
    ui->sea_name->lineEdit()->setReadOnly(false);
    ui->item_name->setReadOnly(false);
    ui->checkBox->setChecked(false);
    ui->item_Color->setCurrentText("--");
    ui->item_size->setCurrentText("--");


}
void rege::re_con()
{
    db.close();
    db.open();
    model->setQuery("SELECT name, id FROM supplier",db);
    get_sub_det("size");
    get_sub_det("color");
    get_sub_det("description");
    model3->setQuery("SELECT name, id FROM category",db);
    model4->setQuery("SELECT name, id FROM class",db);
    model5->setQuery("SELECT name, id FROM season",db);
}
void rege::rec_item(){
 QString in = search1->find_in;
 qDebug()<<in;
 QString id;
 QString kid;
 QSqlQuery qry(db);
 QSqlQuery qry1(db);
 QSqlQuery qry2(db);
 QSqlQuery qry3(db);

 qry.prepare("SELECT id FROM items WHERE name = '"+in+"'");
 if( qry.exec()){
     while (qry.next())
     {

         id = qry.value(0).toString();
     }
 }
 else{
     QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
       qDebug() << "Error inserting into the table:\n" << qry.lastError();
}
 if(qry.first()){

     ui->item_id->setText(id);
 }
 else{
     qDebug()<<"all wrongaaaaa2";
     qry2.prepare("SELECT item_id FROM sub_items WHERE int_barcode = '"+in+"'");
     if( qry2.exec() ){
         while (qry2.next())
         {
             id = qry2.value(0).toString();
         }
     }
     if(qry2.first()){
         ui->item_id->setText(id);
     }
     else {
         qDebug()<<"all wrongaaaaa";
         qry1.prepare("SELECT item_id FROM sub_items WHERE id = '"+in+"'");
         if( qry1.exec() ){
             while (qry1.next())
             {
                 id = qry1.value(0).toString();
             }
         }
         if(qry1.first()){
             ui->item_id->setText(id);
         }
     }

     /*else{
         qry2.prepare("SELECT item_id FROM item_bala WHERE ex_bcode = :in");
         qry2.bindValue("in",in);
         if( qry2.exec() ){
             while (qry2.next())
             {

                 kid = qry2.value(0).toString();
             }
         }
         if(qry2.first()){
             qry3.prepare("SELECT true_id FROM items WHERE kid = :in");
             qry3.bindValue("in",kid);
             if( qry3.exec() ){
                 while (qry3.next())
                 {
                     id = qry3.value(0).toString();
                 }
             }
             if(qry3.first()){
                 ui->item_id->setText(id);
             }
             else{
                 qDebug()<<"all wrong";
             }

         }
     }*/
 }




}
void rege::test1()
{
    if(ui->item_Color->lineEdit()->text().isEmpty()){
       /* yes = new Dialog(this);
        yes->show();
        hide();*/



    }
    else{
        ui->item_size->setFocus();
    }


}
void rege::test6()
{
   ui->item_buy->setFocus();

}

void rege::test7()
{
   ui->sea_name->lineEdit()->setFocus();
}
void rege::test2()
{
    if(ui->item_size->lineEdit()->text().isEmpty()){
      /*  yes1 = new Dialog1(this);
        yes1->show();
        hide();*/
        ui->item_descrip->lineEdit()->setFocus();

    }

    else{ui->item_descrip->lineEdit()->setFocus();
    }

}
void rege::test3()
{
    if(ui->cat_name->lineEdit()->text().isEmpty()){
        /*yes2 = new Dialog2(this);
        yes2->show();
        hide();*/
        ui->item_int->setFocus();

    }

    else{ui->item_int->setFocus();
    }

}
void rege::test4()
{
    if(ui->sea_name->lineEdit()->text().isEmpty()){
       /* yes3 = new Dialog3(this);
        yes3->show();
        hide();*/
        ui->class_name->lineEdit()->setFocus();
    }

    else{ui->class_name->lineEdit()->setFocus();
    }

}
void rege::test5()
{
    if(ui->class_name->lineEdit()->text().isEmpty()){
        /*yes4 = new Dialog4(this);
        yes4->show();
        hide();*/
        ui->cat_name->lineEdit()->setFocus();
    }

    else{ui->cat_name->lineEdit()->setFocus();
    }

}
void rege::find_items(){
    search1->show();
}

void rege::insert_items()
{

    QString id=ui->item_id->text();
    QString item_name = ui->item_name->text();
    int row = ui->supp_name->currentIndex();
    QModelIndex supp_index = model->index(row,1);
    QString item_supp_id = model->data(supp_index).toString();
    QString item_buy = ui->item_buy->text();
    QString item_profit = ui->item_profit->text();
    QString item_sell = ui->item_sell->text();
    QString item_int = ui->item_int->text();
    QString item_size = ui->item_size->lineEdit()->text();
    QString item_color = ui->item_Color->lineEdit()->text();
    QString order_point;
    QString item_descrip = ui->item_descrip->lineEdit()->text();
    int sub_id;
    if(!ui->order_point->text().isEmpty()){
        order_point = ui->order_point->text();
    }else {
        order_point = QString::number(0);
       }

    int row2 = ui->class_name->currentIndex();
    QModelIndex class_index = model4->index(row2,1);
    in_class = model4->data(class_index).toString();
    int row3 = ui->cat_name->currentIndex();
    QModelIndex category_index = model3->index(row3,1);
    in_cat = model3->data(category_index).toString();
    int row4 = ui->sea_name->currentIndex();
    QModelIndex season_index = model5->index(row4,1);
    in_sea = model5->data(season_index).toString();


    int recCount = 0;
    QSqlQuery qry(db);
    qry.prepare("SELECT * FROM items WHERE id = :id");
    qry.bindValue(":id",id);
    if( qry.exec() ){
        while (qry.next())
        {
            recCount++;
        }
    }

    else{
        QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
          qDebug() << "Error inserting into the table:\n" << qry.lastError();
   }
    QSqlQuery qry1(db);
    qry1.prepare("SELECT MAX(id) FROM sub_items");
    if( qry1.exec() ){
        while (qry1.next())
        {
            sub_id = qry1.value(0).toInt()+1;

        }
    }
    if(recCount==0){
        qDebug() << "null";
     QSqlQuery in(db);
     QSqlQuery in2(db);
     in.prepare("INSERT INTO items (`id`, `name`, `supplier_id`, `class_id`, `category_id`, `season_id`) VALUES (:id,:name,:supp,:class,:cat,:sea)");
     in.bindValue(":id",id);
     in.bindValue(":name",item_name);
     in.bindValue(":supp",item_supp_id);
     in.bindValue(":class",in_class);
     in.bindValue(":cat",in_cat);
     in.bindValue(":sea",in_sea);

     in2.prepare("INSERT INTO sub_items (id,`item_id`, `buy_price`, `profit`, `sell_price`, `int_barcode`, `size`, `color`,`description`,`order_point`) VALUES (:sub_id,:id,:buy,:profit,:sell,:int,:size,:color,:descrip,:order)");
     in2.bindValue(":id",id);
     in2.bindValue(":sub_id",sub_id);
     in2.bindValue(":buy",item_buy);
     in2.bindValue(":profit",item_profit);
     in2.bindValue(":sell",item_sell);
     in2.bindValue(":int",item_int);
     in2.bindValue(":size",item_size);
     in2.bindValue(":color",item_color);
     in2.bindValue(":order",order_point);
     in2.bindValue(":descrip",item_descrip);

     if( !in.exec() ){
         QMessageBox::warning(this,tr("error Updating"),in.lastError().text(),QMessageBox::Ok);
         qDebug() << "Error inserting into the table:\n" << in.lastError()  << in.executedQuery();
     }
     if( !in2.exec() ){

         QMessageBox::warning(this,tr("error Updating"),in2.lastError().text(),QMessageBox::Ok);
             qDebug() << "Error inserting into the table888:\n" << in2.lastError()  << in2.executedQuery();}
    }
    else{

		QSqlQuery in1(db);
        QSqlQuery in2(db);
		 
        in1.prepare("UPDATE items SET name = :name,supplier_id = :supp,class_id = :class,category_id = :cat,season_id = :sea WHERE id = :id");
		in1.bindValue(":id",id);
		in1.bindValue(":name",item_name);
		in1.bindValue(":supp",item_supp_id);
		in1.bindValue(":class",in_class);
		in1.bindValue(":cat",in_cat);
		in1.bindValue(":sea",in_sea);
        if( !in1.exec() ){

            QMessageBox::warning(this,tr("error Updating item table"),in1.lastError().text(),QMessageBox::Ok);
            qDebug() << "Error inserting into the table22222:\n" << in1.lastError();}
        if(!ui->item_int->text().isEmpty()||!ui->item_buy->text().isEmpty()||!ui->item_sell->text().isEmpty()){
		
        in2.prepare("INSERT INTO sub_items (id,`item_id`, `buy_price`, `profit`, `sell_price`, `int_barcode`, `size`, `color`,`description`,`order_point`) VALUES (:sub_id,:id,:buy,:profit,:sell,:int,:size,:color,:descrip,:order)");
        in2.bindValue(":id",id);
        in2.bindValue(":sub_id",sub_id);
        in2.bindValue(":buy",item_buy);
        in2.bindValue(":profit",item_profit);
        in2.bindValue(":sell",item_sell);
        in2.bindValue(":int",item_int);
        in2.bindValue(":size",item_size);
        in2.bindValue(":color",item_color);
        in2.bindValue(":order",order_point);
        in2.bindValue(":descrip",item_descrip);
        if( !in2.exec() ){

            QMessageBox::warning(this,tr("error inserting sub_items"),in2.lastError().text(),QMessageBox::Ok);
            qDebug() << "Error inserting into the table22222:\n" << in2.lastError();}
        }

    }

    db.close();
    db.open();
    emit addeditems();
}
void rege::update_item(){
    QSqlQuery in1(db);
    int row2 = ui->class_name->currentIndex();
    QModelIndex class_index = model4->index(row2,1);
    in_class = model4->data(class_index).toString();
    int row3 = ui->cat_name->currentIndex();
    QModelIndex category_index = model3->index(row3,1);
    in_cat = model3->data(category_index).toString();
    int row4 = ui->sea_name->currentIndex();
    QModelIndex season_index = model5->index(row4,1);
    in_sea = model5->data(season_index).toString();
    QString id=ui->item_id->text();
    QString item_name = ui->item_name->text();
    int row = ui->supp_name->currentIndex();

    QModelIndex supp_index = model->index(row,1);

    QString item_supp_id = model->data(supp_index).toString();
        qDebug()<<row<<supp_index<<item_supp_id;
    in1.prepare("UPDATE items SET name = :name,supplier_id = :supp,class_id = :class,category_id = :cat,season_id = :sea WHERE id = :id");
    in1.bindValue(":id",id);
    in1.bindValue(":name",item_name);
    in1.bindValue(":supp",item_supp_id);
    in1.bindValue(":class",in_class);
    in1.bindValue(":cat",in_cat);
    in1.bindValue(":sea",in_sea);
    if( !in1.exec() ){

        QMessageBox::warning(this,tr("error Updating item table"),in1.lastError().text(),QMessageBox::Ok);
        qDebug() << "Error inserting into the table22222:\n" << in1.lastError();}
    emit addeditems();


}

void rege::load_table()
{
    QString class_id;
    QString cat_id;
    QString sea_id;
    QString supplier_id;
    QString id = ui->item_id->text();
    this->model8 = new QSqlQueryModel();
    model8->setQuery("SELECT `id`, `size`, `color`,`description`, `int_barcode`, `buy_price`, `profit`, `sell_price` , `order_point` FROM sub_items WHERE item_id = '"+id+"' ",db);
    ui->tableView->setModel(model8);
    QSqlQuery qry(db);
    QSqlQuery qry1(db);
    qry.prepare("SELECT class_id,category_id,season_id,supplier_id FROM items WHERE id = :id");
    qry.bindValue(":id",id);
    if( qry.exec() ){
        while (qry.next())
        {
            class_id = qry.value(0).toString();
            cat_id = qry.value(1).toString();
            sea_id =  qry.value(2).toString();
            supplier_id =  qry.value(3).toString();
        }
    }
    else{
        QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
        qDebug() << "Error inserting into the table:\n" << qry.lastError();}

    model->setQuery("SELECT name ,id FROM supplier WHERE id = "+supplier_id+"",db);
    model3->setQuery("SELECT name ,id FROM category WHERE id = "+cat_id+"",db);
    model4->setQuery("SELECT name ,id FROM class WHERE id = "+class_id+"",db);
    model5->setQuery("SELECT name  ,id FROM season WHERE id = "+sea_id+"",db);
    ui->supp_name->setCurrentIndex(0);
    ui->cat_name->setCurrentIndex(0);
    ui->class_name->setCurrentIndex(0);
    ui->sea_name->setCurrentIndex(0);



}
void rege::on_item_id_textChanged(const QString &arg1)
{
    QString id = ui->item_id->text();
    QString class_id;
    QString cat_id;
    QString sea_id;
    QString supplier_id;
    ui->item_name->clear();
    ui->supp_name->lineEdit()->clear();
    ui->cat_name->lineEdit()->clear();
    ui->sea_name->lineEdit()->clear();
    ui->class_name->lineEdit()->clear();
    ui->item_size->lineEdit()->clear();
    ui->item_Color->lineEdit()->clear();
    ui->item_int->clear();
    ui->item_buy->clear();
    ui->item_sell->clear();
    ui->item_profit->clear();
    ui->order_point->clear();
    this->model8 = new QSqlQueryModel();
    model8->setQuery("SELECT `id`, `size`, `color`,`description`, `int_barcode`, `buy_price`, `profit`, `sell_price` , `order_point` FROM sub_items WHERE item_id = '"+id+"' ",db);
    ui->tableView->setModel(model8);
    int recCount = 0;
    QSqlQuery qry(db);
    qry.prepare("SELECT * FROM items WHERE id = :id");
    qry.bindValue(":id",id);
    if( qry.exec() ){
        while (qry.next())
        {
            recCount++;
        }
    }

    else{
        QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
          qDebug() << "Error inserting into the table:\n" << qry.lastError();
   }
    if(recCount!=0){

    QSqlQuery qry(db);
    QSqlQuery qry1(db);
    qry.prepare("SELECT name,class_id,category_id,season_id,supplier_id FROM items WHERE id = :id");
    qry.bindValue(":id",id);
    if( qry.exec() ){
        while (qry.next())
        {
            ui->item_name->setText(qry.value(0).toString());
            class_id = qry.value(1).toString();
            cat_id = qry.value(2).toString();
            sea_id =  qry.value(3).toString();
            supplier_id =  qry.value(4).toString();
        }
    }
    else{
        QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
        qDebug() << "Error inserting into the table:\n" << qry.lastError();}

    QList<QModelIndex> modelIndexList;
    modelIndexList.clear();
    modelIndexList = model->match(model->index(0, 1), Qt::DisplayRole, QVariant::fromValue(supplier_id), -1, Qt::MatchExactly);
    if(!modelIndexList.isEmpty()){
        ui->supp_name->setCurrentIndex(modelIndexList.first().row());
    }
    modelIndexList.clear();
    modelIndexList = model3->match(model3->index(0, 1), Qt::DisplayRole, QVariant::fromValue(cat_id), -1, Qt::MatchExactly);
    if(!modelIndexList.isEmpty()){
        ui->cat_name->setCurrentIndex(modelIndexList.first().row());
    }
    modelIndexList.clear();
    modelIndexList = model4->match(model4->index(0, 1), Qt::DisplayRole, QVariant::fromValue(class_id), -1, Qt::MatchExactly);
    if(!modelIndexList.isEmpty()){
        ui->class_name->setCurrentIndex(modelIndexList.first().row());
    }
    modelIndexList.clear();
    modelIndexList = model5->match(model5->index(0, 1), Qt::DisplayRole, QVariant::fromValue(sea_id), -1, Qt::MatchExactly);
    if(!modelIndexList.isEmpty()){
        ui->sea_name->setCurrentIndex(modelIndexList.first().row());
    }

    }

}

void rege::on_item_int_returnPressed()
{
   ui->item_Color->setFocus();

}

void rege::on_item_buy_returnPressed()
{
    ui->item_profit->setFocus();
}

void rege::on_item_profit_returnPressed()
{
    ui->item_sell->setFocus();
  float buy = ui->item_buy->text().toFloat();
  float prof_num = ui->item_profit->text().toFloat();
  if(!ui->item_profit->text().isEmpty()){
      float prof_inc = buy * prof_num/100;
      float sell = buy + prof_inc;
      QString sell_price = QString::number(sell);
      ui->item_sell->setText(sell_price);
  }

}

void rege::on_item_name_returnPressed()
{
    ui->supp_name->lineEdit()->setFocus();
}

void rege::on_item_profit_editingFinished()
{
    float buy = ui->item_buy->text().toFloat();
    float prof_num = ui->item_profit->text().toFloat();
    if(!ui->item_profit->text().isEmpty()){
    float prof_inc = buy * prof_num/100;
    float sell = buy + prof_inc;
    QString sell_price = QString::number(sell);
    ui->item_sell->setText(sell_price);
    }
}

void rege::on_item_sell_editingFinished()
{
    float buy = ui->item_buy->text().toFloat();
    float sell = ui->item_sell->text().toFloat();
    if(ui->item_buy->text().isEmpty()||ui->item_buy->text()=="0"){
    }else{
        float profit = sell-buy;
        float profit_prec = (profit/buy)*100;
        QString prof = QString::number(profit_prec);
        ui->item_profit->setText(prof);
    }
}
void rege::on_item_buy_editingFinished()
{
    float buy = ui->item_buy->text().toFloat();
    float prof_num = ui->item_profit->text().toFloat();
    float prof_inc = buy * prof_num/100;
    float sell = buy + prof_inc;
    QString sell_price = QString::number(sell);
    ui->item_sell->setText(sell_price);
}

void rege::on_item_sell_returnPressed()
{
  ui->order_point->setFocus();
}

void rege::on_add_clicked()
{
  insert_items();
  clear_sub();
  load_table();


}

void rege::on_next_clicked()
{
    clear_all();
}



void rege::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    in_code = model8->index(row,0).data().toString();
    qDebug()<<model8->index(row,0).data().toString();
}

void rege::on_pushButton_clicked()
{
    int min_id;
    QSqlQuery qry(db);
    qry.prepare("SELECT MIN(id) from items");
    if (qry.exec())
    {
        while (qry.next())
        {
            min_id = qry.value(0).toInt();

        }
    }
    int id =  ui->item_id->text().toInt();
    if(id>min_id){
   ui->item_id->setText(QString::number(id-1));}
}

void rege::on_pushButton_2_clicked()
{
    int max_id;
    QSqlQuery qry(db);
    qry.prepare("SELECT MAX(id) from items");
    if (qry.exec())
    {
        while (qry.next())
        {

            max_id = qry.value(0).toInt();


        }
    }

    int id =  ui->item_id->text().toInt();
    if(id<max_id){
    ui->item_id->setText(QString::number(id+1));}
}

void rege::on_pushButton_3_clicked()
{
    if (QMessageBox::question(this, tr("delete?!!!"),
    tr("Are You Sure You WANT TO DELEETE!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        if (QMessageBox::question(this, tr("delete?!!!"),
        tr("Are You Sure You WANT TO DELEETE!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
    QString id = ui->item_id->text();
    QSqlQuery qry(db);
    QSqlQuery qry2(db);
    qry.prepare("DELETE FROM items WHERE id = :id");
    qry.bindValue(":id",id);
    if( qry.exec() ){}
    else{
        QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
        qDebug() << "Error inserting into the table:\n" << qry.lastError();}
    qry2.prepare("DELETE FROM sub_items WHERE item_id = :id");
    qry2.bindValue(":id",id);
    if( qry2.exec() ){}
    else{

       QMessageBox::warning(this,tr("error Updating"),qry2.lastError().text(),QMessageBox::Ok);
        qDebug() << "Error inserting into the table:\n" << qry2.lastError();}
    clear_all();}
        emit addeditems();


    }
}

void rege::on_pushButton_4_clicked()
{
    if(!in_code.isEmpty()){
    if (QMessageBox::question(this, tr("delete?!!!"),
    tr("Are You Sure You WANT TO DELEETE!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        if (QMessageBox::question(this, tr("delete?!!!"),
        tr("Are You Sure You WANT TO DELEETE!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
    QSqlQuery qry2(db);
    qry2.prepare("DELETE FROM sub_items WHERE id = :id");
    qry2.bindValue(":id",in_code);
    if( qry2.exec() ){}
    else{
        QMessageBox::warning(this,tr("error Updating"),qry2.lastError().text(),QMessageBox::Ok);
        qDebug() << "Error inserting into the table:\n" << qry2.lastError();}
    load_table();
    in_code.clear();
        }
        emit addeditems();

    }
    }
}

void rege::on_pushButton_5_clicked()
{
    pri_up->show();

}

void rege::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked()){
        ui->item_int->setReadOnly(true);
        QSqlQuery qry(db);
        QVector<int> vector;
        qry.prepare("SELECT substr(int_barcode,3) from sub_items WHERE int_barcode LIKE 'GN%'");
        if(qry.exec()){
            while (qry.next())
            {
                vector.append(qry.value(0).toInt());

            }
        }
        int max_id = *std::max_element(vector.begin(), vector.end());

        ui->item_int->setText("GN"+QString::number(max_id+1));

    }else {
    ui->item_int->setReadOnly(false);
    ui->item_int->clear();
    }
}

void rege::on_pushButton_6_clicked()
{
    update_item();
}

