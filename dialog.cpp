#include "dialog.h"
#include "ui_dialog.h"
#include "QString"
#include "QFileDialog"
#include "QDialog"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QDebug>
#include <QInputDialog>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->MostrarBD,SIGNAL(clicked()),this,SLOT(MostrarBD()));
    connect(ui->AregarBD,SIGNAL(clicked()),this,SLOT(GuardarBD()));
    connect(ui->comboBox_2,SIGNAL(currentIndexChanged(QString)),this,SLOT(Combobuscador()));
    connect(ui->Botonpictures,SIGNAL(clicked()),this,SLOT(Actualizar()));
    connect(ui->autindivBotom,SIGNAL(clicked()),this,SLOT(Mostrar_imagen_del_auto()));
    tabla1 = new QSqlTableModel(this);
    tabla2 = new QSqlTableModel(this);
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::MostrarBD(){

    dir= QFileDialog::getOpenFileName(this,"Seleccione donde esta la Base de Datos");
    dir.replace("/","//");
    db2 = QSqlDatabase::addDatabase("QODBC");
    db2.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ="+dir);
    db2.open();
    if(db2.isOpen())
    {
        qDebug()<<"aaaaa";
    }
    tabla3 = new QSqlTableModel(this, db2);
    tabla3->setTable("Tabla1");
    tabla3->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabla3->select();
    tabla3->sort(0,Qt::AscendingOrder);
    tabla3->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView1->setModel(tabla3);
    ui->tableView1->resizeColumnsToContents();
    qDebug()<<tabla3->tableName();
    db2.close();
}
void Dialog::GuardarBD(){

    dir= QFileDialog::getOpenFileName(this,"Seleccione donde esta la Base de Datos");
    dir.replace("/","//");
    QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena;

    Matricula=ui->lineEdit_4->text();
    Marca=ui->lineEdit->text();
    Modelo=ui->lineEdit_2->text();
    Color=ui->lineEdit_3->text();
    Precio=Precio.number(ui->lineEdit_5->text().toInt());
    Estado=ui->lineEdit_6->text();

    QSqlQuery busca;

    cadena=QString("INSERT INTO Tabla1 (Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
    busca.exec(cadena);
    qDebug()<<cadena;
    ui->tableView1->setModel(tabla1);
    ui->tableView1->resizeColumnsToContents();
    tabla1->submitAll();
    tabla1->setTable("Tabla1");
    tabla1->select();
    tabla1->sort(0,Qt::AscendingOrder);
    tabla1->submitAll();
       db2.close();

}
void Dialog::Combobuscador(){
    dir= QFileDialog::getOpenFileName(this,"Seleccione donde esta la Base de Datos");
    dir.replace("/","//");
    db2 = QSqlDatabase::addDatabase("QODBC");
    db2.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ="+dir);
    db2.open();
    QSqlQuery buscador;
    if(ui->comboBox_2->currentText()=="Marca"){

        ui->tableView1->setModel(tabla2);
        ui->tableView1->resizeColumnsToContents();
        QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Marca","Introduzca la marca de los autos que desea analizar");
        buscador.exec(QString("Select * From Tabla1"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              Matricula=buscador.value(1).toString();
              qDebug()<<Matricula;
              Marca=buscador.value(2).toString();
              Modelo=buscador.value(3).toString();
              Color=buscador.value(4).toString();
              Precio=Precio.number(buscador.value(5).toInt());
              Estado=buscador.value(6).toString();


                 if(buscador.value(2).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView1->setModel(tabla2);
                    ui->tableView1->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
   }
    else if(ui->comboBox_2->currentText()=="Modelo"){
        ui->tableView1->setModel(tabla2);
        ui->tableView1->resizeColumnsToContents();
        QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
        QSqlQuery buscador;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Modelo","Introduzca el modelo de los autos que desea analizar");
        buscador.exec(QString("Select * From Tabla1"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              Matricula=buscador.value(1).toString();
              qDebug()<<Matricula;
              Marca=buscador.value(2).toString();
              Modelo=buscador.value(3).toString();
              Color=buscador.value(4).toString();
              Precio=Precio.number(buscador.value(5).toInt());
              Estado=buscador.value(6).toString();


                 if(buscador.value(3).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView1->setModel(tabla2);
                    ui->tableView1->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
    }
    else if(ui->comboBox_2->currentText()=="Matricula"){
        ui->tableView1->setModel(tabla2);
        ui->tableView1->resizeColumnsToContents();
        QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
        QSqlQuery buscador;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Matricula","Introduzca la matriculla del auto que esta buscando");
        buscador.exec(QString("Select Matricula From Tabla1"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              Matricula=buscador.value(1).toString();
              qDebug()<<Matricula;
              Marca=buscador.value(2).toString();
              Modelo=buscador.value(3).toString();
              Color=buscador.value(4).toString();
              Precio=Precio.number(buscador.value(5).toInt());
              Estado=buscador.value(6).toString();


                 if(buscador.value(1).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                    gancho.exec(cadena);
                    ui->tableView1->setModel(tabla2);
                    ui->tableView1->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
    }
    else if(ui->comboBox_2->currentText()=="Color"){
        ui->tableView1->setModel(tabla2);
        ui->tableView1->resizeColumnsToContents();
        QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
        QSqlQuery buscador;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Color","Introduzca el color de los autos que desea analizar");
        buscador.exec(QString("Select * From Tabla1"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              Matricula=buscador.value(1).toString();
              qDebug()<<Matricula;
              Marca=buscador.value(2).toString();
              Modelo=buscador.value(3).toString();
              Color=buscador.value(4).toString();
              Precio=Precio.number(buscador.value(5).toInt());
              Estado=buscador.value(6).toString();


                 if(buscador.value(4).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView1->setModel(tabla2);
                    ui->tableView1->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
    }
    else if(ui->comboBox_2->currentText()=="Disponibilidad"){
        ui->tableView1->setModel(tabla2);
        ui->tableView1->resizeColumnsToContents();
        QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
        QSqlQuery buscador;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Disponibilidad","Califique la condicion de los autos que desea\n observar en Dis0ponible(escriba D) o no disponible(escriba ND)");
        buscador.exec(QString("Select * From Tabla1"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              Matricula=buscador.value(1).toString();
              qDebug()<<Matricula;
              Marca=buscador.value(2).toString();
              Modelo=buscador.value(3).toString();
              Color=buscador.value(4).toString();
              Precio=Precio.number(buscador.value(5).toInt());
              Estado=buscador.value(6).toString();


                 if(buscador.value(6).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView1->setModel(tabla2);
                    ui->tableView1->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
    }
    else if(ui->comboBox_2->currentText()=="Precio"){
        ui->tableView1->setModel(tabla2);
        ui->tableView1->resizeColumnsToContents();
        QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
        QSqlQuery buscador;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Precio","Introduzca el precio de los autos que busca");
        buscador.exec(QString("Select * From Tabla1"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              Matricula=buscador.value(1).toString();
              qDebug()<<Matricula;
              Marca=buscador.value(2).toString();
              Modelo=buscador.value(3).toString();
              Color=buscador.value(4).toString();
              Precio=Precio.number(buscador.value(5).toInt());
              Estado=buscador.value(6).toString();


                 if(buscador.value(5).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView1->setModel(tabla2);
                    ui->tableView1->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
    }
    db2.close();
}
void Dialog::Actualizar(){
    tabla3->submitAll();
    tabla1->submitAll();
    tabla2->submitAll();
}
void Dialog::Mostrar_imagen_del_auto(){

}
void Dialog::Salir(){
    close();
}
