#include "rege_cut.h"
#include "ui_rege_cut.h"

rege_cut::rege_cut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rege_cut)
{
    ui->setupUi(this);
    connect(ui->item_Color->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test1()));
    connect(ui->item_size->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test2()));
    connect(ui->cat_name->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test3()));
    connect(ui->supp_name->lineEdit(),SIGNAL(returnPressed()),this,SLOT(test7()));



    db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    if(db.open()){

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

    pri_up = new price_update(this);

    ui->supp_name->setModel(model);
    ui->cat_name->setModel(model3);


    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this, SLOT(find_items()));
    search1 = new search_items(this);
    connect(search1,SIGNAL(accepted()),this,SLOT(rec_item()));
    connect(pri_up,SIGNAL(accepted()),this,SLOT(update_price()));

    clear_all();

}

rege_cut::~rege_cut()
{
    delete ui;
}
void rege_cut::update_combo()
{
    db.close();
    db.open();
    int sup_ind = ui->supp_name->currentIndex();
    int cat_ind = ui->cat_name->currentIndex();
    model->setQuery("SELECT name , id FROM supplier",db);
    model3->setQuery("SELECT name , id FROM category",db);
    ui->supp_name->setCurrentIndex(sup_ind);
    ui->cat_name->setCurrentIndex(cat_ind);
}
void rege_cut::update_price(){
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



void rege_cut::get_sub_det(QString table){
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

            }

        }
    }
    else{
        QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
          qDebug() << "Error getting shit:\n" << qry.lastError();
   }

}
void rege_cut::clear_sub()
{
    ui->item_size->lineEdit()->clear();
    ui->item_Color->lineEdit()->clear();
    ui->item_buy->clear();
    ui->item_sell->clear();
    ui->item_profit->clear();
    ui->order_point->clear();

}
void rege_cut::clear_all()
{
    ui->item_name->clear();
    ui->supp_name->lineEdit()->clear();
    ui->cat_name->lineEdit()->clear();
    ui->item_size->lineEdit()->clear();
    ui->item_Color->lineEdit()->clear();
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
    model->setQuery("SELECT name , id FROM supplier",db);
    model3->setQuery("SELECT name , id FROM category",db);


    ui->supp_name->lineEdit()->setReadOnly(false);
    ui->cat_name->lineEdit()->setReadOnly(false);
    ui->item_name->setReadOnly(false);
    ui->item_Color->setCurrentText("--");
    ui->item_size->setCurrentText("--");


}
void rege_cut::re_con()
{
    db.close();
    db.open();
    model->setQuery("SELECT name, id FROM supplier",db);
    get_sub_det("size");
    get_sub_det("color");
    model3->setQuery("SELECT name, id FROM category",db);

}
void rege_cut::rec_item(){
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
     /*qDebug()<<"all wrongaaaaa2";
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
     }*/
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
void rege_cut::test1()
{
    if(ui->item_Color->lineEdit()->text().isEmpty()){


        ui->item_size->setFocus();

    }
    else{
        ui->item_size->setFocus();
    }


}
void rege_cut::test6()
{
   ui->item_buy->setFocus();

}


void rege_cut::test2()
{
    if(ui->item_size->lineEdit()->text().isEmpty()){

        ui->item_buy->setFocus();

    }

    else{        ui->item_buy->setFocus();

    }

}
void rege_cut::test3()
{
    if(ui->cat_name->lineEdit()->text().isEmpty()){

        ui->item_Color->lineEdit()->setFocus();

    }

    else{ui->item_Color->lineEdit()->setFocus();
    }

}
void rege_cut::test7(){
    if(ui->cat_name->lineEdit()->text().isEmpty()){

        ui->cat_name->lineEdit()->setFocus();


    }

    else{    ui->cat_name->lineEdit()->setFocus();

    }
}

void rege_cut::find_items(){
    search1->show();
}

void rege_cut::insert_items()
{

    QString id=ui->item_id->text();
    QString item_name = ui->item_name->text();
    int row = ui->supp_name->currentIndex();
    QModelIndex supp_index = model->index(row,1);
    QString item_supp_id = model->data(supp_index).toString();
    QString item_buy = ui->item_buy->text();
    QString item_profit = ui->item_profit->text();
    QString item_sell = ui->item_sell->text();

    QSqlQuery qry_9(db);
    QVector<int> vector;
    qry_9.prepare("SELECT substr(int_barcode,3) from sub_items WHERE int_barcode LIKE 'GN%'");
    if(qry_9.exec()){
        while (qry_9.next())
        {
            vector.append(qry_9.value(0).toInt());

        }
    }
    int max_id = *std::max_element(vector.begin(), vector.end());
    QString item_int = "GN"+QString::number(max_id+1);

    //QString item_int = ui->item_int->text();
    QString item_size = ui->item_size->lineEdit()->text();
    QString item_color = ui->item_Color->lineEdit()->text();
    QString order_point;
    int sub_id;
    if(!ui->order_point->text().isEmpty()){
        order_point = ui->order_point->text();
    }else {
        order_point = QString::number(0);
       }

    int row3 = ui->cat_name->currentIndex();
    QModelIndex category_index = model3->index(row3,1);
    in_cat = model3->data(category_index).toString();



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
     in.prepare("INSERT INTO items (`id`, `name`, `supplier_id`, `category_id`) VALUES (:id,:name,:supp,:cat)");
     in.bindValue(":id",id);
     in.bindValue(":name",item_name);
     in.bindValue(":supp",item_supp_id);
     in.bindValue(":cat",in_cat);


     in2.prepare("INSERT INTO sub_items (id,`item_id`,int_barcode, `buy_price`, `profit`, `sell_price`, `size`, `color`,`order_point`) VALUES (:sub_id,:id,:int,:buy,:profit,:sell,:size,:color,:order)");
     in2.bindValue(":id",id);
     in2.bindValue(":sub_id",sub_id);
     in2.bindValue(":buy",item_buy);
     in2.bindValue(":profit",item_profit);
     in2.bindValue(":sell",item_sell);
     in2.bindValue(":int",item_int);
     in2.bindValue(":size",item_size);
     in2.bindValue(":color",item_color);
     in2.bindValue(":order",order_point);

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

        in1.prepare("UPDATE items SET name = :name,supplier_id = :supp,category_id = :cat WHERE id = :id");
        in1.bindValue(":id",id);
        in1.bindValue(":name",item_name);
        in1.bindValue(":supp",item_supp_id);
        in1.bindValue(":cat",in_cat);
        if( !in1.exec() ){

            QMessageBox::warning(this,tr("error Updating item table"),in1.lastError().text(),QMessageBox::Ok);
            qDebug() << "Error inserting into the table22222:\n" << in1.lastError();}
        if(/*!ui->item_int->text().isEmpty()||*/!ui->item_buy->text().isEmpty()||!ui->item_sell->text().isEmpty()){

        in2.prepare("INSERT INTO sub_items (id,`item_id`,int_barcode, `buy_price`, `profit`, `sell_price`, `size`, `color`,`order_point`) VALUES (:sub_id,:id,:int,:buy,:profit,:sell,:size,:color,:order)");
        in2.bindValue(":id",id);
        in2.bindValue(":sub_id",sub_id);
        in2.bindValue(":buy",item_buy);
        in2.bindValue(":profit",item_profit);
        in2.bindValue(":sell",item_sell);
        in2.bindValue(":int",item_int);
        in2.bindValue(":size",item_size);
        in2.bindValue(":color",item_color);
        in2.bindValue(":order",order_point);
        if( !in2.exec() ){

            QMessageBox::warning(this,tr("error inserting sub_items"),in2.lastError().text(),QMessageBox::Ok);
            qDebug() << "Error inserting into the table22222:\n" << in2.lastError();}
        }

    }

    db.close();
    db.open();
    emit addeditems();
}
void rege_cut::update_item(){
    QSqlQuery in1(db);

    int row3 = ui->cat_name->currentIndex();
    QModelIndex category_index = model3->index(row3,1);
    in_cat = model3->data(category_index).toString();

    QString id=ui->item_id->text();
    QString item_name = ui->item_name->text();
    int row = ui->supp_name->currentIndex();

    QModelIndex supp_index = model->index(row,1);

    QString item_supp_id = model->data(supp_index).toString();
        qDebug()<<row<<supp_index<<item_supp_id;
    in1.prepare("UPDATE items SET name = :name,supplier_id = :supp,category_id = :cat WHERE id = :id");
    in1.bindValue(":id",id);
    in1.bindValue(":name",item_name);
    in1.bindValue(":supp",item_supp_id);
    in1.bindValue(":cat",in_cat);
    if( !in1.exec() ){

        QMessageBox::warning(this,tr("error Updating item table"),in1.lastError().text(),QMessageBox::Ok);
        qDebug() << "Error inserting into the table22222:\n" << in1.lastError();}
    emit addeditems();


}

void rege_cut::load_table()
{
    QString class_id;
    QString cat_id;
    QString sea_id;
    QString supplier_id;
    QString id = ui->item_id->text();
    this->model8 = new QSqlQueryModel();
    model8->setQuery("SELECT `id`, `size`, `color`, `buy_price`, `profit`, `sell_price` , `order_point` FROM sub_items WHERE item_id = '"+id+"' ",db);
    ui->tableView->setModel(model8);
    QSqlQuery qry(db);
    QSqlQuery qry1(db);
    qry.prepare("SELECT category_id,supplier_id FROM items WHERE id = :id");
    qry.bindValue(":id",id);
    if( qry.exec() ){
        while (qry.next())
        {
            cat_id = qry.value(0).toString();
            supplier_id =  qry.value(1).toString();
        }
    }
    else{
        QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
        qDebug() << "Error inserting into the table:\n" << qry.lastError();}

    model->setQuery("SELECT name ,id FROM supplier WHERE id = "+supplier_id+"",db);
    model3->setQuery("SELECT name ,id FROM category WHERE id = "+cat_id+"",db);
    ui->supp_name->setCurrentIndex(0);
    ui->cat_name->setCurrentIndex(0);

}
void rege_cut::on_item_id_textChanged(const QString &arg1)
{
    QString id = ui->item_id->text();
    QString class_id;
    QString cat_id;
    QString sea_id;
    QString supplier_id;
    ui->item_name->clear();
    ui->supp_name->lineEdit()->clear();
    ui->cat_name->lineEdit()->clear();
    ui->item_size->lineEdit()->clear();
    ui->item_Color->lineEdit()->clear();
   // ui->item_int->clear();
    ui->item_buy->clear();
    ui->item_sell->clear();
    ui->item_profit->clear();
    ui->order_point->clear();
    this->model8 = new QSqlQueryModel();
    model8->setQuery("SELECT `id`, `size`, `color`, `buy_price`, `profit`, `sell_price` , `order_point` FROM sub_items WHERE item_id = '"+id+"' ",db);
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
    qry.prepare("SELECT name,category_id,supplier_id FROM items WHERE id = :id");
    qry.bindValue(":id",id);
    if( qry.exec() ){
        while (qry.next())
        {
            ui->item_name->setText(qry.value(0).toString());
            cat_id = qry.value(1).toString();
            supplier_id =  qry.value(2).toString();
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

   }

}

/*void rege_cut::on_item_int_returnPressed()
{

}*/

void rege_cut::on_item_buy_returnPressed()
{
    ui->item_profit->setFocus();
}

void rege_cut::on_item_profit_returnPressed()
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

void rege_cut::on_item_name_returnPressed()
{
    ui->supp_name->lineEdit()->setFocus();
}

void rege_cut::on_item_profit_editingFinished()
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

void rege_cut::on_item_sell_editingFinished()
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
void rege_cut::on_item_buy_editingFinished()
{
    float buy = ui->item_buy->text().toFloat();
    float prof_num = ui->item_profit->text().toFloat();
    float prof_inc = buy * prof_num/100;
    float sell = buy + prof_inc;
    QString sell_price = QString::number(sell);
    ui->item_sell->setText(sell_price);
}

void rege_cut::on_item_sell_returnPressed()
{
  ui->order_point->setFocus();
}

void rege_cut::on_add_clicked()
{
  insert_items();
  clear_sub();
  load_table();


}

void rege_cut::on_next_clicked()
{
    clear_all();
}



void rege_cut::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    in_code = model8->index(row,0).data().toString();
    qDebug()<<model8->index(row,0).data().toString();
}

void rege_cut::on_pushButton_clicked()
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

void rege_cut::on_pushButton_2_clicked()
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

void rege_cut::on_pushButton_3_clicked()
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

void rege_cut::on_pushButton_4_clicked()
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

void rege_cut::on_pushButton_5_clicked()
{
    pri_up->show();

}

/*void rege_cut::on_checkBox_stateChanged(int arg1)
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
}*/

void rege_cut::on_pushButton_6_clicked()
{
    update_item();
}

