#include "interfaz.h"
#include "ui_interfaz.h"
#include "cliente.h"
#include "QString"
#include <QFileDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QDialog>
#include <QDebug>
#include <QInputDialog>
#include <QPixmap>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <qmath.h>
#include <QTime>
#include <QSize>



interfaz::interfaz(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::interfaz)
{
    ui->setupUi(this);

    this->show();
    sonido=new QSound("C:\\asd.wav",this);
    connect(ui->actionMarca,SIGNAL(triggered()),this,SLOT(BuscarMarca()));
    connect(ui->actionColor,SIGNAL(triggered()),this,SLOT(BuscarColor()));
    connect(ui->actionModelo,SIGNAL(triggered()),this,SLOT(BuscarModelo()));
    connect(ui->actionDisponibilidad,SIGNAL(triggered()),this,SLOT(BuscarDispon()));
    connect(ui->actionPrecio,SIGNAL(triggered()),this,SLOT(BuscarPrecio()));
    connect(ui->actionMatricula,SIGNAL(triggered()),this,SLOT(BuscarMatricula()));
    connect(ui->actionSalir,SIGNAL(triggered()),this,SLOT(Salir()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Mostrar_clientes_por_inscribir()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(Cargar_based_incumplimientos()));
    connect(ui->MostrarBD,SIGNAL(clicked()),this,SLOT(MostrarBD()));
    connect(ui->AregarBD,SIGNAL(clicked()),this,SLOT(GuardarBD()));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(Combobuscador()));
    connect(ui->Botonpictures,SIGNAL(clicked()),this,SLOT(Actualizar()));
    connect(ui->autindivBotom,SIGNAL(clicked()),this,SLOT(Mostrar_imagen_del_auto()));
    connect(ui->mostrarclientes,SIGNAL(clicked()),this,SLOT(MostrarClientes()));
    connect(ui->agregarclientes,SIGNAL(clicked()),this,SLOT(AgregarClientes()));
    connect(ui->actualizarclientes,SIGNAL(clicked()),this,SLOT(ActualizarBDClientes()));
    connect(ui->comboBox_2,SIGNAL(currentIndexChanged(QString)),this,SLOT(ComboBuscador2()));
    connect(ui->actionGuardar_datos_del_auto,SIGNAL(triggered()),this,SLOT(GuardarBD()));
    QString dir= QFileDialog::getOpenFileName(this,"Seleccione donde esta la Base de Datos");
    dir.replace("/","//");
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ="+dir);
    db.open();
    if(!db.isOpen()){
        QMessageBox::critical(this,"No introdujo la base de datos","Vuelva a acceder como admin e introduzca la base de datos /n correctamente");
        this->close();
    }
    tabla = new QSqlTableModel(this);
    tabla1 = new QSqlTableModel(this);
    tabla2 = new QSqlTableModel(this);
    Mostrar_clientes_por_inscribir();
    Notificacion_de_clientes();
    if(ui->label_8->text()==""){
        ui->label_8->setText("No hay notificaciones de clientes");
    }
    else{
        ui->label_8->setText("Tienes notificaciones de clientes por asignar\n a la base de datos");
    }
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    Guardar_incumplimiento();
    Aviso_de_incumplimiento();
    if(ui->label_10->text()==""){
        ui->label_10->setText("No hay clientes con incumplimiento");
    }
    else{
        ui->label_10->setText("Tiene clientes con incumplimiento");
        sonido->setLoops(3);
        sonido->play();
    }

}

interfaz::~interfaz()
{
    delete ui;
}
void interfaz::MostrarBD(){
    ui->label_3->hide();
    ui->tableView->setModel(tabla);
    ui->tableView->resizeColumnsToContents();
    tabla->setTable("Tabla1");
    tabla->select();
    tabla->sort(0,Qt::AscendingOrder);
    tabla->setEditStrategy(QSqlTableModel::OnManualSubmit);

}
void interfaz::GuardarBD(){
 ui->label_3->hide();
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
    ui->tableView->setModel(tabla1);
    ui->tableView->resizeColumnsToContents();
    tabla1->submitAll();
    tabla1->setTable("Tabla1");
    tabla1->select();
    tabla1->sort(0,Qt::AscendingOrder);
    tabla1->submitAll();


}
void interfaz::Combobuscador(){
    QSqlQuery buscador;
     if(ui->comboBox->currentText()=="Marca"){

         ui->tableView->setModel(tabla2);
         ui->tableView->resizeColumnsToContents();
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
                     ui->tableView->setModel(tabla2);
                     ui->tableView->resizeColumnsToContents();
                     tabla2->submitAll();
                     tabla2->setTable("Tabla2");
                     tabla2->select();
                     tabla2->sort(0,Qt::AscendingOrder);
                     tabla2->submitAll();
                  }

           }
           buscador.first();
           ui->comboBox->setCurrentIndex(0);
           QString busqueda = "DELETE * FROM Tabla2";
           buscador.exec(busqueda);
    }
     else if(ui->comboBox->currentText()=="Modelo"){
         ui->tableView->setModel(tabla2);
         ui->tableView->resizeColumnsToContents();
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
                     ui->tableView->setModel(tabla2);
                     ui->tableView->resizeColumnsToContents();
                     tabla2->submitAll();
                     tabla2->setTable("Tabla2");
                     tabla2->select();
                     tabla2->sort(0,Qt::AscendingOrder);
                     tabla2->submitAll();
                  }

           }
           buscador.first();
           ui->comboBox->setCurrentIndex(0);
           QString busqueda = "DELETE * FROM Tabla2";
           buscador.exec(busqueda);
     }
     else if(ui->comboBox->currentText()=="Matricula"){
         ui->tableView->setModel(tabla2);
         ui->tableView->resizeColumnsToContents();
         QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
         QSqlQuery buscador;
         buscador.first();
         QString txt=QInputDialog::getText(this,"Matricula","Introduzca la matriculla del auto que esta buscando");
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


                  if(buscador.value(1).toString()==QString("%1").arg(txt)){
                     QSqlQuery gancho;
                     cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                     gancho.exec(cadena);
                     ui->tableView->setModel(tabla2);
                     ui->tableView->resizeColumnsToContents();
                     tabla2->submitAll();
                     tabla2->setTable("Tabla2");
                     tabla2->select();
                     tabla2->sort(0,Qt::AscendingOrder);
                     tabla2->submitAll();
                  }

           }
           buscador.first();
           ui->comboBox->setCurrentIndex(0);
           QString busqueda = "DELETE * FROM Tabla2";
           buscador.exec(busqueda);
     }
     else if(ui->comboBox->currentText()=="Color"){
         ui->tableView->setModel(tabla2);
         ui->tableView->resizeColumnsToContents();
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
                     ui->tableView->setModel(tabla2);
                     ui->tableView->resizeColumnsToContents();
                     tabla2->submitAll();
                     tabla2->setTable("Tabla2");
                     tabla2->select();
                     tabla2->sort(0,Qt::AscendingOrder);
                     tabla2->submitAll();
                  }

           }
           buscador.first();
           ui->comboBox->setCurrentIndex(0);
           QString busqueda = "DELETE * FROM Tabla2";
           buscador.exec(busqueda);
     }
     else if(ui->comboBox->currentText()=="Disponibilidad"){
         ui->tableView->setModel(tabla2);
         ui->tableView->resizeColumnsToContents();
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
                     ui->tableView->setModel(tabla2);
                     ui->tableView->resizeColumnsToContents();
                     tabla2->submitAll();
                     tabla2->setTable("Tabla2");
                     tabla2->select();
                     tabla2->sort(0,Qt::AscendingOrder);
                     tabla2->submitAll();
                  }

           }
           buscador.first();
           ui->comboBox->setCurrentIndex(0);
           QString busqueda = "DELETE * FROM Tabla2";
           buscador.exec(busqueda);
     }
     else if(ui->comboBox->currentText()=="Precio"){
         ui->tableView->setModel(tabla2);
         ui->tableView->resizeColumnsToContents();
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
                     ui->tableView->setModel(tabla2);
                     ui->tableView->resizeColumnsToContents();
                     tabla2->submitAll();
                     tabla2->setTable("Tabla2");
                     tabla2->select();
                     tabla2->sort(0,Qt::AscendingOrder);
                     tabla2->submitAll();
                  }

           }
           buscador.first();
           ui->comboBox->setCurrentIndex(0);
           QString busqueda = "DELETE * FROM Tabla2";
           buscador.exec(busqueda);
     }
 }
void interfaz::Actualizar(){
    ui->label_3->hide();
    tabla->submitAll();
    tabla1->submitAll();
    tabla2->submitAll();
}
void interfaz::Mostrar_imagen_del_auto(){

    ui->label_3->show();
    QString Matricula,Marca,Modelo,Color,Precio,Estado,cadenita,caracter,linea,direccion_nueva,cambio_de_nombre;
    Matricula=ui->lineEdit_4->text();
    Marca=ui->lineEdit->text();
    Modelo=ui->lineEdit_2->text();
    Color=ui->lineEdit_3->text();
    Precio=ui->lineEdit_5->text();
    Estado=ui->lineEdit_6->text();
    cadenita=QString(""+Matricula+","+Marca+","+Modelo+","+Color+","+Precio+","+Estado+"");
    QDir direccion_carpeta("C:\\Fotos de la base de datos");
    if(!direccion_carpeta.exists()){
        direccion_carpeta.mkdir("C:\\Fotos de la base de datos");
    }
   QString imagefilename = QFileDialog::getOpenFileName( this,"Busque la imagen que desea asignar al auto","C:\\","Images (*.bmp *.png *.xpm *.jpg)");
   QDir archivo_copiado(imagefilename);
   caracter=archivo_copiado.dirName();
   direccion_nueva=QString("C:\\Fotos de la base de datos\\").append(caracter).append(".jpg");
   if(Matricula==""||Marca==""||Modelo==""||Color==""||Precio==""||Estado==""){
       QMessageBox::critical(this,"Error","No introdujo los datos del auto");
   }
   else {
   QFile cambio_de_lugar;
   cambio_de_lugar.copy(imagefilename,direccion_nueva);
   cambio_de_nombre=direccion_nueva;
   linea=cambio_de_nombre.replace(caracter,cadenita);
   linea=cambio_de_nombre.replace("/","\\");
   direccion_carpeta.rename(direccion_nueva,linea);
   QString fotillodir = QFileDialog::getOpenFileName( this,"Escoja la foto transformada para verla","C:\\Fotos de la base de datos\\","Images (*.jpg)");
   QPixmap foto;
   foto.load(fotillodir);

   ui->label_3->setPixmap( foto.scaled(ui->label_3->size(),Qt::IgnoreAspectRatio,Qt::FastTransformation));
   ui->label_3->repaint();
   ui->label_3->show();

}
}
void interfaz::MostrarClientes(){
    ui->tableView->setModel(tabla);
    ui->tableView->resizeColumnsToContents();
    tabla->setTable("Tabla3");
    tabla->select();
    tabla->sort(1,Qt::AscendingOrder);
    tabla->setEditStrategy(QSqlTableModel::OnManualSubmit);
}
void interfaz::ActualizarBDClientes(){
    ui->label_3->hide();
    tabla->submitAll();
    tabla1->submitAll();
    tabla2->submitAll();
}
void interfaz::AgregarClientes(){
       QString CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,cadena,cambiaD,letras,letrilla,cadenita,Caca;
       QString Marca,Modelo,Color,Precio,Estado;
       QVariant convertidor;
       convertidor=ui->dateTimeEdit->dateTime();
       FechaC=convertidor.toString();
       convertidor=ui->dateTimeEdit_3->dateTime();
       FechaE=convertidor.toString();
       Tiempdealquiler=QString("%1/%2/%3  %4 ").arg(ui->spinBox->value()).arg(ui->spinBox_2->value()).arg(ui->spinBox_3->value()).arg(ui->timeEdit->time().toString());
       CarnetdeIdentidad=ui->lineEdit_7->text();
       NombreClt=ui->lineEdit_8->text();
       ApellidosClt=ui->lineEdit_9->text();
       Matricula=ui->lineEdit_10->text();
       Acceder_a_precios_por_dia();
       if(Matricula==""){
           QMessageBox::critical(this,"Error","No ha introducido la matricula");
       }
       else{
       QSqlQuery busca;
       cadena=QString("INSERT INTO Tabla3 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+CarnetdeIdentidad+"','"+NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+almacen_precios+")");
       busca.exec(cadena);
       qDebug()<<cadena;
       ui->tableView->setModel(tabla1);
       ui->tableView->resizeColumnsToContents();
       tabla1->submitAll();
       tabla1->setTable("Tabla3");
       tabla1->select();
       tabla1->sort(0,Qt::AscendingOrder);
       tabla1->submitAll();
       busca.exec("Select * from Tabla1");
       while(busca.next()){
           if(busca.value(1).toString()==Matricula){
               Matricula=busca.value(1).toString();
               Marca=busca.value(2).toString();
               Modelo=busca.value(3).toString();
               Color=busca.value(4).toString();
               Precio=busca.value(5).toString();
               Estado="ND";
           }
       }
       cadena=QString(""+Matricula+","+Marca+","+Modelo+","+Color+","+Precio+","+Estado+"").append(".jpg");
       cambiaD=QString("C:\\Fotos de la base de datos\\").append(cadena);
       cadenita=QString("C:\\Fotos de la base de datos\\"+Matricula+","+Marca+","+Modelo+","+Color+","+Precio+",D").append(".jpg");
       QDir direccionDD(cadenita);
       letras=direccionDD.dirName();

       letrilla=letras.replace(letras,cadena);
       letrilla=letras.replace("/","\\");
       letrilla=QString("C:\\Fotos de la base de datos\\").append(letrilla);
       direccionDD.rename(cadenita,letrilla);

       busca.exec(QString("DELETE * from Tabla1 WHERE Matricula='%1'").arg(Matricula));
       busca.exec("INSERT INTO Tabla1 (Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
   }

}
void interfaz::Acceder_a_precios_por_dia(){
    QSqlQuery buscador;
    QString valor;
    int l=0;
    buscador.first();
    buscador.exec(QString("Select * From Tabla1"));

    while(buscador.next()){
        if(buscador.value(1).toString()==ui->lineEdit_10->text()){
            valor= buscador.value(1).toString();
            buscador.exec("Select * from Tabla5");
            while(buscador.next()){
        if(valor==buscador.value(3)){
            almacen_precios=almacen_precios.number(buscador.value(7).toInt());
        l=1;}
    }
         if(buscador.value(1).toString()==""){
              QMessageBox::critical(this,"No se guardo correctamente","No existe el auto que desea alquilar el cliente o\n no introdujo la Matricula correctamente");
         }
         else{
         QMessageBox::information(this,"Se guardo correctamente","El cliente a sido registrado en la base de datos");
         l=1;
     }
        }

    }
 if(l!=1){
       QMessageBox::critical(this,"No se guardo correctamente","No existe el auto que desea alquilar el cliente o\n no introdujo la Matricula correctamente");
    }
   l=0;
}
void interfaz::ComboBuscador2(){
    QSqlQuery buscador;
    if(ui->comboBox_2->currentText()=="Nombre"){

        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
        QString Matricula,NombreClt,ApellidosClt,FechaC,Costo,FechaE,cadena,ID,Tiempdealquiler;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Nombre","Introduzca el nombre del cliente que desea analizar");
        buscador.exec(QString("Select * From Tabla3"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              NombreClt=buscador.value(1).toString();
              ApellidosClt=buscador.value(2).toString();
              Matricula=buscador.value(3).toString();
              FechaC=buscador.value(4).toString();
              FechaE=buscador.value(5).toString();
              Tiempdealquiler=buscador.value(6).toString();
              Costo=Costo.number(buscador.value(7).toInt());


                 if(buscador.value(1).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla4 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+ID+"','"+ NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+Costo+")");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla4");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "INSERT INTO Tabla1 WHERE ";
          qDebug()<<busqueda;
          buscador.exec(busqueda);


}
   else if(ui->comboBox_2->currentText()=="Apellidos"){

        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
        QString Matricula,NombreClt,ApellidosClt,FechaC,Costo,FechaE,cadena,ID,Tiempdealquiler;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Apellidos","Introduzca el apellido del cliente que desea analizar");
        buscador.exec(QString("Select * From Tabla3"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              NombreClt=buscador.value(1).toString();
              ApellidosClt=buscador.value(2).toString();
              Matricula=buscador.value(3).toString();
              FechaC=buscador.value(4).toString();
              FechaE=buscador.value(5).toString();
              Tiempdealquiler=buscador.value(6).toString();
              Costo=Costo.number(buscador.value(7).toInt());


                 if(buscador.value(2).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla4 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+ID+"','"+ NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+Costo+")");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla4");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla4";
          qDebug()<<busqueda;
          buscador.exec(busqueda);


}
   else if(ui->comboBox_2->currentText()=="Carnet de Identidad"){

        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
        QString Matricula,NombreClt,ApellidosClt,FechaC,Costo,FechaE,cadena,ID,Tiempdealquiler;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Identificación por carnet","Introduzca el número de carnet de identidad del cliente que desea analizar");
        buscador.exec(QString("Select * From Tabla3"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              NombreClt=buscador.value(1).toString();
              ApellidosClt=buscador.value(2).toString();
              Matricula=buscador.value(3).toString();
              FechaC=buscador.value(4).toString();
              FechaE=buscador.value(5).toString();
              Tiempdealquiler=buscador.value(6).toString();
              Costo=Costo.number(buscador.value(7).toInt());


                 if(buscador.value(0).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla4 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+ID+"','"+ NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+Costo+")");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla4");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla4";
          qDebug()<<busqueda;
          buscador.exec(busqueda);


}
   else if(ui->comboBox_2->currentText()=="Fecha de inicio"){

        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
        QString Matricula,NombreClt,ApellidosClt,FechaC,Costo,FechaE,cadena,ID,Tiempdealquiler;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Fecha de Inicio","Introduzca el día de inicio de contrato de los clientes\n valgase para ello de la forma en la que se muestra en las tablas");
        buscador.exec(QString("Select * From Tabla3"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              NombreClt=buscador.value(1).toString();
              ApellidosClt=buscador.value(2).toString();
              Matricula=buscador.value(3).toString();
              FechaC=buscador.value(4).toString();
              FechaE=buscador.value(5).toString();
              Tiempdealquiler=buscador.value(6).toString();
              Costo=Costo.number(buscador.value(7).toInt());


                 if(buscador.value(4).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla4 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+ID+"','"+ NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+Costo+")");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla4");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla4";
          qDebug()<<busqueda;
          buscador.exec(busqueda);


}
   else if(ui->comboBox_2->currentText()=="Matricula de autos alquilados"){

        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
        QString Matricula,NombreClt,ApellidosClt,FechaC,Costo,FechaE,cadena,ID,Tiempdealquiler;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Matricula de autos alquilados","Introduzca la Matricula del auto alquilado por el cliente");
        buscador.exec(QString("Select * From Tabla3"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              NombreClt=buscador.value(1).toString();
              ApellidosClt=buscador.value(2).toString();
              Matricula=buscador.value(3).toString();
              FechaC=buscador.value(4).toString();
              FechaE=buscador.value(5).toString();
              Tiempdealquiler=buscador.value(6).toString();
              Costo=Costo.number(buscador.value(7).toInt());


                 if(buscador.value(3).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla4 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+ID+"','"+ NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+Costo+")");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla4");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla4";
          qDebug()<<busqueda;
          buscador.exec(busqueda);


}
   else if(ui->comboBox_2->currentText()=="Fecha de entrega"){

        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
        QString Matricula,NombreClt,ApellidosClt,FechaC,Costo,FechaE,cadena,ID,Tiempdealquiler;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Fecha de Entrega","Introduzca el día de fin de cotrato de los clientes que desea buscar\n valgase para ello de la forma en la que se muestra en las tablas");
        buscador.exec(QString("Select * From Tabla3"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              NombreClt=buscador.value(1).toString();
              ApellidosClt=buscador.value(2).toString();
              Matricula=buscador.value(3).toString();
              FechaC=buscador.value(4).toString();
              FechaE=buscador.value(5).toString();
              Tiempdealquiler=buscador.value(6).toString();
              Costo=Costo.number(buscador.value(7).toInt());


                 if(buscador.value(5).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla4 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+ID+"','"+ NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+Costo+")");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla4");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla4";
          qDebug()<<busqueda;
          buscador.exec(busqueda);


}
   else if(ui->comboBox_2->currentText()=="Tiempo de alquiler"){

        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
        QString Matricula,NombreClt,ApellidosClt,FechaC,Costo,FechaE,cadena,ID,Tiempdealquiler;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Tiempo de alquiler","Introduzca el tiempo de contrato de los clientes\n valgase para ello de la forma en la que se muestra en las tablas");
        buscador.exec(QString("Select * From Tabla3"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              NombreClt=buscador.value(1).toString();
              ApellidosClt=buscador.value(2).toString();
              Matricula=buscador.value(3).toString();
              FechaC=buscador.value(4).toString();
              FechaE=buscador.value(5).toString();
              Tiempdealquiler=buscador.value(6).toString();
              Costo=Costo.number(buscador.value(7).toInt());


                 if(buscador.value(6).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla4 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+ID+"','"+ NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+Costo+")");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla4");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla4";
          qDebug()<<busqueda;
          buscador.exec(busqueda);


}
   else if(ui->comboBox_2->currentText()=="Cobro"){

        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
        QString Matricula,NombreClt,ApellidosClt,FechaC,Costo,FechaE,cadena,ID,Tiempdealquiler;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Fecha de Inicio","Introduzca el día de inicio de contrato de los clientes");
        buscador.exec(QString("Select * From Tabla3"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              qDebug()<<ID;
              NombreClt=buscador.value(1).toString();
              ApellidosClt=buscador.value(2).toString();
              Matricula=buscador.value(3).toString();
              FechaC=buscador.value(4).toString();
              FechaE=buscador.value(5).toString();
              Tiempdealquiler=buscador.value(6).toString();
              Costo=Costo.number(buscador.value(7).toInt());


                 if(buscador.value(7).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla4 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+ID+"','"+ NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+Costo+")");
                    gancho.exec(cadena);
                    qDebug()<<cadena;
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla4");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox_2->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla4";
          qDebug()<<busqueda;
          buscador.exec(busqueda);


}
}
void interfaz::Salir(){
    this->close();
}
void interfaz::Mostrar_clientes_por_inscribir(){
    ui->tableView->setModel(tabla);
    ui->tableView->resizeColumnsToContents();
    tabla->setTable("Tabla5");
    tabla->select();
    tabla->sort(0,Qt::AscendingOrder);
    tabla->setEditStrategy(QSqlTableModel::OnManualSubmit);
}
void interfaz::closeEvent(QCloseEvent *ev){

    QSqlQuery buscador;
    QString busqueda = "DELETE * FROM Tabla5";
    buscador.exec(busqueda);
    ui->label_8->setText("");
    busqueda = "DELETE * FROM Tabla6";
    buscador.exec(busqueda);
    ui->label_10->setText("");
    db.close();
    intr = new Inicial();
    intr->exec();
    ev->accept();

}
void interfaz::Notificacion_de_clientes(){
    QSqlQuery buscador;
     QString escritura;
    buscador.first();
    buscador.exec(QString("SELECT autonumeracion FROM Tabla5"));
    while(buscador.next()){
        escritura = buscador.value(0).toString();
    }
    ui->label_8->setText(escritura);
}
void interfaz::Guardar_incumplimiento(){
   QSqlQuery buscador;
        QString Matricula,NombreClt,ApellidosClt,FechaC,Costo,FechaE,cadena,ID,Tiempdealquiler;
        buscador.first();
        QDateTime fecha_resta,fechita;
        QVariant ah;

        buscador.exec(QString("Select * From Tabla3"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              NombreClt=buscador.value(1).toString();
              ApellidosClt=buscador.value(2).toString();
              Matricula=buscador.value(3).toString();
              FechaC=buscador.value(4).toString();
              FechaE=buscador.value(5).toString();
              Tiempdealquiler=buscador.value(6).toString();
              Costo=Costo.number(buscador.value(7).toInt());
              fecha_resta=fecha_resta.currentDateTime();

              ah = buscador.value(5).toString();
              QDateTime aj = ah.toDateTime();

               if(aj<fecha_resta){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla6 (CarnetdeIdentidad,NombreClt,ApellidosClt,Matricula,FechaC,FechaE,Tiempdealquiler,Costo) VALUES('"+ID+"','"+ NombreClt+"','"+ApellidosClt+"','"+Matricula+"','"+FechaC+"','"+FechaE+"','"+Tiempdealquiler+"',"+Costo+")");
                    gancho.exec(cadena);
                    ui->label_10->setText("tatata");
                 }
          }

          buscador.first();
}
void interfaz::Aviso_de_incumplimiento(){
    QSqlQuery buscador;
     QString escritura;
    buscador.first();
    buscador.exec(QString("SELECT autonumeracion FROM Tabla6"));
    while(buscador.next()){
        escritura = buscador.value(0).toString();
    }
    ui->label_10->setText(escritura);
}
void interfaz::Cargar_based_incumplimientos(){
    ui->tableView->setModel(tabla);
    ui->tableView->resizeColumnsToContents();
    tabla->setTable("Tabla6");
    tabla->select();
    tabla->sort(0,Qt::AscendingOrder);
    tabla->setEditStrategy(QSqlTableModel::OnManualSubmit);
}
void interfaz::BuscarMarca(){
    QSqlQuery buscador;
        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
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
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
      }
void interfaz::BuscarColor(){

             ui->tableView->setModel(tabla2);
             ui->tableView->resizeColumnsToContents();
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
                         ui->tableView->setModel(tabla2);
                         ui->tableView->resizeColumnsToContents();
                         tabla2->submitAll();
                         tabla2->setTable("Tabla2");
                         tabla2->select();
                         tabla2->sort(0,Qt::AscendingOrder);
                         tabla2->submitAll();
                      }

               }
               buscador.first();
               ui->comboBox->setCurrentIndex(0);
               QString busqueda = "DELETE * FROM Tabla2";
               buscador.exec(busqueda);
           }
void interfaz::BuscarDispon(){
        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
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
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
    }
void interfaz::BuscarMatricula(){
        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
        QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
        QSqlQuery buscador;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Matricula","Introduzca la matriculla del auto que esta buscando");
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


                 if(buscador.value(1).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                    gancho.exec(cadena);
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
      }
void interfaz::BuscarModelo(){

        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
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
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
    }
void interfaz::BuscarPrecio(){
        ui->tableView->setModel(tabla2);
        ui->tableView->resizeColumnsToContents();
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
                    ui->tableView->setModel(tabla2);
                    ui->tableView->resizeColumnsToContents();
                    tabla2->submitAll();
                    tabla2->setTable("Tabla2");
                    tabla2->select();
                    tabla2->sort(0,Qt::AscendingOrder);
                    tabla2->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
    }
