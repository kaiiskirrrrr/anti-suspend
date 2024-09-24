#ifndef DLL_MAIN_CXX
#define DLL_MAIN_CXX

#include <aclapi.h>
#include <tlhelp32.h>
#include <winbase.h>
#include <windows.h>
#include <iostream>
#include <mutex>
#include <tlhelp32.h>
#include "tools/lazy.hxx"

class anti_suspension
{
public:

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
        while (true)
        {
            if (process_name == process_entry.szExeFile)
            {
                process_id = process_entry.th32ProcessID;
                break;
            }
            if (!LI_FN(Process32NextW).forwarded_safe_cached()(snapshot_handle, &process_entry))
            {
                break;
            }
        }

        LI_FN(CloseHandle).forwarded_safe_cached()(snapshot_handle);
        return process_id;
    }

    auto is_process_running(const std::wstring& process_name) const -> bool
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

    auto resume_threads_of_process(const DWORD& process_id) -> void
    {
        HANDLE snapshot_handle = LI_FN(CreateToolhelp32Snapshot).forwarded_safe_cached()(TH32CS_SNAPTHREAD, 0);
        if (snapshot_handle == INVALID_HANDLE_VALUE)
        {
            return;
        }

        THREADENTRY32 thread_entry;
        thread_entry.dwSize = sizeof(thread_entry);
        if (LI_FN(Thread32First).forwarded_safe_cached()(snapshot_handle, &thread_entry))
        {
            while (true)
            {
                if (thread_entry.th32OwnerProcessID == process_id)
                {
                    HANDLE thread_handle = LI_FN(OpenThread).forwarded_safe_cached()(THREAD_SUSPEND_RESUME, FALSE, thread_entry.th32ThreadID);
                    if (thread_handle)
                    {
                        LI_FN(ResumeThread).forwarded_safe_cached()(thread_handle);
                        LI_FN(CloseHandle).forwarded_safe_cached()(thread_handle);
                    }
                }

                if (!LI_FN(Thread32Next).forwarded_safe_cached()(snapshot_handle, &thread_entry))
                {
                    break;
                }
            }
        }

        LI_FN(CloseHandle).forwarded_safe_cached()(snapshot_handle);
    }
};
inline const auto c_anti_suspension = std::make_unique<anti_suspension>();

extern "C" __declspec(dllexport) void start()
{
    const DWORD process_id = c_anti_suspension->get_process_id_from_process_name(L"start.exe");
    if (process_id == 0)
    {
        return;
    }

    while (c_anti_suspension->is_process_running(L"start.exe")) 
    {
        c_anti_suspension->resume_threads_of_process(process_id);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    static std::once_flag flag;
    switch (reason)
    {
    case DLL_PROCESS_ATTACH: { std::call_once(flag, [] { std::thread(start).detach(); }); } break;
    case DLL_THREAD_DETACH: break;
    case DLL_THREAD_ATTACH: break;
    case DLL_PROCESS_DETACH: break;
    }

    return 0;
}

#endif
