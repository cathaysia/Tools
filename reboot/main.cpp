/**
 * 2020年8月16日 20:33:21
 * 用来重启计算机或者强制重启计算机
 */

#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

bool reboot(bool force = false);

int main(int argc, char **argv) {
    bool result = true;
    if (argc != 1 && (!strcmp(argv[0], "-f") || !strcmp(argv[0], "--force")))
        result = reboot(true);
    else reboot();
    cout<<boolalpha<<result;
    return 0;
}

bool reboot(bool force) {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    // 提权
    if (!OpenProcessToken(GetCurrentProcess(),
                          (uint8_t) TOKEN_ADJUST_PRIVILEGES | (uint8_t) TOKEN_QUERY, &hToken))
        return false;
    // Get the LUID for the shutdown privilege.
    LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME,
                         &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, false, &tkp, 0,
                          (PTOKEN_PRIVILEGES) nullptr, nullptr);
    if (GetLastError() != ERROR_SUCCESS)
        return false;
    if (force && !ExitWindowsEx((uint8_t) EWX_REBOOT | (uint8_t) EWX_FORCE, 0)) {
        return false;
    } else if (!ExitWindowsEx(EWX_REBOOT, 0)) {
        return false;
    }
    return true;
}
}