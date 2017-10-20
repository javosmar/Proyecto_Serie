#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QMessageBox>
#include <iostream>
#include <QDebug>
#include <QSerialPortInfo>

//continúa siendo basofia

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void Serial_Conf();

    void Serial_Conect();

    void Serial_Desconect();

    void Serial_Error();

    void Serial_Pedir();

    void on_conectar_boton_clicked();

    void on_enviar_boton_clicked();

    void on_pushButton_clicked();

    void on_conf_boton_clicked();

    void Tipo_Dato();

    void on_serie_actualizar_clicked();

    void Serial_Pedir_2();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

};

#endif // MAINWINDOW_H
