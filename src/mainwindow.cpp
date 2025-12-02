#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QLabel>
#include "controlwidget.h"
#include "databasemanager.h"
#include "mascotadao.h"
#include "mascota.h"
#include "tcpservermanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //se conecta las clases y asigna pantalla y puerto
    controlWidget = new ControlWidget(this);
    setCentralWidget(controlWidget);
    tcpServerManager = new TcpServerManager(this);
    tcpServerManager->startServer(selectPuerto());

    //conexiones al cliente
    connect(tcpServerManager,&TcpServerManager::requestAllMascotas,
            this,&MainWindow::onRequestAllMascotas);
    connect(tcpServerManager,&TcpServerManager::requestAddMascota,
            this,&MainWindow::onRequestAddMascota);
    connect(tcpServerManager,&TcpServerManager::requestUpdateMascota,
            this,&MainWindow::onRequestUpdateMascota);
    connect(tcpServerManager,&TcpServerManager::requestDeleteMascota,
            this,&MainWindow::onRequestDeleteMascota);
    connect(tcpServerManager,&TcpServerManager::requestResearchIdMascota,
            this,&::MainWindow::onRequestResearchIdMascota);
    connect(tcpServerManager,&TcpServerManager::requestResearchNameMascota,
            this,&MainWindow::onRequestResearchNameMascota);
    connect(tcpServerManager,&TcpServerManager::requestViewImagenMascotas,
            this,&MainWindow::onRequestViewImagenMascota);
    //avisa si se conecta o desconecta un app
    connect(tcpServerManager,&TcpServerManager::newClientConnect,
            this,&MainWindow::onNewClient);
    connect(tcpServerManager,&TcpServerManager::clientDisconnect,
            this,&MainWindow::onDisconnectClient);
}
//genera una dialogo de entrada para elegir puerto
int MainWindow::selectPuerto(){
    bool ok;
    int puerto = QInputDialog::getInt(
        this, "Configuracion del Servidor",
        "Ingrese el puerto",
        8080,1,65535,1,&ok);
    if(!ok) puerto = 5050;
    //se carga el puerto en statusbar y retorna el puerto
    QLabel* labelPuerto = new QLabel(QString("Puerto: %1").arg(puerto));
    statusBar()->addPermanentWidget(labelPuerto);
    return puerto;
}
//
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
void MainWindow::onRequestUpdateMascota(QTcpSocket* client,const Mascota& m)
{
    QSqlDatabase& db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    int lastId = m.id;
    bool ok = dao.actualizar(m);
    QJsonObject resp;
    resp["type"] = ok ? "update_ok" : "update_error";
    resp["id"] = lastId;
    tcpServerManager->sendToClient(client, resp);
}
void MainWindow::onRequestDeleteMascota(QTcpSocket* client,int id)
{
    QSqlDatabase& db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    int lastId;
    bool ok = dao.eliminar(id);
    QJsonObject resp;
    resp["type"] = ok ? "delete_ok" : "delete_error";
    tcpServerManager->sendToClient(client, resp);
}
void MainWindow::onRequestResearchIdMascota(QTcpSocket* client, int id)
{
    QSqlDatabase& db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    int lastId;
    Mascota m = dao.obtenerPorId(id);
    QJsonObject resp;
    resp["type"] = "id_research";
    resp["data"] = mascotaToJson(m);
    // enviar al cliente
    tcpServerManager->sendToClient(client, resp);
}
void MainWindow::onRequestResearchNameMascota(QTcpSocket* client,QString name)
{
    QSqlDatabase& db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    const auto lista = dao.obtenerPorName(name);

    QJsonArray arr;
    for (const Mascota& m : lista)
        arr.append(mascotaToJson(m));

    QJsonObject resp;
    resp["type"] = "name_research";
    resp["data"] = arr;
    tcpServerManager->sendToClient(client, resp);
}
void MainWindow::onRequestViewImagenMascota(QTcpSocket* client,int id){
    QSqlDatabase& db = DataBaseManager::instance().getDatabase();
    MascotaDAO dao(db);
    QByteArray foto = dao.obtenerFoto(id);
    QJsonObject resp;
    resp["type"] = "view_imagen_result";
    resp["foto"] = QString::fromLatin1(foto.toBase64());
    tcpServerManager->sendToClient(client, resp);
}
void MainWindow::onNewClient(QString msg){
    statusBar()->showMessage(msg,3000);
}
void MainWindow::onDisconnectClient(QString msg){
    statusBar()->showMessage(msg,3000);
}
MainWindow::~MainWindow()
{
    delete ui;
}
