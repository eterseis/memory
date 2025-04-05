#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <wchar.h>

class Memory {
public:
	Memory(const wchar_t* proc) noexcept;
	~Memory() {
		::CloseHandle(m_hProcess);
	}
public:
	const std::uintptr_t GetModuleAddress(const wchar_t* proc) const noexcept;

	template<typename T>
	constexpr T Read(const std::uintptr_t& address) const noexcept {
		T buffer{};
		::ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), nullptr);
		return buffer;
	}

	template<typename T>
	constexpr void Write(const std::uintptr_t& address, T buffer) const noexcept {
		::WriteProcessMemory(m_hProcess, reinterpret_cast<LPVOID>(address), &buffer, sizeof(T), nullptr);
	}

private:
	std::uintptr_t m_procId{};
	HANDLE m_hProcess{};
};