#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_add.h"

#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QStringList>
#include <QSqlRecord>
#include <QVariant>
#include <QtDebug>
#include <ctime>
#include <algorithm>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->radioButton_1->setChecked(true);
    db = QSqlDatabase::addDatabase("QODBC");
    QString dbname ="DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=" + QCoreApplication::applicationDirPath() + "//Database.accdb";
    db.setDatabaseName(dbname);
    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),db.lastError().text());
    }

    ReadLibData();

    this->setWindowTitle("Random Box");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ADD_clicked()
{
    Dialog_Add* dlg = new Dialog_Add(this);
    dlg->setModal(true);
    dlg->show();
}


void MainWindow::on_DEL_clicked()
{
   QString svalue = ui->lib_list->currentItem()->text();
   if (!svalue.isEmpty())
   {
       for (auto i = 0 ;i < m_vecLibData.size(); ++i)
       {
           if (m_vecLibData.at(i) == svalue){
               m_vecLibData.remove(i);
               break;
           }
       }

       QSqlQuery query;
       query=QSqlQuery(db);
       query.prepare("delete from Vocabulary where name=?");
       query.addBindValue(svalue);
       query.exec();

       UpdateLibDataList();
   }

}


void MainWindow::on_Start_btn_clicked()
{
    if (m_combineSize <= 0 )
    {
        QMessageBox::warning(this,"Warning","选择组合个数");
        return;
    }
    srand(time(0));
    vecIndex.clear();
    int rn = 0;
    for (auto i =0; i < m_combineSize; ++i)
    {
        rn = rand() % (m_vecLibData.size()-1);
        while (count(vecIndex.begin(),vecIndex.end(),rn))
        {
            rn = rand() % (m_vecLibData.size());
        }
        vecIndex.push_back(rn);
    }

    ui->result_list->clear();

    foreach(int i,vecIndex)
    {
        ui->result_list->addItem(m_vecLibData[i]);
    }
}


void MainWindow::on_radioButton_1_clicked()
{
    m_combineSize = 2;
}


void MainWindow::on_radioButton_2_clicked()
{
    m_combineSize = 3;
}

void MainWindow::on_radioButton_3_clicked()
{
    m_combineSize = 4;
}


void MainWindow::on_radioButton_4_clicked()
{
    m_combineSize = 5;
}

void MainWindow::AddData(const QString &sValue)
{
    if (m_vecLibData.contains(sValue))
    {
        QMessageBox::warning(this,"重复警告","数据重复");
        return;
    }

    m_vecLibData.push_back(sValue);


    QSqlQuery query;
    query=QSqlQuery(db);
    query.prepare("insert into Vocabulary(name)" "VALUES(?)");
    query.addBindValue(sValue);
    query.exec();
    query.clear();

    UpdateLibDataList();
}

void MainWindow::UpdateLibDataList()
{
    ui->lib_list->clear();
    foreach(QString s,m_vecLibData)
    {
        if(!m_curSearchKey.isEmpty())
        {
            if (s.contains(m_curSearchKey))
            {
                ui->lib_list->addItem(s);
            }
        }
        else
            ui->lib_list->addItem(s);
    }

    ui->lib_list->update();
}

void MainWindow::ReadLibData()
{
    m_vecLibData.clear();
    QStringList tables;
    QString tabName,sqlString;
    tables = db.tables(QSql::Tables);
    for (int i =0; i < tables.size() ; ++i) {
        tabName = tables.at(i);
        sqlString = "select * from " + tabName;
        QSqlQuery q(sqlString);
        QSqlRecord rec = q.record();
        int fieldCount = rec.count();

        QString fieldName;
        for(auto j=0;j<fieldCount;j++)
            fieldName = rec.fieldName(j);

        while(q.next()){
            for(int i=0;i<fieldCount;i++){
             m_vecLibData.push_back(q.value(i).toString());
            }

        }
    }

    UpdateLibDataList();
}

void MainWindow::UpdateGroupList()
{
    ui->list_Group->clear();
    foreach(QString s,m_vecGroup)
    {
        ui->list_Group->addItem(s);
    }
    ui->list_Group->update();
}

void MainWindow::on_textEdit_Search_textChanged()
{    
   m_curSearchKey = ui->textEdit_Search->toPlainText();
   UpdateLibDataList();
}


void MainWindow::on_pushButton_add_grouplist_clicked()
{
    if (!vecIndex.empty())
    {
        QString temp;
        foreach(int i,vecIndex)
        {
            temp += m_vecLibData[i] + " + ";
        }
        temp.remove(temp.length()-3,temp.length());
        m_vecGroup.push_back(temp);
        UpdateGroupList();
    }

}


void MainWindow::on_pushButton_g_clear_clicked()
{
    m_vecGroup.clear();
    UpdateGroupList();
}

