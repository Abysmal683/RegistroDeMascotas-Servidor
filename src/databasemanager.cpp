#include "databasemanager.h"
#include <QStandardPaths>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QThread>
DataBaseManager::DataBaseManager() {
    //tipo de sql que se utilizara
    db = QSqlDatabase::addDatabase("QSQLITE");
    //ubicacion donde se buscara o creara el db
    QString dbpath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation)+ "/DataBase_RegistroDeMascotas.db";
    //crea una carpeta si no existe
    QDir dir(QFileInfo(dbpath).path());
    //si no existe se crea
    if(!dir.exists()) dir.mkpath(dir.path());
    db.setDatabaseName(dbpath);
}
DataBaseManager::~DataBaseManager() {
    QString name = db.connectionName();
    db.close();
    QSqlDatabase::removeDatabase(name);
}
DataBaseManager& DataBaseManager::instance() {
    //la primera ves crea una instancia, la siguiente la reutiliza
    static DataBaseManager instance;
    return instance;
}
bool DataBaseManager::createTables() {
    //ubica el schema
    QFile schemaFile(":/resources/database/RegistroDeMascotas-Schema.sql");
    //abre el archivo en modo lectura de texto
    if (!schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "No se pudo abrir chatbot_schema.sql desde recursos.";
        return false;
    }
    //lee todo el contenido y cierra el archivo
    QTextStream stream(&schemaFile);
    QString sqlContent = stream.readAll();
    schemaFile.close();
    //divide los comandos para ejecutarlo 1 a 1 seperados por el ;,puede fallar si ahi ; en comentarios
    QSqlQuery q(db);
    const QStringList commands = sqlContent.split(';', Qt::SkipEmptyParts);

    if (!db.transaction()) qWarning() << "No se pudo iniciar transacción";
    //recorre los comandos y limpia para ejecutar, o captura errores
    for (const QString& cmd : commands) {
        QString trimmed = cmd.trimmed();
        if (trimmed.isEmpty())
            continue;

        if (!q.exec(trimmed)) {
            qCritical() << "Error ejecutando SQL:" << trimmed;
            qCritical() << "Detalle:" << q.lastError().text();
            db.rollback();
            return false;
        }
    }
    if (!db.commit()) {
        db.rollback();
        qCritical() << "No se pudo commitear creación de tablas";
        return false;
    }
    //si todo funciona, tira un qdebug para avisar su estado
    qDebug() << "Tablas creadas/verificadas correctamente.";
    return true;
}
bool DataBaseManager::initialize() {
    //inicia por primera ves la base de dato
    if (!db.open()) {
        qCritical() << "ERROR al abrir la base de datos:" << db.lastError().text();
        return false;
    }
    return createTables();
}
QSqlDatabase& DataBaseManager::getDatabase() {
    return db;
}
//conexion por hilos
QSqlDatabase DataBaseManager::connectionForThread() {
    QString threadName = QString("conn_%1").arg((qulonglong)QThread::currentThreadId());

    if (QSqlDatabase::contains(threadName)) {
        return QSqlDatabase::database(threadName);
    }

    QSqlDatabase conn = QSqlDatabase::addDatabase("QSQLITE", threadName);
    conn.setDatabaseName(db.databaseName());
    conn.open();

    return conn;
}
//control de conexion
bool DataBaseManager::open(){
    //si esta abierto, avisa, en caso contrario lo intenta abrir
    if(db.isOpen()) return true;
    return db.open();
}
void DataBaseManager::close(){
    //si esta abierto, lo cierra
    if(db.isOpen()) db.close();
}
bool DataBaseManager::isOpen()const {
    //envia si esta abierto o cerrado en bool
    return db.isOpen();
}
bool DataBaseManager::beginTransaction(){
    //comprueba si esta abierto, para entrar en modo transaccion segura
    if(!db.isOpen()) return false;
    return db.transaction();
}
bool DataBaseManager::commit(){
    //comprueba si esta abierto, para finalizar la transaccion segura y aprobrar si es posible
    if(!db.isOpen()) return false;
    return db.commit();
}
void DataBaseManager::rollback(){
    //si esta abierto, cancela la transaccion y regresa todo a su ultimo punto
    if(db.isOpen()) db.rollback();
}
bool DataBaseManager::clearTable(const QString &tableName){
    //comprobar si esta abierto
    if(!db.isOpen()) return false;
    //se crea un querry tipo delete y se ejecuta
    QSqlQuery q(db);
    QString sql = QStringLiteral("DELETE FROM \"%1\"").arg(tableName);
    //si falla mientras esta ejecutando, alertara
    if(!q.exec(sql)){
        qCritical() << "error limpiando tabla: " << tableName <<
            q.lastError().text();
        return false;
    }
    //si funciona, retornara true
    return true;
}
