#ifndef VENTANA_H
#define VENTANA_H

#include <QWidget>

namespace Ui {
class Ventana;
}

class Ventana : public QWidget
{
    Q_OBJECT

public:
    explicit Ventana(QWidget *parent = 0);
    ~Ventana();

private slots:
    void on_config_boton_enviar_clicked();

private:
    Ui::Ventana *ui;
};

#endif // VENTANA_H
