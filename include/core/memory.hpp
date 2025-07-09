#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>

class Memory {
public:
    Memory(const std::wstring& processName);
    ~Memory();

    bool Attach();
    bool ReadMemory(uintptr_t address, void* buffer, size_t size) const;
    bool WriteMemory(uintptr_t address, const void* buffer, size_t size) const;
    uintptr_t GetModuleBase(DWORD processId ,const std::wstring& moduleName) const;
    bool IsProcessRunning() const;
    DWORD GetProcessId() const { return processId_; }
    HANDLE GetProcessHandle() const { return processHandle_; }
    std::wstring GetProcessName() const { return processName_; }
    std::vector<MODULEENTRY32W> GetProcessModules() const;

private:
    HANDLE processHandle_;
    DWORD processId_;
    std::wstring processName_;
};