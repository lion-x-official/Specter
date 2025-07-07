#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>

class Memory {
public:
    Memory(const std::wstring& processName);
    ~Memory();

    bool Attach();
    bool ReadMemory(uintptr_t address, void* buffer, size_t size) const;
    bool WriteMemory(uintptr_t address, const void* buffer, size_t size) const;
    uintptr_t GetModuleBase(const std::wstring& moduleName) const;
    DWORD GetProcessId() const { return processId_; }
    HANDLE GetProcessHandle() const { return processHandle_; }
    std::wstring GetProcessName() const { return processName_; }

private:
    HANDLE processHandle_;
    DWORD processId_;
    std::wstring processName_;
};