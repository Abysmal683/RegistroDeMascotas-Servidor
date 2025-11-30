#include "tcpservermanager.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "Mascota.h"

TcpServerManager::TcpServerManager(QObject *parent)
    : QObject(parent), server(new QTcpServer(this))
{
    connect(server, &QTcpServer::newConnection,
            this, &TcpServerManager::onNewConnection);
}

bool TcpServerManager::startServer(quint16 port)
{
    if (!server->listen(QHostAddress::Any, port)) {
        qCritical() << "No se pudo iniciar servidor TCP en puerto" << port;
        return false;
    }

    qDebug() << "Servidor TCP escuchando en puerto" << port;
    return true;
}

void TcpServerManager::sendToClient(QTcpSocket* client, const QJsonObject& obj)
{
    QJsonDocument doc(obj);
    client->write(doc.toJson(QJsonDocument::Compact)+"\n");
}
void TcpServerManager::onNewConnection()
{
    QTcpSocket *client = server->nextPendingConnection();
    clientes.append(client);

    qDebug() << "Cliente conectado:" << client->peerAddress().toString();

    connect(client, &QTcpSocket::readyRead,
            this, &TcpServerManager::onClientReadyRead);

    connect(client, &QTcpSocket::disconnected,
            this, &TcpServerManager::onClientDisconnected);
}
QList<QTcpSocket*> TcpServerManager::getClients(){
    return clientes;
}
void TcpServerManager::onClientReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    QByteArray data = client->readAll();
    qDebug() << "Mensaje recibido:" << data;

    //Interpretar JSON
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);

    if (err.error != QJsonParseError::NoError) {
        qWarning() << "JSON inválido:" << err.errorString();
        return;
    }
    if(!doc.isObject()){
        qWarning()<<"JSON no es un Objecto";
        return;
    }

    QJsonObject obj = doc.object();
    if(!obj.contains("type")){
        qWarning() << "JSON sin type";
        return;
    }
    QString type = obj["type"].toString();

    if (type == "request_all") {
        emit requestAllMascotas(client);
        return;
    }

    if (type == "add") {
        if(!obj.contains("data")){
            qWarning() << "JSON sin data";
            return;
        }
        Mascota m = mascotaFromJson(obj["data"].toObject());
        emit requestAddMascota(client,m);
        return;
    }

    if (type == "update") {
        if(!obj.contains("data")){
            qWarning() << "JSON sin data";
            return;
        }
        Mascota m = mascotaFromJson(obj["data"].toObject());
        emit requestUpdateMascota(m);
        return;
    }

    if (type == "delete") {
        if(!obj.contains("id")){
            qWarning() << "JSON sin id";
            return;
        }
        emit requestDeleteMascota(obj["id"].toInt());
        return;
    }
}

void TcpServerManager::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    clientes.removeOne(client);
    client->deleteLater();

    qDebug() << "Cliente desconectado.";
}
