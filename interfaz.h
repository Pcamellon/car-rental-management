#ifndef INTERFAZ_H
#define INTERFAZ_H
#include "inicial.h"
#include <QMessageBox>
#include <QMainWindow>
#include <QDialog>
#include <QImage>
#include <QTime>
#include <QTimer>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QInputDialog>
#include <QDir>
#include <QCloseEvent>
#include <QSound>

namespace Ui {
    class interfaz;
}
class Inicial;
class interfaz : public QMainWindow
{
    Q_OBJECT

public:
    explicit interfaz(QWidget *parent = 0);
    ~interfaz();
    QTime *temporal,*temporalmente;
    QTimer *timer;
    QTime tiempo,tiempito;
    QSound *sonido;


private:
    Ui::interfaz *ui;
     QSqlDatabase db;
     QSqlTableModel *tabla;
     QSqlTableModel *tabla1;
     QSqlTableModel *tabla2;
     QString almacen_precios,precio;
     QDateTime tiempo_intant,*puntero;
     QDir alberto;
     Inicial *intr;
     void closeEvent(QCloseEvent *ev);


public slots:
     void MostrarBD();
     void GuardarBD();
     void Combobuscador();
     void Actualizar();
     void Mostrar_imagen_del_auto();
     void MostrarClientes();
     void AgregarClientes();
     void ActualizarBDClientes();
     void Acceder_a_precios_por_dia();
     void ComboBuscador2();
     void Salir();
     void Mostrar_clientes_por_inscribir();
     void Notificacion_de_clientes();
     void Guardar_incumplimiento();
     void Aviso_de_incumplimiento();
     void Cargar_based_incumplimientos();
     void BuscarMarca();
     void BuscarModelo();
     void BuscarMatricula();
     void BuscarColor();
     void BuscarDispon();
     void BuscarPrecio();

};

#endif // INTERFAZ_H
