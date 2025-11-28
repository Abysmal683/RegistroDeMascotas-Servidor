#include "mascotadao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

MascotaDAO::MascotaDAO(QSqlDatabase& db) : db(db){}

// ----------------------------------------------------------
bool MascotaDAO::insertar(const Mascota &m, int &lastInsertId)
{
    if (!db.isOpen()) {
        qCritical() << "DB cerrada en insertar().";
        return false;
    }

    QSqlQuery q(db);
    q.prepare(R"(
        INSERT INTO mascotas
        (nombre, raza, sexo, peso, edad, especie, foto)
        VALUES (:nombre, :raza, :sexo, :peso, :edad, :especie, :foto)
    )");

    q.bindValue(":nombre", m.nombre);            // nombre sin normalizar
    q.bindValue(":raza", m.raza.toLower());
    q.bindValue(":sexo", m.sexo.toLower());
    q.bindValue(":peso", m.peso);
    q.bindValue(":edad", m.edad);
    q.bindValue(":especie", m.especie.toLower());
    q.bindValue(":foto", m.foto);

    if (!q.exec()) {
        qCritical() << "Error INSERT mascota:" << q.lastError().text();
        return false;
    }

    lastInsertId = q.lastInsertId().toInt();
    return true;
}

// ----------------------------------------------------------
std::optional<Mascota> MascotaDAO::obtenerPorId(int id)
{
    if (!db.isOpen()) {
        qCritical() << "DB cerrada en obtenerPorId().";
        return std::nullopt;
    }

    QSqlQuery q(db);
    q.prepare("SELECT id, nombre, raza, sexo, peso, edad, especie, foto "
              "FROM mascotas WHERE id = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qCritical() << "Error SELECT por id:" << q.lastError().text();
        return std::nullopt;
    }

    if (!q.next())
        return std::nullopt;

    Mascota m;
    m.id = q.value(0).toInt();
    m.nombre = q.value(1).toString();
    m.raza = q.value(2).toString();
    m.sexo = q.value(3).toString();
    m.peso = q.value(4).toDouble();
    m.edad = q.value(5).toInt();
    m.especie = q.value(6).toString();
    m.foto = q.value(7).toByteArray();

    return m;
}

// ----------------------------------------------------------
QVector<Mascota> MascotaDAO::obtenerTodas()
{
    QVector<Mascota> lista;

    if (!db.isOpen()) {
        qCritical() << "DB cerrada en obtenerTodas().";
        return lista;
    }

    QSqlQuery q(db);
    if (!q.exec("SELECT id, nombre, raza, sexo, peso, edad, especie, foto FROM mascotas")) {
        qCritical() << "Error SELECT todas:" << q.lastError().text();
        return lista;
    }

    while (q.next()) {
        Mascota m;
        m.id = q.value(0).toInt();
        m.nombre = q.value(1).toString();
        m.raza = q.value(2).toString();
        m.sexo = q.value(3).toString();
        m.peso = q.value(4).toDouble();
        m.edad = q.value(5).toInt();
        m.especie = q.value(6).toString();
        m.foto = q.value(7).toByteArray();

        lista.append(m);
    }

    return lista;
}

// ----------------------------------------------------------
bool MascotaDAO::actualizar(const Mascota &m)
{
    if (m.id <= 0) {
        qWarning() << "ID inválido en actualizar().";
        return false;
    }

    QSqlQuery q(db);
    q.prepare(R"(
        UPDATE mascotas
        SET nombre = :nombre,
            raza = :raza,
            sexo = :sexo,
            peso = :peso,
            edad = :edad,
            especie = :especie,
            foto = :foto
        WHERE id = :id
    )");

    q.bindValue(":nombre", m.nombre);
    q.bindValue(":raza", m.raza.toLower());
    q.bindValue(":sexo", m.sexo.toLower());
    q.bindValue(":peso", m.peso);
    q.bindValue(":edad", m.edad);
    q.bindValue(":especie", m.especie.toLower());
    q.bindValue(":foto", m.foto);
    q.bindValue(":id", m.id);

    if (!q.exec()) {
        qCritical() << "Error UPDATE mascota:" << q.lastError().text();
        return false;
    }

    return true;
}

// ----------------------------------------------------------
bool MascotaDAO::eliminar(int id)
{
    if (id <= 0)
        return false;

    QSqlQuery q(db);
    q.prepare("DELETE FROM mascotas WHERE id = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qCritical() << "Error DELETE mascota:" << q.lastError().text();
        return false;
    }

    return true;
}
