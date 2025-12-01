#ifndef MASCOTA_H
#define MASCOTA_H
#include <QString>
#include <QByteArray>
#include <qobject.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
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
Q_DECLARE_METATYPE(Mascota);

inline QJsonObject mascotaToJson(const Mascota& m)
{
    QJsonObject obj;
    obj["id"] = m.id;
    obj["nombre"] = m.nombre;
    obj["raza"] = m.raza;
    obj["sexo"] = m.sexo;
    obj["peso"] = m.peso;
    obj["edad"] = m.edad;
    obj["especie"] = m.especie;

    // La foto va en base64 (solo si existe)
    obj["foto"] = QString(m.foto.toBase64());

    return obj;
}

inline Mascota mascotaFromJson(const QJsonObject& obj)
{
    Mascota m;

    m.id = obj["id"].toInt();
    m.nombre = obj["nombre"].toString();
    m.raza = obj["raza"].toString();
    m.sexo = obj["sexo"].toString();
    m.peso = obj["peso"].toDouble();
    m.edad = obj["edad"].toInt();
    m.especie = obj["especie"].toString();

    // Foto en base64 → QByteArray
    m.foto = QByteArray::fromBase64(obj["foto"].toString().toUtf8());

    return m;
}
#endif // MASCOTA_H
