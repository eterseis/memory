#include "memory.h"

Memory::Memory(const wchar_t* proc) noexcept
{
	::PROCESSENTRY32 entry{};
	entry.dwSize = sizeof(::PROCESSENTRY32);

	const auto snap{ ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

	while (::Process32Next(snap, &entry)) {
		if (!wcscmp(proc, entry.szExeFile)) {
			m_procId = entry.th32ProcessID;
			break;
		}
	}

	m_hProcess = ::OpenProcess(PROCESS_VM_OPERATION
		| PROCESS_VM_WRITE
		| PROCESS_VM_READ, false, m_procId);

	::CloseHandle(snap);
}

const std::uintptr_t Memory::GetModuleAddress(const wchar_t* proc) const noexcept {
	::MODULEENTRY32 entry{};
	entry.dwSize = sizeof(::MODULEENTRY32);

	const auto snap{ ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_procId) };
	std::uintptr_t moduleBase{};

	while (Module32First(snap, &entry)) {
		if (!wcscmp(proc, entry.szModule)) {
			moduleBase = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
			break;
		}
	}
	::CloseHandle(snap);
	return moduleBase;
}