#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include <QString>
#include <QMap>
#include <QDate>
#include <QPair>

class TimeTracker
{
public:
    using DateTimeRange = QPair<QDateTime, QDateTime>;
    using AppData = QMap<QString, QVector<DateTimeRange>>;

    TimeTracker();
    ~TimeTracker();
    TimeTracker(const TimeTracker&) = default;
    TimeTracker& operator=(const TimeTracker&) = default;
    TimeTracker(TimeTracker&&) = default;
    TimeTracker& operator=(TimeTracker&&) = default;

    void update();
    void saveData();
    void loadData();

    AppData getDataInRange(const QDate& beginDate, const QDate& endDate) const;

    static QString getCurrentApplicationName();

    // temporary
    void print();

private:
    AppData appData;
    QDateTime currentAppStartTime;
    QString currentAppName;

    enum class OS
    {
        Undefined,
        Windows,
        Linux
    };
#if defined(Q_OS_WINDOWS)
    constexpr static OS os = OS::Windows;
#elif defined(Q_OS_LINUX)
    constexpr static OS os = OS::Linux;
#else
    constexpr static OS os = OS::Undefined;
#endif
};

#endif // TIMETRACKER_H
