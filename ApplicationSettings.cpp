#include "ApplicationSettings.h"

QDataStream& operator<<(QDataStream& stream, const ApplicationSettings& appSettings)
{
    stream << appSettings.displayName;
    stream << appSettings.categoryName;
    stream << appSettings.chartColor;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ApplicationSettings& appSettings)
{
    stream >> appSettings.displayName;
    stream >> appSettings.categoryName;
    stream >> appSettings.chartColor;
    return stream;
}
