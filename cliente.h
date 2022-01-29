#ifndef CLIENTE_H
#define CLIENTE_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include "tablas_pal_cliente.h"
#include "interfaz.h"
#include "QString"
#include "QDateTime"

namespace Ui {
    class cliente;
}

class cliente : public QMainWindow
{
    Q_OBJECT

public:
    explicit cliente(QWidget *parent = 0);
    ~cliente();

    QSqlDatabase db1;
    QSqlTableModel *tabla;
    QSqlTableModel *tabla1;
    QSqlTableModel *tabla2;
    QString almacen_precios;
    QDateTime tiempo_intant,*puntero;



private:
    Ui::cliente *ui;
    Tablas_pal_cliente *win;
    QDateTime *dias,diadecobro;
        int l;
private slots:
    void Mostrar_lista_de_autos();
    void Enviar_solicitud_de_ingreso();
    void Acceder_a_precios_por_dia();
    void Desabilitar();
    void Desabilitar1();

};

#endif // CLIENTE_H
