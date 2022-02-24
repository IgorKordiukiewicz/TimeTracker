#include "TimeTracker.h"
#include <array>
#include <memory>
#include <QFile>

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#include <Psapi.h>
#endif

TimeTracker::TimeTracker()
{
    currentAppStartTime = QDateTime::currentDateTime();
}

TimeTracker::~TimeTracker()
{
    update();
    saveData();
}

void TimeTracker::update()
{
    const QString appName = TimeTracker::getCurrentApplicationName();
    if (appName == currentAppName) {
        return;
    }

    const QDateTime currentDateTime = QDateTime::currentDateTime();

    if (currentAppName.isEmpty()) {
        currentAppName = appName;
        currentAppStartTime = currentDateTime;
        return;
    }

    if (auto it = appData.find(currentAppName); it != appData.end()) {
        it.value().push_back(DateTimeRange(currentAppStartTime, currentDateTime));
    }
    else {
        appData.insert(currentAppName, QVector<DateTimeRange>({DateTimeRange(currentAppStartTime, currentDateTime)}));
    }

    currentAppName = appName;
    currentAppStartTime = currentDateTime;
}

void TimeTracker::saveData()
{
    const QString fileName = "data.txt";
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_6_2);

    stream << appData;

    file.flush();
    file.close();
}

void TimeTracker::loadData()
{
    const QString fileName = "data.txt";
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_6_2);

    stream >> appData;

    file.close();

    print();
}

TimeTracker::AppData TimeTracker::getDataInRange(const QDate &beginDate, const QDate &endDate) const
{
    AppData data;

    const auto keys = appData.keys();
    for(const auto& key : keys) {
        auto it = data.insert(key, {});
        for(auto dateRange : appData.value(key)) {
            const QDate dateRangeBeginDate = dateRange.first.date();
            const QDate dateRangeEndDate = dateRange.second.date();
            if(dateRangeBeginDate >= beginDate && dateRangeEndDate <= endDate) {
                it->push_back(std::move(dateRange));
            }
            else if(dateRangeBeginDate < beginDate && dateRangeEndDate >= beginDate) {
                dateRange.first.setDate(beginDate);
                it->push_back(std::move(dateRange));
            }
            else if(dateRangeEndDate > endDate && dateRangeBeginDate <= endDate) {
                dateRange.second.setDate(endDate);
                it->push_back(std::move(dateRange));
            }
        }
    }

    return data;
}

QString TimeTracker::getCurrentApplicationName()
{
    QString appName = "Invalid";

    if constexpr(os == OS::Windows) {
        HWND foreground = GetForegroundWindow();
        if(foreground){
            DWORD procId;
            GetWindowThreadProcessId(foreground, &procId);
            if(procId){
                HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procId);
                if(handle){
                    TCHAR buffer[256];
                    if(GetModuleFileNameEx(handle, 0, buffer, 256)){
                        QString str = QString::fromWCharArray(buffer);
                        const int idx = str.lastIndexOf('\\') + 1; // first char of the app name
                        appName = str.mid(idx, str.lastIndexOf('.') - idx);
                    }
                }
                CloseHandle(handle);
            }
        }
    }
    else if constexpr(os == OS::Linux) {
        std::array<char, 128> buffer;
        const char* cmd = "cat /proc/$(xdotool getwindowpid $(xdotool getwindowfocus))/comm";
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
        if(pipe){
            while(fgets(buffer.data(), buffer.size(), pipe.get())){
                appName += buffer.data();
            }
        }
    }

    return appName;
}

void TimeTracker::print()
{
    for(auto key : appData.keys()) {
        qDebug() << key << '\n';
        for(auto& V : appData.value(key)){
            qDebug() << V.first << " " << V.second << '\n';
        }
    }
}
