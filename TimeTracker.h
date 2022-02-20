#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include <QString>

class TimeTracker
{
public:
    static QString getCurrentApplicationName();

private:
    enum class OS
    {
        Windows,
        Linux
    };
#if defined(Q_OS_WINDOWS)
    constexpr static OS os = OS::Windows;
#elif defined(Q_OS_LINUX)
    constexpr static OS os = OS::Linux;
#endif
};

#endif // TIMETRACKER_H
