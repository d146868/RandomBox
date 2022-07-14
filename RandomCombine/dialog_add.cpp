#include "dialog_add.h"
#include "ui_dialog_add.h"
#include "mainwindow.h"
#include <qmessagebox.h>

Dialog_Add::Dialog_Add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Add)
{
    ui->setupUi(this);
}

Dialog_Add::~Dialog_Add()
{
    delete ui;
}

void Dialog_Add::on_pushButton_Ok_clicked()
{
   MainWindow* pMain = (MainWindow*)parentWidget();
   QString svalue = ui->lineEdit->text();
   if (svalue.isEmpty())
   {
       QMessageBox::warning(this,"warning","值为空",QMessageBox::Yes,QMessageBox::Yes);
       return;
   }
   pMain->AddData(svalue);
   ui->lineEdit->clear();
}


void Dialog_Add::on_pushButton_Cancel_clicked()
{
    close();
}

