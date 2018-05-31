#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QSqlDatabase>
//#include <QSqlQuery>
#include <QDebug>
#include <QKeyEvent>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kahaostr="";
    jifenNum=0;
    model = new QStandardItemModel();
    model->setColumnCount(3);
    model->setHeaderData(0,Qt::Horizontal,tr("时间"));
    model->setHeaderData(1,Qt::Horizontal,tr("变更类型"));
    model->setHeaderData(2,Qt::Horizontal,tr("积分"));
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setColumnWidth(0,200);
    ui->tableView->setColumnWidth(1,200);
    ui->tableView->setColumnWidth(2,200);
    ui->tableView->horizontalHeader()->setMinimumHeight(25);
    //ui->tableView->horizontalHeader()->setStyleSheet("border: 1px solid black;");
    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    //ui->tableView->horizontalHeader()->hide();
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("testdb");
    db.setUserName("root");
    db.setPassword("123456");
//    QString strmsg=QObject::tr("打开");
//    QLabel *label=new QLabel(tr("中文"));
//    ui->statusBar->showMessage(tr("中文"),2000);
    //ui->statusBar->addWidget(label);
    //ui->textEdit_2->setReadOnly(true);
//    QStringList drivelist=QSqlDatabase::drivers();
//    qDebug()<<drivelist;
//    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("localhost");
//    db.setDatabaseName("testdb");
//    db.setUserName("root");
//    db.setPassword("123456");
//    bool ok=db.open();
//    qDebug()<<ok;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->statusBar->showMessage(tr("正在增加会员，请稍等。"));
    if(ui->lineEdit->text().trimmed()==""||ui->lineEdit_2->text().trimmed()==""||ui->lineEdit_4->text().trimmed()=="")
    {
        ui->statusBar->showMessage(tr("请完善资料！"),3000);
        return;
    }
    QString sqlstr;
    sqlstr="insert into user (kahao,name,phone,jifen,time) values('"
            +ui->lineEdit->text()+"','"
            +ui->lineEdit_2->text()+"','"
            +ui->lineEdit_4->text()+"',"
            +"0,"
            +"NOW()"
            +");";
    QSqlQuery query;
    if(!db.open())
    {
        qDebug()<<"1";//db.lastError()
        return;
    }
    if(!query.exec("set names utf8;"))
    {
        qDebug()<<"2";//query.lastError()
        db.close();
        ui->statusBar->showMessage(tr("设置字符失败"));
        return;
    }
    if(!query.exec("select * from user where kahao like '"+ui->lineEdit->text()+"'"))
    {
        db.close();
        ui->statusBar->showMessage(tr("查询失败！"));
        return;
    }
    if(query.size()>0)
    {
        ui->statusBar->showMessage(tr("该卡号已存在！"));
        qDebug()<<query.size();
        return;
    }
    if(!query.exec(sqlstr))
    {
        qDebug()<<"3";//<<query.lastError()
        db.close();
        ui->statusBar->showMessage(ui->lineEdit_2->text()+tr("添加失败"));
        return;
    }
    db.close();
    kahaostr=ui->lineEdit->text();
    jifenNum=0;
    ui->statusBar->showMessage(ui->lineEdit_2->text()+tr("添加成功"));
//    QLabel *status=new QLabel();
//    status->setText(ui->lineEdit_2->text()+tr("添加成功"));
}
void MainWindow::keyPressEvent(QKeyEvent *k)
{
    if(k->key()==Qt::Key_F1)
    {
        ui->lineEdit->setFocus();
    }
    if(k->key()==Qt::Key_F2)
    {
        ui->lineEdit_5->setFocus();
    }
    if(k->key()==Qt::Key_F3)
    {
        ui->lineEdit_2->setFocus();
    }
    if(k->key()==Qt::Key_F4)
    {
        ui->lineEdit_6->setFocus();
    }
    if(k->key()==Qt::Key_F5)
    {
        ui->lineEdit_3->setFocus();
    }
    if(k->key()==Qt::Key_F6)
    {
        ui->lineEdit_4->setFocus();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->lineEdit->text().trimmed()=="")
    {
        ui->statusBar->showMessage(tr("请输入卡号"));
        return;
    }
    if(!db.open())
    {
        ui->statusBar->showMessage(tr("数据库打开失败！"));
        return;
    }
    QSqlQuery query;
    if(!query.exec("set names utf8;"))
    {
        ui->statusBar->showMessage(tr("设置字符失败"));
        return;
    }
    if(!query.exec("select * from user where kahao like '"+ui->lineEdit->text()+"'"))
    {
        db.close();
        ui->statusBar->showMessage(tr("查询失败"));
        return;
    }
    if(query.size()==0)
    {
        ui->statusBar->showMessage(tr("无当前会员！"));
        db.close();
        return;
    }

    query.first();
    kahaostr=ui->lineEdit->text();
    ui->lineEdit_2->setText(query.value(2).toString());
    jifenNum=query.value(4).toFloat();
    ui->lineEdit_3->setText(QString::number(jifenNum));
    ui->lineEdit_4->setText(query.value(3).toString());

    if(!query.exec("select * from record where kahao like '"+kahaostr+"' order by id DESC"))//record
    {
        db.close();
        ui->statusBar->showMessage(tr("查询失败"));
        return;
    }
    model->removeRows(0,model->rowCount());          /////////////
    if(query.size()!=0)
    {
        query.first();
        int i=0,j=0;
        do
        {
            QStandardItem *itemTime=new QStandardItem(query.value(2).toString());
            model->setItem(j,0,itemTime);
            QStandardItem *itemState=new QStandardItem(query.value(3).toString());
            model->setItem(j,1,itemState);
            QStandardItem *itemNum=new QStandardItem(query.value(4).toString());
            model->setItem(j,2,itemNum);
            j++;
            i++;
        }
        while(query.next()&&i<10);

    }
    ui->tableView->setModel(model);
    ui->statusBar->showMessage(tr("查询成功！"));
    db.close();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->lineEdit_5->text().trimmed()=="")
    {
        ui->statusBar->showMessage(tr("请输入数量！"));
        return;
    }
    float numtmp=0;
    try
    {
        numtmp=ui->lineEdit_5->text().toFloat();
    }
    catch(float)
    {
        ui->statusBar->showMessage(tr("积分输入错误"));
        return;
    }
    if(ui->lineEdit->text().trimmed()!=kahaostr)
    {
        ui->statusBar->showMessage(tr("请先查询当前会员积分！"));
        return;
    }
    if(!db.open())
    {
        ui->statusBar->showMessage(tr("数据库打开失败！"));
        return;
    }
    QSqlQuery query;
    if(!query.exec("set names utf8;"))
    {
        db.close();
        ui->statusBar->showMessage(tr("设置字符失败"));
        return;
    }
    jifenNum+=ui->lineEdit_5->text().toFloat();
    if(!query.exec("update user set jifen="+QString::number(jifenNum)+",time=now() where kahao like '"+kahaostr+"'"))
    {
        db.close();
        ui->statusBar->showMessage(tr("更新积分失败！"));
        jifenNum-=ui->lineEdit_5->text().toFloat();
        return;
    }
    ui->lineEdit_3->setText(QString::number(jifenNum));
    ui->statusBar->showMessage(tr("积分已更新为")+QString::number(jifenNum));
    QString sqlstr;
    sqlstr="insert into record (kahao,time,state,number) values('"
            +kahaostr+"',now(),'增加',"
            +ui->lineEdit_5->text()+");";
    if(!query.exec(sqlstr))
    {
        db.close();
        ui->statusBar->showMessage(tr("插入积分记录失败"));
        return;
    }
    db.close();
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->lineEdit_6->text().trimmed()=="")
    {
        ui->statusBar->showMessage(tr("请输入数量！"));
        return;
    }
    float numtmp=0;
    try
    {
        numtmp=ui->lineEdit_6->text().toFloat();
    }
    catch(float)
    {
        ui->statusBar->showMessage(tr("积分输入错误"));
        return;
    }
    if(ui->lineEdit->text().trimmed()!=kahaostr)
    {
        ui->statusBar->showMessage(tr("请先查询当前会员积分！"));
        return;
    }
    if(!db.open())
    {
        ui->statusBar->showMessage(tr("数据库打开失败！"));
        return;
    }
    QSqlQuery query;
    if(!query.exec("set names utf8;"))
    {
        db.close();
        ui->statusBar->showMessage(tr("设置字符失败"));
        return;
    }
    jifenNum-=numtmp;
    if(!query.exec("update user set jifen="+QString::number(jifenNum)+",time=now() where kahao like '"+kahaostr+"'"))
    {
        db.close();
        ui->statusBar->showMessage(tr("更新积分失败！"));
        jifenNum+=ui->lineEdit_6->text().toFloat();
        return;
    }
    ui->lineEdit_3->setText(QString::number(jifenNum));

    ui->statusBar->showMessage(tr("积分已更新为")+QString::number(jifenNum));
    QString sqlstr;
    sqlstr="insert into record (kahao,time,state,number) values('"
            +kahaostr+"',now(),'消费',"
            +ui->lineEdit_6->text()+");";
    if(!query.exec(sqlstr))
    {
        db.close();
        ui->statusBar->showMessage(tr("插入积分记录失败"));
        return;
    }
    db.close();
}
