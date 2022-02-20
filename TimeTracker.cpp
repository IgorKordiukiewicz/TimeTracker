#include "TimeTracker.h"
#include <array>
#include <memory>

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#include <Psapi.h>
#endif

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
