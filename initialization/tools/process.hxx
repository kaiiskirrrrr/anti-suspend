#ifndef PROCESS_HXX
#define PROCESS_HXX

#include "includes.hxx"

class process
{
public:

    auto is_process_running(const std::wstring& process_name) -> bool
    {
        HANDLE snapshot_handle = LI_FN(CreateToolhelp32Snapshot).forwarded_safe_cached()(TH32CS_SNAPPROCESS, 0);
        if (snapshot_handle == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        PROCESSENTRY32W process_entry;
        process_entry.dwSize = sizeof(PROCESSENTRY32W);

        if (!LI_FN(Process32FirstW).forwarded_safe_cached()(snapshot_handle, &process_entry))
        {
            LI_FN(CloseHandle).forwarded_safe_cached()(snapshot_handle);
            return false;
        }

        while (LI_FN(Process32NextW).forwarded_safe_cached()(snapshot_handle, &process_entry))
        {
            if (process_name == process_entry.szExeFile)
            {
                LI_FN(CloseHandle).forwarded_safe_cached()(snapshot_handle);
                return true;
            }
        }

        LI_FN(CloseHandle).forwarded_safe_cached()(snapshot_handle);
        return false;
    }

    auto get_process_id_from_process_name(const std::wstring& process_name) const -> DWORD
    {
        HANDLE snapshot_handle = LI_FN(CreateToolhelp32Snapshot).forwarded_safe_cached()(TH32CS_SNAPPROCESS, 0);
        if (snapshot_handle == INVALID_HANDLE_VALUE)
        {
            return 0;
        }

        PROCESSENTRY32W process_entry;
        process_entry.dwSize = sizeof(PROCESSENTRY32W);

        if (!LI_FN(Process32FirstW).forwarded_safe_cached()(snapshot_handle, &process_entry))
        {
            LI_FN(CloseHandle).forwarded_safe_cached()(snapshot_handle);
            return 0;
        }

        DWORD process_id = 0;
        do
        {
            if (process_name == process_entry.szExeFile)
            {
                process_id = process_entry.th32ProcessID;
                break;
            }
        } while (LI_FN(Process32NextW).forwarded_safe_cached()(snapshot_handle, &process_entry));

        LI_FN(CloseHandle).forwarded_safe_cached()(snapshot_handle);
        return process_id;
    }

    auto get_pid_from_name(const WCHAR* process_name) const -> DWORD
    {
        HANDLE snapshot_handle = LI_FN(CreateToolhelp32Snapshot).forwarded_safe_cached()(TH32CS_SNAPPROCESS, 0);
        DWORD pid = 0;
        if (snapshot_handle != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32W process_entry;
            process_entry.dwSize = sizeof(PROCESSENTRY32W);
            if (LI_FN(Process32FirstW).forwarded_safe_cached()(snapshot_handle, &process_entry))
            {
                do
                {
                    if (!LI_FN(wcscmp).forwarded_safe_cached()(process_name, process_entry.szExeFile))
                    {
                        pid = process_entry.th32ProcessID;
                        break;
                    }
                } while (LI_FN(Process32NextW).forwarded_safe_cached()(snapshot_handle, &process_entry));
            }
            LI_FN(CloseHandle).forwarded_safe_cached()(snapshot_handle);
        }
        return pid;
    }

};
inline const auto c_process = std::make_unique<process>();

#endif // !PROCESS_HXX
