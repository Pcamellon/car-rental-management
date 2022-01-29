#include "inicial.h"
#include "ui_inicial.h"
#include "QMessageBox"

Inicial::Inicial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Inicial)
{
    ui->setupUi(this);
    ui->label_2->setText("Introduzca el nombre de usuario y contraseña\n para acceder al programa\n Puede acceder como Cliente o como admin");
}

Inicial::~Inicial()
{
    delete ui;
}

void Inicial::on_pushButton_clicked()
{
    if(ui->lineEdit->text()=="admin" && ui->lineEdit_2->text()=="123"){

        this->close();
        k = new interfaz;
        k->show();
    }
    else if(ui->lineEdit->text()=="Cliente" && ui->lineEdit_2->text()=="Cliente"){

        this->close();
        c = new cliente;
        c->show();
    }
    else{
        QMessageBox::warning(this,"ERROR","Contraseña o nombre \n de usuario incorrectos");
    }
}

void Inicial::on_lineEdit_textChanged(QString )
{
    if(ui->lineEdit->text()=="admin"){
        ui->label_2->setText("Usted quiere a acceder al programa a traves de:\n la cuenta de administración");
    }
    else if(ui->lineEdit->text()=="Cliente"){
        ui->label_2->setText("Usted quiere a acceder al programa a traves de:\n la cuenta de cliente\n Contraseña: Cliente");
    }
    else{
       ui->label_2->setText("Introduzca el nombre de usuario y contraseña\n para acceder al programa\n Puede acceder como Cliente o como admin");
    }
}

void Inicial::on_pushButton_2_clicked()
{
    this->close();
}

