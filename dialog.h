#ifndef DIALOG_H
#define DIALOG_H
#include "QString"
#include <QtSql/QSqlTableModel>
#include <QMessageBox>
#include <QMainWindow>
#include <QDialog>
#include <QImage>
#include <QTime>
#include <QTimer>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QInputDialog>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();


private:
    Ui::Dialog *ui;
    QSqlDatabase db2;
    QString dir;
    QSqlTableModel *tabla3;
    QSqlTableModel *tabla1;
    QSqlTableModel *tabla2;
private slots:
    void MostrarBD();
    void GuardarBD();
    void Combobuscador();
    void Actualizar();
    void Mostrar_imagen_del_auto();
    void Salir();
};

#endif // DIALOG_H
