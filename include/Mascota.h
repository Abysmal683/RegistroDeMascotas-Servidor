#ifndef MASCOTA_H
#define MASCOTA_H
#include <QString>
#include <QByteArray>
#include <qobject.h>
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
#endif // MASCOTA_H
