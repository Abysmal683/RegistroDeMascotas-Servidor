#include "tcpservermanager.h"
#include <QDebug>

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

void TcpServerManager::onClientReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    QByteArray data = client->readAll();
    emit rawMessageReceived(data);

    qDebug() << "Mensaje recibido:" << data;
}

void TcpServerManager::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    clientes.removeOne(client);
    client->deleteLater();

    qDebug() << "Cliente desconectado.";
}
