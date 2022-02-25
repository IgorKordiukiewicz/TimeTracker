#ifndef SETTINGS_H
#define SETTINGS_H

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

struct CategorySettings
{
    QColor chartColor;
};
using CategoriesSettings = QMap<QString, CategorySettings>;

QDataStream& operator<<(QDataStream& stream, const ApplicationSettings& appSettings);
QDataStream& operator>>(QDataStream& stream, ApplicationSettings& appSettings);

QDataStream& operator<<(QDataStream& stream, const CategorySettings& categorySettings);
QDataStream& operator>>(QDataStream& stream, CategorySettings& categorySettings);

#endif // SETTINGS_H
