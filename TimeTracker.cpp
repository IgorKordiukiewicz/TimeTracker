#include "TimeTracker.h"
#include <array>
#include <memory>
#include <QFile>
#include "FileIO.h"

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#include <Psapi.h>
#endif

TimeTracker::TimeTracker()
{
    currentAppStartTime = QDateTime::currentDateTime();
    lastDate = QDate::currentDate();
}

TimeTracker::~TimeTracker()
{
    update();
    saveData();
}

void TimeTracker::update()
{
    const QString appName{ TimeTracker::getCurrentApplicationName() };
    if (appName == currentAppName) {
        return;
    }

    if(!appData.contains(appName) && appName != "Invalid") {
        emit newAppTracked(appName);
    }

    const QDateTime currentDateTime{ QDateTime::currentDateTime() };

    if (currentAppName.isEmpty()) {
        currentAppName = appName;
        currentAppStartTime = currentDateTime;
        return;
    }

    if (auto it{ appData.find(currentAppName) }; it != appData.end()) {
        it.value().push_back(DateTimeRange{ currentAppStartTime, currentDateTime });
    }
    else if(appName != "Invalid") {
        appData.insert(currentAppName, QVector<DateTimeRange>({ DateTimeRange{ currentAppStartTime, currentDateTime } }));
    }

    currentAppName = appName;
    currentAppStartTime = currentDateTime;
}

void TimeTracker::saveData()
{
    FileIO::save("applicationsData", appData);
}

void TimeTracker::loadData()
{
    FileIO::load("applicationsData", appData);
}

TimeTracker::AppData TimeTracker::getDataInRange(const QDate &beginDate, const QDate &endDate) const
{
    AppData data;
    for(auto it{ appData.constBegin() }; it != appData.constEnd(); ++it) {
        auto dateRangesIt{ data.insert(it.key(), {}) };
        for(DateTimeRange dateRange : appData.value(it.key())) {
            const QDate dateRangeBeginDate{ dateRange.first.date() };
            const QDate dateRangeEndDate{ dateRange.second.date() };
            if(dateRangeBeginDate >= beginDate && dateRangeEndDate <= endDate) {
                dateRangesIt->push_back(std::move(dateRange));
            }
            else if(dateRangeBeginDate < beginDate && dateRangeEndDate >= beginDate) {
                dateRange.first.setDate(beginDate);
                dateRange.first.setTime(QTime());
                dateRangesIt->push_back(std::move(dateRange));
            }
            else if(dateRangeEndDate > endDate && dateRangeBeginDate <= endDate) {
                dateRange.second.setDate(endDate.addDays(1));
                dateRange.second.setTime(QTime());
                dateRangesIt->push_back(std::move(dateRange));
            }
        }
    }

    return data;
}

const TimeTracker::AppData& TimeTracker::getData() const
{
    return appData;
}

QString TimeTracker::getCurrentApplicationName()
{
    QString appName{ "Invalid" };

    if constexpr(os == OS::Windows) {
        HWND foreground{ GetForegroundWindow() };
        if(foreground){
            DWORD procId;
            GetWindowThreadProcessId(foreground, &procId);
            if(procId){
                HANDLE handle{ OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procId) };
                if(handle){
                    TCHAR buffer[256];
                    if(GetModuleFileNameEx(handle, 0, buffer, 256)){
                        const QString str{ QString::fromWCharArray(buffer) };
                        const int idx{ static_cast<int>(str.lastIndexOf('\\')) + 1 }; // first char of the app name
                        appName = str.mid(idx, str.lastIndexOf('.') - idx);
                    }
                }
                CloseHandle(handle);
            }
        }
    }
    else if constexpr(os == OS::Linux) {
        std::array<char, 128> buffer;
        const char* cmd{ "cat /proc/$(xdotool getwindowpid $(xdotool getwindowfocus))/comm" };
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
        if(pipe){
            while(fgets(buffer.data(), buffer.size(), pipe.get())){
                appName += buffer.data();
            }
        }
    }

    return appName;
}
