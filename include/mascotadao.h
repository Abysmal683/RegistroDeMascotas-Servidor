#ifndef MASCOTADAO_H
#define MASCOTADAO_H

#include <QObject>
#include <QSqlDatabase>
#include <QByteArray>
#include <QVector>
#include <optional>

struct Mascota {
    int id = -1;
    QString nombre;
    QString raza;
    QString sexo;
    double peso;
    int edad;
    QString especie;
    QByteArray foto;   // puede estar vacío
};

class MascotaDAO {
public:
    MascotaDAO(QSqlDatabase& db);

    bool insertar(const Mascota &m, int &lastInsertId);
    std::optional<Mascota> obtenerPorId(int id);
    QVector<Mascota> obtenerTodas();
    bool actualizar(const Mascota &m);
    bool eliminar(int id);

private:
    QSqlDatabase& db;
};

#endif // MASCOTADAO_H
