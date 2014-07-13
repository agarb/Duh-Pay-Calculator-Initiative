#include <QApplication>
#include <QSettings>

#include <boost/optional.hpp>

#include "mainwindow.h"

using boost::optional;

template<typename T>
optional<T>
settingsEnumValue(const QSettings& settings, const QString& key)
{
    QVariant var = settings.value(key);
    if(var.convert(QMetaType::Int))
        return optional<T>( static_cast<T>(var.toInt()) );
    return optional<T>();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("duhpaycalc");
    a.setApplicationVersion("0.0");
    a.setApplicationDisplayName("Duh Pay Calculator Initiative");
    a.setOrganizationName("Agarb");
    a.setOrganizationDomain("agarb.github.io");

    //boot.ini can contain Format and Scope, or name of settings file,
    //in last case Ini format will be using.
    QSettings bootSets("boot.ini", QSettings::IniFormat);

    auto setsFormat =
            settingsEnumValue<QSettings::Format>(bootSets,
                                                 "settings_storage/format");

    auto setsScope =
            settingsEnumValue<QSettings::Scope>(bootSets,
                                                "settings_storage/scope");

    QString setsFileName =
            bootSets.value("settings_storage/file").toString();

    QString dbFileName = a.arguments().size()>=2 ?
                a.arguments().at(1) :
                QString();

    MainWindow w(setsFormat, setsScope,
                 setsFileName, dbFileName);
    w.show();

    return a.exec();
}
