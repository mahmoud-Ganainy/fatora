#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QDate lock(2010,06,17);
    if(QDate::currentDate() > lock){
        ui->setupUi(this);
        QString loc = QCoreApplication::applicationDirPath()+"/setting2.db";
        QFile setting_f(":/Rc/DB/setting2.db");
        QFile setti(loc);
        if(setti.exists()){
        db1 = QSqlDatabase::addDatabase("QSQLITE","setting");
        db1.setDatabaseName(loc);
        if( db1.open()){

        }
        else{
           ui->statusBar->showMessage("Error Error",2000000);
        }
        }else {
            QFile::copy(":/Rc/DB/setting2.db",loc);
            QFile::setPermissions(loc,QFileDevice::WriteOther);
            db1 = QSqlDatabase::addDatabase("QSQLITE","setting");
            db1.setDatabaseName(loc);
            if( db1.open()){

            }
            else{
               ui->statusBar->showMessage("Error Error",2000000);
            }

    }

        QSqlQuery qry(db1);
        qry.prepare("SELECT * FROM db_default");
        if( qry.exec() ){
            while (qry.next())
            {
                type = qry.value(0).toString();
            }
        }
        else{
              qDebug() << "Error hhhhhh into the table:\n" << qry.lastError();
       }
        if (type == "File Based"){
            QSqlQuery qry1(db1);
            qry1.prepare("SELECT * FROM file_data");
            if( qry1.exec() ){
                while (qry1.next())
                {
                    loc_data = qry1.value(1).toString();
                }
            }
            else{
                  qDebug() << "Error inserting into the table:\n" << qry1.lastError();
           }
        db = QSqlDatabase::addDatabase("QSQLITE","test");
        db.setDatabaseName(loc_data);
        }
        else {

            QSqlQuery qry1(db1);
            qry1.prepare("SELECT * FROM server_data");
            if( qry1.exec() ){
                while (qry1.next())
                {
                    server_host = qry1.value(0).toString();
                    server_user = qry1.value(1).toString();
                    server_pass = qry1.value(2).toString();
                }
            }
            else{
                  qDebug() << "Error inserting into the table:\n" << qry1.lastError();
           }
            QSqlQuery qry2(db1);
            qry2.prepare("SELECT * FROM version");
            if( qry2.exec() ){
                while (qry2.next())
                {
                    version = qry2.value(0).toInt();

                }
            }
            else{
                  qDebug() << "Error inserting into the table:\n" << qry1.lastError();
           }




         db = QSqlDatabase::addDatabase("QMYSQL","test");
         db.setHostName(server_host);
         db.setUserName(server_user);
         db.setPassword(server_pass);
         if(version == 0){
         db.setDatabaseName("fatora_db");
         }else if (version == 1) {
             db.setDatabaseName("fatora_db_2");
         }
        }

        //this->setStyleSheet(QString::fromUtf8("font: 14pt FreeMono;"));
         sell_wind = new sell_win;
         buy_wind = new buy_Win;
         categ = new cat_rege;
         class_regi = new class_rege;
         reges = new rege;
         sett = new setting_window;
         sea_regi = new sea_rege;
         supp = new supp_rege;
         custom = new custom_rege;
         payment = new payments;
         cash_rege = new cash_depo_rege;
         storage_reg = new storage_rege;
         expen = new expenses;
         moni = new monitor;
         repo = new reportmanager;
         supp_rep = new supplier_report;
         custom_rep = new customer_report;
         reges_cut = new rege_cut;
         buy_wind_cut = new buy_win_cut;
         sell_wind_cut = new sell_win_cut;
         emp_rege = new employee_rege;
         emp_man = new employee_man;
         data_shit = new data_sheet;

         ui->tabWidget->hide();
         connect(reges,SIGNAL(addeditems()),buy_wind,SLOT(update_itemsList()));
         connect(reges_cut,SIGNAL(addeditems()),buy_wind_cut,SLOT(update_itemsList()));
         connect(reges,SIGNAL(addeditems()),sell_wind,SLOT(update_itemsList()));
         connect(reges_cut,SIGNAL(addeditems()),sell_wind_cut,SLOT(update_itemsList()));
         connect(supp,SIGNAL(addeditems()),buy_wind,SLOT(update_List()));
         connect(supp,SIGNAL(addeditems()),buy_wind_cut,SLOT(update_List()));
         connect(custom,SIGNAL(addeditems()),sell_wind,SLOT(update_List()));
         connect(custom,SIGNAL(addeditems()),sell_wind_cut,SLOT(update_List()));
         connect(supp,SIGNAL(addeditems()),payment,SLOT(update_List()));
         connect(custom,SIGNAL(addeditems()),payment,SLOT(update_List()));
         connect(categ,SIGNAL(addeditems()),reges,SLOT(update_combo()));
         connect(sea_regi,SIGNAL(addeditems()),reges,SLOT(update_combo()));
         connect(class_regi,SIGNAL(addeditems()),reges,SLOT(update_combo()));
         connect(supp,SIGNAL(addeditems()),reges,SLOT(update_combo()));

         connect(categ,SIGNAL(addeditems()),reges_cut,SLOT(update_combo()));
         connect(sea_regi,SIGNAL(addeditems()),reges_cut,SLOT(update_combo()));
         connect(class_regi,SIGNAL(addeditems()),reges_cut,SLOT(update_combo()));
         connect(supp,SIGNAL(addeditems()),reges_cut,SLOT(update_combo()));

       // ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/Rc/Xml/123.qml")));

    }




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e){
    if (QMessageBox::question(this, tr("Close?!!!"),
    tr("Are You Sure You WANT TO QUIT!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        if (QMessageBox::question(this, tr("backup?!!!"),
        tr("Do u want to backup database!!!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
			if(version == 0){
			QProcess dumpProcess(this);
			QStringList args;
			args <<"-uroot"<<"-p0102588220" <<"-hlocalhost"<<"fatora_db";
			dumpProcess.setStandardOutputFile("dump-"+QDate::currentDate().toString()+".sql");
			dumpProcess.start("mysqldump", args);
            dumpProcess.waitForFinished();
            qDebug()<< dumpProcess.exitStatus();

            }
			else if (version == 1) {
			    QProcess dumpProcess(this);
				QStringList args;
				args <<"-uroot"<<"-p0102588220" <<"-hlocalhost"<<"fatora_db_2";
				dumpProcess.setStandardOutputFile("dump-"+QDate::currentDate().toString()+".sql");
				dumpProcess.start("mysqldump", args);
				dumpProcess.waitForFinished();
			}
			
			
        
        }
    qApp->closeAllWindows();
    e->accept();         
    }
    else{
        e->ignore();
    }
}
void MainWindow::on_actionCustomer_triggered()
{    
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(custom,"customers");
        ui->tabWidget->setCurrentWidget(custom);
}

void MainWindow::on_actionSupplier_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(supp,"suppliers");
        ui->tabWidget->setCurrentWidget(supp);

}


void MainWindow::on_actionSeason_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(sea_regi,"season");
        ui->tabWidget->setCurrentWidget(sea_regi);

}

void MainWindow::on_actionSales_Return_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
    if(version == 0){
        ui->tabWidget->addTab(sell_wind,"sales");
        ui->tabWidget->setCurrentWidget(sell_wind);
    }else if (version == 1) {
        ui->tabWidget->addTab(sell_wind_cut,"sales");
        ui->tabWidget->setCurrentWidget(sell_wind_cut);
    }
}

void MainWindow::on_actionBuy_Return_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
    if(version == 0){
        ui->tabWidget->addTab(buy_wind,"buy");
        ui->tabWidget->setCurrentWidget(buy_wind);
    }else if (version == 1) {
        ui->tabWidget->addTab(buy_wind_cut,"buy");
        ui->tabWidget->setCurrentWidget(buy_wind_cut);
    }
}

void MainWindow::on_actionCategory_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(categ,"category");
        ui->tabWidget->setCurrentWidget(categ);
}

void MainWindow::on_actionClass_triggered()
{

    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(class_regi,"class");
        ui->tabWidget->setCurrentWidget(class_regi);
}

void MainWindow::on_actionItems_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
    if(version == 0){
        ui->tabWidget->addTab(reges,"item register");
        ui->tabWidget->setCurrentWidget(reges);
    }
    else if (version == 1) {
        ui->tabWidget->addTab(reges_cut,"item register");
        ui->tabWidget->setCurrentWidget(reges_cut);
    }
}

void MainWindow::on_actionsetting_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(sett,"setting");
        ui->tabWidget->setCurrentWidget(sett);
}

void MainWindow::on_action_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(payment,"payments");
        ui->tabWidget->setCurrentWidget(payment);
}

void MainWindow::on_action_2_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(cash_rege,"cash deposite register");
        ui->tabWidget->setCurrentWidget(cash_rege);
}

void MainWindow::on_action_3_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(storage_reg,"storage register");
        ui->tabWidget->setCurrentWidget(storage_reg);
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
   ui->tabWidget->removeTab( index);
   if(ui->tabWidget->count()==0){

    ui->tabWidget->hide();

   }
}

void MainWindow::on_action_4_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(expen,"مصروفات");
        ui->tabWidget->setCurrentWidget(expen);
}

void MainWindow::on_action_5_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(moni,"تقرير يومي");
        ui->tabWidget->setCurrentWidget(moni);
        moni->getstat(0);
        moni->getTable();
}

void MainWindow::on_actiontest_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(repo,"تقارير ");
        ui->tabWidget->setCurrentWidget(repo);
}

void MainWindow::on_action_6_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(supp_rep,"تقرير مورد ");
        ui->tabWidget->setCurrentWidget(supp_rep);
}

void MainWindow::on_action_7_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(emp_rege,"تكويد الموظفين ");
        ui->tabWidget->setCurrentWidget(emp_rege);
}

void MainWindow::on_action_8_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(emp_man,"ادارة الموظفين ");
        ui->tabWidget->setCurrentWidget(emp_man);
}

void MainWindow::on_action_9_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(data_shit,"قائيمة الالصناف ");
        ui->tabWidget->setCurrentWidget(data_shit);
}

void MainWindow::on_action_10_triggered()
{
    if(ui->tabWidget->isHidden()){
    ui->tabWidget->show();
    }
    ui->tabWidget->currentWidget();
        ui->tabWidget->addTab(custom_rep,"تقرير عملاء ");
        ui->tabWidget->setCurrentWidget(custom_rep);
}
