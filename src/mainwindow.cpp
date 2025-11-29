#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controlwidget.h"
#include "databasemanager.h"
#include "mascotadao.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controlWidget = new ControlWidget(this);
    setCentralWidget(controlWidget);
    tcpServerManager = new TcpServerManager(this);
    tcpServerManager->startServer(5555);

    //conexiones al cliente
    connect(tcpServerManager,&TcpServerManager::rawMessageReceived,this,[this](const QByteArray &msg){
        if (msg == "recargar"){
          //  controlWidget->cargarDatos();
        }
    });
    connect(tcpServerManager,&TcpServerManager::requestAllMascotas,
            this,&MainWindow::onRequestAllMascotas);
    connect(tcpServerManager,&TcpServerManager::requestAddMascota,
            this,&MainWindow::onRequestAddMascota);
    connect(tcpServerManager,&TcpServerManager::requestUpdateMascota,
            this,&MainWindow::onRequestUpdateMascota);
    connect(tcpServerManager,&TcpServerManager::requestDeleteMascota,
            this,&MainWindow::onRequestDeleteMascota);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onRequestAllMascotas(QTcpSocket *client)
{
    QSqlDatabase& db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    const auto lista = dao.obtenerTodas();

    QJsonArray arr;
    for (const Mascota& m : lista)
        arr.append(mascotaToJson(m));

    QJsonObject resp;
    resp["type"] = "all_table";
    resp["data"] = arr;

    // enviar al cliente
        tcpServerManager->sendToClient(client, resp);
}
void MainWindow::onRequestAddMascota(QTcpSocket* client,const Mascota& m)
{
    QSqlDatabase& db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    int lastId;
    bool ok = dao.insertar(m, lastId);
    QJsonObject resp;
    resp["type"] = ok ? "insert_ok" : "insert_error";
    resp["id"] = lastId;
        tcpServerManager->sendToClient(client, resp);
}

void MainWindow::onRequestUpdateMascota(const Mascota& m)
{
    QSqlDatabase& db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    int lastId;
    dao.actualizar(m);
}
void MainWindow::onRequestDeleteMascota(int id)
{
    QSqlDatabase& db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    int lastId;
    dao.eliminar(id);
}

