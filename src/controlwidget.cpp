#include "controlwidget.h"
#include "Mascota.h"
#include "ui_controlwidget.h"
#include "databasemanager.h"
#include "mascotadao.h"
#include <QString>
ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ControlWidget)
{
    ui->setupUi(this);
    tabla = ui->tableWidgetVista;
    cargarDatos();
    connect(ui->tableWidgetVista,&QTableWidget::cellClicked,
        this,&ControlWidget::mostraImagen);
}

ControlWidget::~ControlWidget()
{
    delete ui;
}
void ControlWidget::cargarDatos(){
    ui->pushButtonRecargar->setEnabled(false);
    tabla->clear();
    tabla->setRowCount(0);
    QSqlDatabase &db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    const QVector<Mascota> lista = dao.obtenerTodas();
     QStringList headers = {"ID","Nombre","Raza","Genero","Peso kg","Edad","Especie"};
    tabla->setColumnCount(headers.size());
    tabla->setHorizontalHeaderLabels(headers);
    int row=0;
    for(const Mascota &m : lista){
        tabla->insertRow(row);
        tabla->setItem(row,0,new QTableWidgetItem(QString::number(m.id)));
        tabla->setItem(row,1,new QTableWidgetItem(m.nombre));
        tabla->setItem(row,2,new QTableWidgetItem(m.raza));
        tabla->setItem(row,3,new QTableWidgetItem(m.sexo));
        tabla->setItem(row,4,new QTableWidgetItem(QString::number(m.peso)));
        tabla->setItem(row,5,new QTableWidgetItem(QString::number(m.edad)));
        tabla->setItem(row,6,new QTableWidgetItem(m.especie));
        row++;
    }
    tabla->resizeColumnsToContents();
    ui->pushButtonRecargar->setEnabled(true);
}
void ControlWidget::on_pushButtonRecargar_clicked(){
    cargarDatos();
}
void ControlWidget::mostraImagen(int row,int column){
    Q_UNUSED(column);
    int id = ui->tableWidgetVista->item(row,0)->text().toInt();
    QSqlDatabase &db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    Mascota m = dao.obtenerPorId(id);

    QPixmap foto;
    foto.loadFromData(m.foto);
    ui->labelImagen->setPixmap(foto.scaled(ui->labelImagen->size(),
        Qt::KeepAspectRatio,Qt::SmoothTransformation));
}
