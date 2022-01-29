#include "cliente.h"
#include "ui_cliente.h"
#include "interfaz.h"
#include "QString"
#include "QDialog"
#include "QFileDialog"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QWidget>
#include <math.h>
#include <QTime>
#include <QDebug>

cliente::cliente(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cliente)
{
    ///ojoo hay que probar esta clase antes de comprimirla en el ejecutable lo mismo va para las demas

    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Mostrar_lista_de_autos()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(Enviar_solicitud_de_ingreso()));
    connect(ui->dateTimeEdit,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(Desabilitar()));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(Desabilitar1()));
    connect(ui->spinBox_2,SIGNAL(valueChanged(int)),this,SLOT(Desabilitar1()));
    connect(ui->spinBox_3,SIGNAL(valueChanged(int)),this,SLOT(Desabilitar1()));
    connect(ui->timeEdit,SIGNAL(timeChanged(QTime)),this,SLOT(Desabilitar1()));
    tabla = new QSqlTableModel(this);
    tabla1 = new QSqlTableModel(this);
    tabla2 = new QSqlTableModel(this);
}

cliente::~cliente()
{
    delete ui;
}
void cliente::Mostrar_lista_de_autos(){
   win= new Tablas_pal_cliente;
   win->show();

}
void cliente::Enviar_solicitud_de_ingreso(){

    QString dir= QFileDialog::getOpenFileName(this,"Seleccione donde esta la Base de Datos");
    dir.replace("/","//");
    db1=QSqlDatabase::addDatabase("QODBC");
    db1.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ="+dir);
    db1.open();
       QString CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,cadena;
       QDateTime fechac,fechae;
       fechac=tiempo_intant.currentDateTime();
       FechaC=fechac.toString();
       if(ui->spinBox->isHidden()&&ui->spinBox_2->isHidden()&&ui->spinBox_3->isHidden()&&ui->timeEdit->isHidden())
       {
       fechae=ui->dateTimeEdit->dateTime();
       FechaE=fechae.toString();
       }
       else if(ui->dateTimeEdit->isHidden()){
           fechae=puntero->currentDateTime();
           fechae.addDays(ui->spinBox->value()).addMonths(ui->spinBox_2->value()).addYears(ui->spinBox_3->value());
           fechae.setTime(QTime(fechae.time().hour() + ui->timeEdit->time().hour(),
                                fechae.time().minute() + ui->timeEdit->time().minute(),
                                fechae.time().second() + ui->timeEdit->time().second()));
           FechaE=fechae.toString();
       }
       Tiempdealquiler=QString("%1/%2/%3  %4 ").arg(ui->spinBox->value()).arg(ui->spinBox_2->value()).arg(ui->spinBox_3->value()).arg(ui->timeEdit->time().toString());
       CarnetdeIdentidad=ui->lineEdit->text();
       NombreClt=ui->lineEdit_1->text();
       ApellidosClt=ui->lineEdit_2->text();
       Matricula=ui->lineEdit_3->text();
       Acceder_a_precios_por_dia();
       QSqlQuery busca;
       cadena=QString("INSERT INTO Tabla5 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+CarnetdeIdentidad+"','"+NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+almacen_precios+")");
       busca.exec(cadena);
       db1.close();
       ui->spinBox->show();
       ui->spinBox_2->show();
       ui->spinBox_3->show();
       ui->timeEdit->show();
       ui->dateTimeEdit->show();
}
void cliente::Acceder_a_precios_por_dia(){
    QSqlQuery buscador;
    l=0;
    buscador.first();
    buscador.exec(QString("Select * From Tabla1"));

    while(buscador.next()){

        if(buscador.value(1).toString()==ui->lineEdit_3->text()){

         if(buscador.value(1).toString()==""||buscador.value(6)=="ND"){
              QMessageBox::critical(this,"No se guardo correctamente","No existe el auto que desea alquilar el cliente ,/n no introdujo la Matricula correctamente/n o esta intentando solicitar un automovil que no esta disponible");
         }
         else{
             if(ui->dateTimeEdit->isHidden()){

                 almacen_precios=almacen_precios.number((buscador.value(5).toInt())*(ui->spinBox->value()));}
             else if(ui->spinBox->isHidden()||ui->spinBox_2->isHidden()||ui->spinBox_3->isHidden()){
                 diadecobro=ui->dateTimeEdit->dateTime();

             almacen_precios=almacen_precios.number((buscador.value(5).toInt())*(dias->currentDateTime().daysTo(diadecobro)));
             }
              qDebug()<<almacen_precios;
         QMessageBox::information(this,"Se guardo correctamente","Su cuenta a sido registrada /n contacte con un empleado de la empresa para realizarle el alquiler");
         l=1;}
        }
    }
 if(l!=1){
       QMessageBox::critical(this,"No se guardo correctamente","No existe el auto que desea alquilar el cliente o/n no introdujo la Matricula correctamente/n o esta intentando solicitar un automovil que no esta disponible");
    }
   l=0;
}
void cliente::Desabilitar(){
    ui->spinBox->hide();
    ui->spinBox_2->hide();
    ui->spinBox_3->hide();
    ui->timeEdit->hide();
}
void cliente::Desabilitar1(){
    ui->dateTimeEdit->hide();
}
