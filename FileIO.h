#ifndef FILEIO_H
#define FILEIO_H

#include <QFile>
#include <QString>

namespace FileIO
{
    template<typename T>
    void load(const QString& fileName, T& data)
    {
        QFile file{ fileName };
        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }

        QDataStream stream{ &file };
        stream.setVersion(QDataStream::Qt_6_2);
        stream >> data;

        file.close();
    }

    template<typename T>
    void save(const QString& fileName, const T& data)
    {
        QFile file{ fileName };
        if(!file.open(QIODevice::WriteOnly)) {
            return;
        }

        QDataStream stream{ &file };
        stream.setVersion(QDataStream::Qt_6_2);
        stream << data;

        file.close();
    }
}

#endif // FILEIO_H
