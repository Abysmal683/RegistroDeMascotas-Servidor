#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "databasemanager.h"
#include "mascota.h"
#include <QMetaType>
int main(int argc, char *argv[])
{
    qRegisterMetaType<Mascota>("Mascota");
    QApplication a(argc, argv);
    if (!DataBaseManager::instance().initialize()){
        qCritical() << "No se pudo inicialziar el sql.";
    }

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "RegistroDeMascotas-Servidor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
