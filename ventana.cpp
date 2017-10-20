#include "ventana.h"
#include "ui_ventana.h"
#include "mainwindow.h"

int tam = 0;
bool ok;

Ventana::Ventana(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ventana)
{
    ui->setupUi(this);
}

Ventana::~Ventana()
{
    delete ui;
}

void Ventana::on_config_boton_enviar_clicked()
{
    pedido_serie.clear();
    if(ui->config_edit_reg->text() != NULL){
        pedido_serie.append((char)ui->config_edit_reg->text().toInt(&ok,16));
        tam = 1;
    }
    if(ui->config_edit_d1->text() != NULL){
        pedido_serie.append((char)ui->config_edit_d1->text().toInt(&ok,16));
        tam = 2;
    }
    if(ui->config_edit_d2->text() != NULL){
        pedido_serie.append((char)ui->config_edit_d2->text().toInt(&ok,16));
        tam = 3;
    }
    if(ui->config_edit_d3->text() != NULL){
        pedido_serie.append((char)ui->config_edit_d3->text().toInt(&ok,16));
        tam = 4;
    }
    if(ui->config_edit_d4->text() != NULL){
        pedido_serie.append((char)ui->config_edit_d4->text().toInt(&ok,16));
        tam = 5;
    }
    if(ui->config_edit_d5->text() != NULL){
        pedido_serie.append((char)ui->config_edit_d5->text().toInt(&ok,16));
        tam = 6;
    }
    if(pedido_serie != NULL){
        serial->write(pedido_serie);                            //escribimos en el buffer
        serial->waitForBytesWritten(30);
        ui->dato_lista->insertItem(0,pedido_serie.toHex().toUpper());
        ui->dato_lista->insertItem(0,"------");
        qDebug() << pedido_serie;
        ui->dato_edit->cursorWordBackward(true);
    }
}
