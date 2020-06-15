#include "item_search.h"
#include "ui_item_search.h"

item_search::item_search(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::item_search)
{
    ui->setupUi(this);
	db = QSqlDatabase::database("test");
    if(!db.isOpen()){
    db.open();
    }
    model = new QSqlQueryModel(this);
    comp = new QCompleter(this);
    model->setQuery("SELECT name FROM items ",db);
    comp->setModel(model);
    ui->search_in->setCompleter(comp);
}

item_search::~item_search()
{
    delete ui;
}
void item_search::on_search_in_returnPressed()
{
	find_in = ui->search_in->text();
	QString id; //
	int supplier_id; //
	QString supplier_name;
    double stock = 0;//
    double buy = 0; //
    double sell = 0; //
	QString kid; //
	QString name; //
	QSqlQuery qry(db);
	QSqlQuery qry1(db);
	QSqlQuery qry2(db);
	QSqlQuery qry3(db);

 qry.prepare("SELECT id,supplier_id FROM items WHERE name = '"+find_in+"'");
 if( qry.exec()){
     while (qry.next())
     {

         id = qry.value(0).toString();
         supplier_id = qry.value(1).toInt();
		 name = ui->search_in->text();
     }
 }
 else{
     QMessageBox::warning(this,tr("error Updating"),qry.lastError().text(),QMessageBox::Ok);
       qDebug() << "Error inserting into the table:\n" << qry.lastError();
}
 if(qry.first()){
	QSqlQuery qry6(db);
	QSqlQuery qry5(db);
	QSqlQuery qry4(db);
    qry4.prepare("SELECT name from supplier WHERE id = "+QString::number(supplier_id)+"");
	if( qry4.exec()){
		while (qry4.next())
		{
		 supplier_name = qry4.value(0).toString();
		}
	}
	qry6.prepare("SELECT buy_price , sell_price , id from sub_items WHERE item_id = "+id+"");
	if( qry6.exec()){
		while (qry6.next())
		{
			buy = qry6.value(0).toDouble();
			sell = qry6.value(1).toDouble();
			kid = qry6.value(2).toString();
			
		}
	}
	qry5.prepare("SELECT item_balance from item_balance WHERE sub_item_id = "+kid+" ");
	if( qry5.exec()){
		while (qry5.next())
		{
			stock += qry5.value(0).toDouble();
            qDebug()<<stock;
		}
	}
    ui->item_name->setText(name);
    ui->supplier->setText(supplier_name);
    ui->buy_price->setText(QString::number(buy));
    ui->sell_price->setText(QString::number(sell));
    ui->quantity->setText(QString::number(stock));

 }
 else{
     qDebug()<<"all wrongaaaaa2";
     qry2.prepare("SELECT item_id , id , buy_price , sell_price  FROM sub_items WHERE int_barcode = '"+find_in+"'");
     if( qry2.exec() ){
         while (qry2.next())
         {
            id = qry2.value(0).toString();
			kid  = qry2.value(1).toString();
			buy = qry2.value(2).toDouble();
            sell = qry2.value(3).toDouble();
         }
     }
     else{
         QMessageBox::warning(this,tr("error Updating"),qry2.lastError().text(),QMessageBox::Ok);
           qDebug() << "Error inserting into the table:\n" << qry2.lastError();
    }
    if(qry2.first()){
		QSqlQuery qry6(db);
		QSqlQuery qry5(db);
		QSqlQuery qry4(db);
        qry5.prepare("SELECT item_balance from item_balance WHERE sub_item_id = "+kid+" ");
			if( qry5.exec()){
				while (qry5.next())
				{
					stock += qry5.value(0).toDouble();
				}
			}
		qry6.prepare("SELECT supplier_id , name from items WHERE id = "+id+"");
			if( qry6.exec()){
				while (qry6.next())
				{
                    supplier_id = qry6.value(0).toInt();
					name = qry6.value(1).toString();					
				}
			} 
		 
        qry4.prepare("SELECT name from supplier WHERE id = "+QString::number(supplier_id)+"");
			if( qry4.exec()){
				while (qry4.next())
				{
				 supplier_name = qry4.value(0).toString();
				}
			}
            ui->item_name->setText(name);
            ui->supplier->setText(supplier_name);
            ui->buy_price->setText(QString::number(buy));
            ui->sell_price->setText(QString::number(sell));
            ui->quantity->setText(QString::number(stock));
     }
     else {
        qDebug()<<"all wrongaaaaa";
        qry1.prepare("SELECT sub_item_id , item_balance FROM item_balance WHERE exp_barcode = '"+find_in+"'");
        if( qry1.exec() ){
            while (qry1.next())
            {
                kid = qry1.value(0).toString();
				stock = qry1.value(1).toDouble();
            }
        }
        if(qry1.first()){
			QSqlQuery qry6(db);
			QSqlQuery qry5(db);
			QSqlQuery qry4(db);
            qry5.prepare("SELECT item_id , buy_price , sell_price  FROM sub_items WHERE id = "+kid+" ");
			if( qry5.exec()){
				while (qry5.next())
				{
					id = qry5.value(0).toString();
					buy = qry5.value(1).toDouble();
					sell = qry5.value(2).toDouble();
					
				}
			}
			qry6.prepare("SELECT supplier_id , name from items WHERE id = "+id+"");
			if( qry6.exec()){
				while (qry6.next())
				{
                    supplier_id = qry6.value(0).toInt();
					name = qry6.value(1).toString();					
				}
			}
            qry4.prepare("SELECT name from supplier WHERE id = "+QString::number(supplier_id)+"");
			if( qry4.exec()){
				while (qry4.next())
				{
				 supplier_name = qry4.value(0).toString();
				}
			}

            ui->item_name->setText(name);
            ui->supplier->setText(supplier_name);
            ui->buy_price->setText(QString::number(buy));
            ui->sell_price->setText(QString::number(sell));
            ui->quantity->setText(QString::number(stock));
         }
		 else{		 
            ui->item_name->setText("Not Found");
            ui->supplier->setText("Not Found");
            ui->buy_price->setText(QString::number(0));
            ui->sell_price->setText(QString::number(0));
            ui->quantity->setText(QString::number(0));
		 }
     }
}
}
