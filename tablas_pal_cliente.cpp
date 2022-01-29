#include "tablas_pal_cliente.h"
#include "ui_tablas_pal_cliente.h"
#include "QFileDialog"

Tablas_pal_cliente::Tablas_pal_cliente(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tablas_pal_cliente)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(AbrirBD()));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(Buscar_por()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(Mostrar_imagenes()));
    QString dir= QFileDialog::getOpenFileName(this,"Seleccione donde esta la Base de Datos");
    dir.replace("/","//");
    QSqlDatabase db1 = QSqlDatabase::addDatabase("QODBC");
    db1.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ="+dir);
    tabla3 = new QSqlTableModel(this);
    tabla4 = new QSqlTableModel(this);
    tabla5 = new QSqlTableModel(this);
}

Tablas_pal_cliente::~Tablas_pal_cliente()
{
    delete ui;
}
void Tablas_pal_cliente::AbrirBD(){
    db1.open();
    ui->tableView->setModel(tabla3);
    ui->tableView->resizeColumnsToContents();
    tabla3->setTable("Tabla1");
    tabla3->select();
    tabla3->sort(0,Qt::AscendingOrder);
    tabla3->setEditStrategy(QSqlTableModel::OnManualSubmit);
    db1.close();
}

void Tablas_pal_cliente::Mostrar_imagenes(){
    QString Matricula,Marca,Modelo,Color,Precio,Estado,cadenita,caracter,linea;
    Marca=ui->lineEdit->text();
    Modelo=ui->lineEdit_2->text();
    Color=ui->lineEdit_3->text();
    Matricula=ui->lineEdit_4->text();
    Precio=ui->lineEdit_5->text();
    Estado=ui->lineEdit_6->text();
    cadenita=QString(""+Matricula+","+Marca+","+Modelo+","+Color+","+Precio+","+Estado+"").append(".jpg");
    caracter=QString("C:\\Fotos de la base de datos\\").append(cadenita);
    QPixmap foto;
    foto.load(caracter);
    ui->label->setPixmap( foto.scaled(ui->label->size(),Qt::IgnoreAspectRatio,Qt::FastTransformation));
    ui->label->repaint();
}
void Tablas_pal_cliente::Buscar_por(){
    QSqlQuery buscador;
    db1.open();
    if(ui->comboBox->currentText()=="Marca"){

        ui->tableView->setModel(tabla4);
        ui->tableView->resizeColumnsToContents();
        QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
        buscador.first();
        QString txt=QInputDialog::getText(this,"Marca","Introduzca la marca de los autos que desea analizar");
        buscador.exec(QString("Select * From Tabla1"));

          while(buscador.next()){

              ID=buscador.value(0).toString();
              Matricula=buscador.value(1).toString();
              Marca=buscador.value(2).toString();
              Modelo=buscador.value(3).toString();
              Color=buscador.value(4).toString();
              Precio=Precio.number(buscador.value(5).toInt());
              Estado=buscador.value(6).toString();


                 if(buscador.value(2).toString()==QString("%1").arg(txt)){
                    QSqlQuery gancho;
                    cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                    gancho.exec(cadena);
                    ui->tableView->setModel(tabla4);
                    ui->tableView->resizeColumnsToContents();
                    tabla4->submitAll();
                    tabla4->setTable("Tabla2");
                    tabla4->select();
                    tabla4->sort(0,Qt::AscendingOrder);
                    tabla4->submitAll();
                 }

          }
          buscador.first();
          ui->comboBox->setCurrentIndex(0);
          QString busqueda = "DELETE * FROM Tabla2";
          buscador.exec(busqueda);
      }
          else if(ui->comboBox->currentText()=="Modelo"){
              ui->tableView->setModel(tabla4);
              ui->tableView->resizeColumnsToContents();
              QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
              QSqlQuery buscador;
              buscador.first();
              QString txt=QInputDialog::getText(this,"Modelo","Introduzca el modelo de los autos que desea analizar");
              buscador.exec(QString("Select * From Tabla1"));

                while(buscador.next()){

                    ID=buscador.value(0).toString();

                    Matricula=buscador.value(1).toString();
                    Marca=buscador.value(2).toString();
                    Modelo=buscador.value(3).toString();
                    Color=buscador.value(4).toString();
                    Precio=Precio.number(buscador.value(5).toInt());
                    Estado=buscador.value(6).toString();


                       if(buscador.value(3).toString()==QString("%1").arg(txt)){
                          QSqlQuery gancho;
                          cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                          gancho.exec(cadena);

                          ui->tableView->setModel(tabla4);
                          ui->tableView->resizeColumnsToContents();
                          tabla4->submitAll();
                          tabla4->setTable("Tabla2");
                          tabla4->select();
                          tabla4->sort(0,Qt::AscendingOrder);
                          tabla4->submitAll();
                       }

                }
                buscador.first();
                ui->comboBox->setCurrentIndex(0);
                QString busqueda = "DELETE * FROM Tabla2";
                buscador.exec(busqueda);
          }
          else if(ui->comboBox->currentText()=="Matricula"){
              ui->tableView->setModel(tabla4);
              ui->tableView->resizeColumnsToContents();
              QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
              QSqlQuery buscador;
              buscador.first();
              QString txt=QInputDialog::getText(this,"Matricula","Introduzca la matricula del auto que esta buscando");
              buscador.exec(QString("Select * From Tabla1"));

                while(buscador.next()){

                    ID=buscador.value(0).toString();
                    Matricula=buscador.value(1).toString();
                    Marca=buscador.value(2).toString();
                    Modelo=buscador.value(3).toString();
                    Color=buscador.value(4).toString();
                    Precio=Precio.number(buscador.value(5).toInt());
                    Estado=buscador.value(6).toString();


                       if(buscador.value(1).toString()==QString("%1").arg(txt)){
                          QSqlQuery gancho;
                          cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                          gancho.exec(cadena);
                          ui->tableView->setModel(tabla4);
                          ui->tableView->resizeColumnsToContents();
                          tabla4->submitAll();
                          tabla4->setTable("Tabla2");
                          tabla4->select();
                          tabla4->sort(0,Qt::AscendingOrder);
                          tabla4->submitAll();
                       }

                }
                buscador.first();
                ui->comboBox->setCurrentIndex(0);
                QString busqueda = "DELETE * FROM Tabla2";
                buscador.exec(busqueda);
          }
          else if(ui->comboBox->currentText()=="Color"){
              ui->tableView->setModel(tabla4);
              ui->tableView->resizeColumnsToContents();
              QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
              QSqlQuery buscador;
              buscador.first();
              QString txt=QInputDialog::getText(this,"Color","Introduzca el color de los autos que desea analizar");
              buscador.exec(QString("Select * From Tabla1"));

                while(buscador.next()){

                    ID=buscador.value(0).toString();
                    Matricula=buscador.value(1).toString();
                    Marca=buscador.value(2).toString();
                    Modelo=buscador.value(3).toString();
                    Color=buscador.value(4).toString();
                    Precio=Precio.number(buscador.value(5).toInt());
                    Estado=buscador.value(6).toString();


                       if(buscador.value(4).toString()==QString("%1").arg(txt)){
                          QSqlQuery gancho;
                          cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                          gancho.exec(cadena);
                          ui->tableView->setModel(tabla4);
                          ui->tableView->resizeColumnsToContents();
                          tabla4->submitAll();
                          tabla4->setTable("Tabla2");
                          tabla4->select();
                          tabla4->sort(0,Qt::AscendingOrder);
                          tabla4->submitAll();
                       }

                }
                buscador.first();
                ui->comboBox->setCurrentIndex(0);
                QString busqueda = "DELETE * FROM Tabla2";
                buscador.exec(busqueda);
          }
          else if(ui->comboBox->currentText()=="Disponibilidad"){
              ui->tableView->setModel(tabla4);
              ui->tableView->resizeColumnsToContents();
              QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
              QSqlQuery buscador;
              buscador.first();
              QString txt=QInputDialog::getText(this,"Disponibilidad","Califique la condicion de los autos que desea\n observar en Dis0ponible(escriba D) o no disponible(escriba ND)");
              buscador.exec(QString("Select * From Tabla1"));

                while(buscador.next()){

                    ID=buscador.value(0).toString();
                    Matricula=buscador.value(1).toString();
                    Marca=buscador.value(2).toString();
                    Modelo=buscador.value(3).toString();
                    Color=buscador.value(4).toString();
                    Precio=Precio.number(buscador.value(5).toInt());
                    Estado=buscador.value(6).toString();


                       if(buscador.value(6).toString()==QString("%1").arg(txt)){
                          QSqlQuery gancho;
                          cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                          gancho.exec(cadena);
                          ui->tableView->setModel(tabla4);
                          ui->tableView->resizeColumnsToContents();
                          tabla4->submitAll();
                          tabla4->setTable("Tabla2");
                          tabla4->select();
                          tabla4->sort(0,Qt::AscendingOrder);
                          tabla4->submitAll();
                       }

                }
                buscador.first();
                ui->comboBox->setCurrentIndex(0);
                QString busqueda = "DELETE * FROM Tabla2";
                buscador.exec(busqueda);
          }
          else if(ui->comboBox->currentText()=="Precio"){
              ui->tableView->setModel(tabla4);
              ui->tableView->resizeColumnsToContents();
              QString Matricula,Marca,Modelo,Color,Precio,Estado,cadena,ID;
              QSqlQuery buscador;
              buscador.first();
              QString txt=QInputDialog::getText(this,"Precio","Introduzca el precio de los autos que busca");
              buscador.exec(QString("Select * From Tabla1"));

                while(buscador.next()){

                    ID=buscador.value(0).toString();
                    Matricula=buscador.value(1).toString();
                    Marca=buscador.value(2).toString();
                    Modelo=buscador.value(3).toString();
                    Color=buscador.value(4).toString();
                    Precio=Precio.number(buscador.value(5).toInt());
                    Estado=buscador.value(6).toString();


                       if(buscador.value(5).toString()==QString("%1").arg(txt)){
                          QSqlQuery gancho;
                          cadena=QString("INSERT INTO Tabla2 (Id,Matricula,Marca,Modelo,Color,Precio,Estado) VALUES('"+ID+"','"+Matricula+"','"+Marca+"','"+Modelo+"','"+Color+"',"+Precio+",'"+Estado+"')");
                          gancho.exec(cadena);
                          ui->tableView->setModel(tabla4);
                          ui->tableView->resizeColumnsToContents();
                          tabla4->submitAll();
                          tabla4->setTable("Tabla2");
                          tabla4->select();
                          tabla4->sort(0,Qt::AscendingOrder);
                          tabla4->submitAll();
                       }

                }
                buscador.first();
                ui->comboBox->setCurrentIndex(0);
                QString busqueda = "DELETE * FROM Tabla2";
                buscador.exec(busqueda);
          }
          db1.close();

}
