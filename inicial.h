#ifndef INICIAL_H
#define INICIAL_H
#include "interfaz.h"
#include "ui_interfaz.h"
#include "cliente.h"
#include "ui_cliente.h"
#include <QWidget>

namespace Ui {
    class Inicial;
}
class interfaz;
class cliente;
class Inicial : public QDialog
{
    Q_OBJECT

public:
    explicit Inicial(QWidget *parent = 0);
    ~Inicial();


private:
    Ui::Inicial *ui;
    interfaz *k;
    cliente *c;


private slots:

    void on_pushButton_2_clicked();
    void on_lineEdit_textChanged(QString );
    void on_pushButton_clicked();
};

#endif // INICIAL_H
