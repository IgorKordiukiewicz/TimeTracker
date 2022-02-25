#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QString>
#include <QDataStream>
#include <QColor>
#include <QMap>

struct ApplicationSettings
{
    QString displayName;
    QString categoryName;
    QColor chartColor;
};
using ApplicationsSettings = QMap<QString, ApplicationSettings>;

QDataStream& operator<<(QDataStream& stream, const ApplicationSettings& appSettings);
QDataStream& operator>>(QDataStream& stream, ApplicationSettings& appSettings);

#endif // APPLICATIONSETTINGS_H
