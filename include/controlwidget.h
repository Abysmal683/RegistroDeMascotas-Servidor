#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include<QTableWidget>
namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = nullptr);
    ~ControlWidget();
private slots:
    void cargarDatos();
    void on_pushButtonRecargar_clicked();
    void mostraImagen(int row,int column);


private:
    Ui::ControlWidget *ui;
    QTableWidget *tabla;
};

#endif // CONTROLWIDGET_H
