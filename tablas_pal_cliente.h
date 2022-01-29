#ifndef TABLAS_PAL_CLIENTE_H
#define TABLAS_PAL_CLIENTE_H
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

#include <QMainWindow>

namespace Ui {
    class Tablas_pal_cliente;
}

class Tablas_pal_cliente : public QMainWindow
{
    Q_OBJECT

public:
    explicit Tablas_pal_cliente(QWidget *parent = 0);
    ~Tablas_pal_cliente();

private:
    Ui::Tablas_pal_cliente *ui;
    QSqlDatabase db1;
    QSqlTableModel *tabla3;
    QSqlTableModel *tabla4;
    QSqlTableModel *tabla5;
    QString almacen_precios,precio;

private slots:
    void AbrirBD();
    void Mostrar_imagenes();
    void Buscar_por();
};

#endif // TABLAS_PAL_CLIENTE_H
