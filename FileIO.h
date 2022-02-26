#ifndef FILEIO_H
#define FILEIO_H

#include <QFile>
#include <QString>
#include <QDebug>

namespace FileIO
{
    template<typename T>
    void load(const QString& fileName, T& data)
    {
        QFile file{ fileName };
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file: " << fileName << " for reading";
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
            qDebug() << "Failed to open file: " << fileName << " for writing";
            return;
        }

        QDataStream stream{ &file };
        stream.setVersion(QDataStream::Qt_6_2);
        stream << data;

        file.flush();
        file.close();
    }
}

#endif // FILEIO_H
