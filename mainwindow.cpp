#include "mainwindow.h"
#include "ui_mainwindow.h"

//checkout testing

bool estado_serial = false, ok, conf = false, pedido = false, bandera = false, dato_valido = false;
QByteArray pedido_serie, recibido_serie(60,0); //recibido_serie(6,0);
int valor, tam = 0, tiempo_espera, contador = 5;
QString string;
char *buf;
char leido[16] = {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    Serial_Conf();
    connect(serial,SIGNAL(readyRead()),this,SLOT(Serial_Pedir_2()));
    ui->enviar_boton->setEnabled(false);
    ui->conectar_boton->setIcon(QIcon(":/pics/red-usb-disconnected-256.png"));
    ui->dato_edit->setEnabled(false);
    ui->dato_edit_2->setEnabled(false);
    ui->dato_edit_3->setEnabled(false);
    ui->dato_edit_4->setEnabled(false);
    ui->dato_edit_5->setEnabled(false);
    ui->dato_edit_6->setEnabled(false);
    ui->groupBox->setEnabled(false);
    ui->groupBox_2->setEnabled(false);
    ui->enviar_boton->setEnabled(false);
    ui->conf_boton->setEnabled(false);
    ui->serie_radio->setChecked(bandera);
    connect(ui->dato_edit,SIGNAL(editingFinished()),this,SLOT(Tipo_Dato()));
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
            ui->serie_combo->insertItem(0,info.portName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Serial_Conf()
{
    serial->setPortName(ui->serie_combo->currentText());
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}

void MainWindow::Serial_Conect()
{
    if((serial->open(QIODevice::ReadWrite)))
    {
        estado_serial = true;
        ui->conectar_boton->setIcon(QIcon(":/pics/orange-usb-connected-256.png"));
        ui->dato_edit->setEnabled(true);
        ui->dato_edit_2->setEnabled(true);
        ui->conf_boton->setEnabled(true);
        ui->serie_combo->setEnabled(false);
        ui->serie_actualizar->setEnabled(false);
        if(conf)
            ui->enviar_boton->setEnabled(true);
    }
    else
        Serial_Error();
}

void MainWindow::Serial_Desconect()
{
    serial->close();
    estado_serial = false;
    ui->enviar_boton->setEnabled(false);
    ui->conectar_boton->setIcon(QIcon(":/pics/red-usb-disconnected-256.png"));
    ui->dato_edit->setEnabled(false);
    ui->dato_edit_2->setEnabled(false);
    ui->dato_edit_3->setEnabled(false);
    ui->dato_edit_4->setEnabled(false);
    ui->dato_edit_5->setEnabled(false);
    ui->dato_edit_6->setEnabled(false);
    ui->conf_boton->setEnabled(false);
    ui->serie_combo->setEnabled(true);
    ui->serie_actualizar->setEnabled(true);
}

void MainWindow::Serial_Error()
{
    QMessageBox error;
    error.setText("Verifique la conexión de la placa.");
    error.setIcon(QMessageBox::Warning);
    error.exec();
}

void MainWindow::Serial_Pedir()                         //muestreo de datos por pedido
{
    if(ui->dato_edit->text().isEmpty())
        tam = 0;
    if((serial->bytesAvailable() >= tam) && (tam != 0))
    {
        serial->waitForReadyRead(300);
        recibido_serie.clear();
        recibido_serie = serial->readAll();
        if(recibido_serie != NULL)
        {
            recibido_serie.truncate(tam);
            ui->recibido_lista->insertItem(0,recibido_serie.toHex().toUpper());
            ui->recibido_lista->insertItem(0,"------");
        }
        qDebug() << recibido_serie.toHex();
        serial->clear();
        if(ui->serie_radio->isChecked() == true)
            ui->serie_radio->setChecked(false);
        else
            ui->serie_radio->setChecked(true);
        tam = 0;
        disconnect(serial,SIGNAL(readyRead()),0,0);
    }
}

void MainWindow::Serial_Pedir_2()                         //monitor serie
{
    qint64 quantity = serial->readLine(leido,sizeof(leido));
    if(leido[0] > 64){
        dato_valido = true;
        contador = 0;
        ui->recibido_lista->insertItem(0,"............");
    }
    if(dato_valido){
        char imprimir [16] = {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0};
        for(int h = 0; h < 16; h++){
            if(leido[h] != '\n')
                imprimir[h] = leido[h];
        }
        ui->recibido_lista->insertItem(0,imprimir);
        contador++;
    }
    if(contador > 4){
        dato_valido = false;
    }
    for(int h = 0; h < 16; h++)
        leido[h] = 0;
}


void MainWindow::on_conectar_boton_clicked()
{
    Serial_Conf();
    if(estado_serial == false)
    {
        Serial_Conect();
        serial->clear();
    }
    else{
        Serial_Desconect();
        ui->dato_edit->clear();
        ui->dato_edit_2->clear();
        ui->dato_edit_3->clear();
        ui->dato_edit_4->clear();
        ui->dato_edit_5->clear();
        ui->dato_edit_6->clear();
    }
}

void MainWindow::on_enviar_boton_clicked()
{
    QString cadena = ui->dato_edit->text();
    if(!cadena.isEmpty()){
        QString binario = QString::number(cadena.toLongLong(&ok,16),2);
        while(binario.length() < 8){
            binario.prepend("0");
        }
        qDebug() << binario;
        QString binario2 = binario.left(3);
        pedido_serie.clear();
        if((cadena == "2A")||(cadena == "0A")||(cadena == "2B")||(cadena == "0B")||(cadena == "2C")||(cadena == "0C")||(cadena == "2D")||(cadena == "0D")||(cadena == "2E")||(cadena == "0E")||(cadena == "2F")||(cadena == "0F")||(cadena == "10")||(cadena == "30")){
            tam = 6;
            if(binario2 == "000"){
                ui->dato_edit_2->setText("FF");
                ui->dato_edit_3->setText("FF");
                ui->dato_edit_4->setText("FF");
                ui->dato_edit_5->setText("FF");
                ui->dato_edit_6->setText("FF");
            }
            pedido_serie.append((char)ui->dato_edit->text().toInt(&ok,16));
            pedido_serie.append((char)ui->dato_edit_2->text().toInt(&ok,16));
            pedido_serie.append((char)ui->dato_edit_3->text().toInt(&ok,16));
            pedido_serie.append((char)ui->dato_edit_4->text().toInt(&ok,16));
            pedido_serie.append((char)ui->dato_edit_5->text().toInt(&ok,16));
            pedido_serie.append((char)ui->dato_edit_6->text().toInt(&ok,16));
        }
        else if(binario2 == "000"){
            tam = 2;
            ui->dato_edit_2->setText("FF");
            pedido_serie.append((char)ui->dato_edit->text().toInt(&ok,16));
            pedido_serie.append((char)ui->dato_edit_2->text().toInt(&ok,16));
            ui->dato_edit_3->clear();
            ui->dato_edit_4->clear();
            ui->dato_edit_5->clear();
            ui->dato_edit_6->clear();
            ui->dato_edit_3->setEnabled(false);
            ui->dato_edit_4->setEnabled(false);
            ui->dato_edit_5->setEnabled(false);
            ui->dato_edit_6->setEnabled(false);
        }
        else if(binario2 == "001"){
            tam = 2;
            pedido_serie.append((char)ui->dato_edit->text().toInt(&ok,16));
            pedido_serie.append((char)ui->dato_edit_2->text().toInt(&ok,16));        }
    }
    else{                                               //Si el primer casillero está vacío
        pedido_serie.clear();
        ui->dato_edit_2->clear();
        ui->dato_edit_3->clear();
        ui->dato_edit_4->clear();
        ui->dato_edit_5->clear();
        ui->dato_edit_6->clear();
        ui->dato_edit_3->setEnabled(false);
        ui->dato_edit_4->setEnabled(false);
        ui->dato_edit_5->setEnabled(false);
        ui->dato_edit_6->setEnabled(false);
    }
    if(pedido_serie != NULL){
        connect(serial,SIGNAL(readyRead()),this,SLOT(Serial_Pedir()));
        serial->write(pedido_serie);
        serial->waitForBytesWritten(30);
        ui->dato_lista->insertItem(0,pedido_serie.toHex().toUpper());
        ui->dato_lista->insertItem(0,"------");
        qDebug() << pedido_serie;
    }
}

void MainWindow::on_pushButton_clicked()        //Botón Limpiar
{
    ui->recibido_lista->clear();
    ui->dato_lista->clear();
    ui->dato_edit->clear();
    ui->dato_edit_2->clear();
    ui->dato_edit_3->clear();
    ui->dato_edit_4->clear();
    ui->dato_edit_5->clear();
    ui->dato_edit_6->clear();
    ui->dato_edit_3->setEnabled(false);
    ui->dato_edit_4->setEnabled(false);
    ui->dato_edit_5->setEnabled(false);
    ui->dato_edit_6->setEnabled(false);
    tam = 0;
    serial->clear();
}

void MainWindow::on_conf_boton_clicked()
{
    if(estado_serial){
        if(conf == false){
            disconnect(serial,SIGNAL(readyRead()),0,0);
            ui->groupBox->setEnabled(true);
            ui->groupBox_2->setEnabled(true);
            ui->enviar_boton->setEnabled(true);
            ui->enviar_boton->setEnabled(true);
            conf = true;
            //connect(serial,SIGNAL(readyRead()),this,SLOT(Serial_Pedir()));
            tam = 0;
            serial->clear();
        }
        else{
            disconnect(serial,SIGNAL(readyRead()),0,0);
            ui->groupBox->setEnabled(false);
            ui->groupBox_2->setEnabled(false);
            ui->enviar_boton->setEnabled(false);
            ui->enviar_boton->setEnabled(false);
            conf = false;
            ui->dato_edit->clear();
            ui->dato_edit_2->clear();
            ui->dato_edit_3->clear();
            ui->dato_edit_4->clear();
            ui->dato_edit_5->clear();
            ui->dato_edit_6->clear();
            connect(serial,SIGNAL(readyRead()),this,SLOT(Serial_Pedir_2()));
            if(estado_serial)
                serial->clear();
        }
    }
}

void MainWindow::Tipo_Dato()
{
    QString cadena = ui->dato_edit->text();
    if(!cadena.isEmpty()){
        QString binario = QString::number(cadena.toLongLong(&ok,16),2);
        while(binario.length() < 8){
            binario.prepend("0");
        }
        QString binario2 = binario.left(3);
        if((cadena == "2A")||(cadena == "0A")||(cadena == "2B")||(cadena == "0B")||(cadena == "2C")||(cadena == "0C")||(cadena == "2D")||(cadena == "0D")||(cadena == "2E")||(cadena == "0E")||(cadena == "2F")||(cadena == "0F")||(cadena == "10")||(cadena == "30")){
           ui->dato_edit_3->setEnabled(true);
           ui->dato_edit_4->setEnabled(true);
           ui->dato_edit_5->setEnabled(true);
           ui->dato_edit_6->setEnabled(true);
           if(binario2 == "001"){
                ui->dato_edit_2->clear();
                ui->dato_edit_3->clear();
                ui->dato_edit_4->clear();
                ui->dato_edit_5->clear();
                ui->dato_edit_6->clear();
            }
            else if(binario2 == "000"){
                ui->dato_edit_2->setText("FF");
                ui->dato_edit_3->setText("FF");
                ui->dato_edit_4->setText("FF");
                ui->dato_edit_5->setText("FF");
                ui->dato_edit_6->setText("FF");
            }
        }
        else{
            ui->dato_edit_2->setText("FF");
            ui->dato_edit_3->clear();
            ui->dato_edit_4->clear();
            ui->dato_edit_5->clear();
            ui->dato_edit_6->clear();
            ui->dato_edit_3->setEnabled(false);
            ui->dato_edit_4->setEnabled(false);
            ui->dato_edit_5->setEnabled(false);
            ui->dato_edit_6->setEnabled(false);
        }
    }
    else{
        ui->dato_edit_2->clear();
        ui->dato_edit_3->clear();
        ui->dato_edit_4->clear();
        ui->dato_edit_5->clear();
        ui->dato_edit_6->clear();
        ui->dato_edit_3->setEnabled(false);
        ui->dato_edit_4->setEnabled(false);
        ui->dato_edit_5->setEnabled(false);
        ui->dato_edit_6->setEnabled(false);
    }
}

void MainWindow::on_serie_actualizar_clicked()
{
    ui->serie_combo->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
            ui->serie_combo->insertItem(0,info.portName());
    }
}
