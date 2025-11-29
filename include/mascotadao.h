#ifndef MASCOTADAO_H
#define MASCOTADAO_H

#include <QObject>
#include <QSqlDatabase>
#include <QVector>
#include "Mascota.h"


class MascotaDAO {
public:
    MascotaDAO(QSqlDatabase& db);

    bool insertar(const Mascota &m, int &lastInsertId);
    Mascota obtenerPorId(int id);
    QVector<Mascota> obtenerTodas();
    bool actualizar(const Mascota &m);
    bool eliminar(int id);

private:
    QSqlDatabase& db;
};

#endif // MASCOTADAO_H
