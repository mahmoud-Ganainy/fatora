#include "setting_window.h"
#include "ui_setting_window.h"

setting_window::setting_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::setting_window)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("setting");
    if(!db.isOpen()){
    if(db.open()){
           qDebug()<<"open";
        ui->statusbar->showMessage("DataBase Connected!",2000000);
    }
    else {
                   qDebug()<<"close";
        ui->statusbar->showMessage("Error Error",2000000);
    }
    }
    this->info_print = new QPrinterInfo();
    QStringList printer = info_print->availablePrinterNames();
    ui->printerlist->addItems(printer);
    ui->loc->hide();
    ui->lineEdit->hide();
    ui->toolButton->hide();
    ui->lineEdit->clear();
    loc_data.clear();

}
setting_window::~setting_window()
{
    delete ui;
}

void setting_window::on_pushButton_clicked()
{
    
   QString printer_name = ui->printerlist->currentText();
   int counter = 0;
   if(ui->barcode->isChecked()){
       type = 1;
       qDebug()<<type;
   }
   else if(ui->receipt->isChecked()){
       type = 2;
       qDebug()<<type;
   }
   else if(ui->report->isChecked()){
       type = 3;
       qDebug()<<type;
   }
   QSqlQuery qry(db);
   qry.prepare("SELECT * FROM print_info WHERE print_type = :type");
   qry.bindValue(":type",type);
   if( qry.exec() ){
       while (qry.next())
       {
           qDebug() <<qry.value(0).toString()<<qry.value(2).toString();
           counter++;
       }
   }

   else{
         qDebug() << "Error inserting into the table66666:\n" << qry.lastError();
  }
   if (counter==0){
      QSqlQuery qry1(db);
      qry1.prepare("INTSERT INTO print_info (print_name,print_type) VALUES (:print_name,:print_type)");
      qry1.bindValue(":print_name",printer_name);
      qry1.bindValue(":print_type",type);
      if( qry1.exec() ){ }
      else{
            qDebug() << "Error inserting into the table55555:\n" << qry.lastError();
     }
   }
   else{
       QSqlQuery qry2(db);
       qry2.prepare("UPDATE print_info SET print_name = :print_name ,print_type = :print_type WHERE print_type = :print_type");
       qry2.bindValue(":print_name",printer_name);
       qry2.bindValue(":print_type",type);
       if( qry2.exec() ){ }
       else{
             qDebug() << "Error inserting into the table36633666:\n" << qry2.lastError();
      }
   }



  /* QSqlQuery qry4(db);
   qry4.prepare("UPDATE db_default SET type = '"+ui->comboBox->currentText()+"' ");
   if( qry4.exec() ){ }
   else{
         qDebug() << "Error inserting into the table:\n" << qry4.lastError();
  }

   if(ui->comboBox->currentText() == "Server"){


   }
   else{
       if(!loc_data.isNull()){
       QSqlQuery qry3(db);
       qry3.prepare("UPDATE file_data SET  db_loc = :loc");
       qry3.bindValue(":loc",loc_data);
       if( qry3.exec() ){ }
       else{
             qDebug() << "Error inserting into the table:\n" << qry3.lastError();
      }
     }
   }*/






}

void setting_window::on_toolButton_clicked()
{
    if (ui->comboBox->currentText() == "File Based"){
        QString s1 =
            QFileDialog::getOpenFileName(this, "Open a file", "directoryToOpen",
                "DB files (*.db *.sqlite)");
        ui->lineEdit->setText(s1);
        loc_data =s1;
    }
}

void setting_window::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0){
      ui->loc->hide();
      ui->lineEdit->hide();
      ui->toolButton->hide();
      ui->lineEdit->clear();
      loc_data.clear();
    }
    else{
        ui->loc->show();
        ui->lineEdit->show();
        ui->toolButton->show();
    }
}

void setting_window::on_pushButton_2_clicked()
{
    if(ui->cut->isChecked()){
        QSqlQuery qry(db);
        qry.prepare("UPDATE version SET version = 1");
        if( qry.exec() ){

        }
        else{
              qDebug() << "Error inserting into the table66666:\n" << qry.lastError();
       }

    }else if (ui->uncut->isChecked()) {
        QSqlQuery qry(db);
        qry.prepare("UPDATE version SET version = 0");
        if( qry.exec() ){

        }
        else{
              qDebug() << "Error inserting into the table66666:\n" << qry.lastError();
       }
    }



}

void setting_window::on_pushButton_3_clicked()
{
    if(!ui->place_name->text().isEmpty()){

        QSqlQuery qry(db);
        qry.prepare("UPDATE shop_info SET shop_name = :shop_name ");
        qry.bindValue(":shop_name",ui->place_name->text());


        if( qry.exec() ){

        }
        else{
              qDebug() << "Error inserting into the table66666:\n" << qry.lastError();
       }
    }
    if(!ui->tax_id->text().isEmpty()){
        QSqlQuery qry(db);
        qry.prepare("UPDATE shop_info SET  tax_id = :tax");
        qry.bindValue(":tax",ui->tax_id->text());
        if( qry.exec() ){

        }
        else{
              qDebug() << "Error inserting into the table66666:\n" << qry.lastError();
       }
    }
    if(!ui->trade_id->text().isEmpty()){
        QSqlQuery qry(db);
        qry.prepare("UPDATE shop_info SET trade_id = :trade");
        qry.bindValue(":trade",ui->trade_id->text());
        if( qry.exec() ){

        }
        else{
              qDebug() << "Error inserting into the table66666:\n" << qry.lastError();
       }

    }
    if(!ui->phone_num->text().isEmpty()){
        QSqlQuery qry(db);
        qry.prepare("UPDATE shop_info SET phone= :phone");
        qry.bindValue(":phone",ui->phone_num->text());

        if( qry.exec() ){

        }
        else{
              qDebug() << "Error inserting into the table66666:\n" << qry.lastError();
       }




    }

}
