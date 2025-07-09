#include "core/memory.hpp"
#include <stdexcept>

Memory::Memory(const std::wstring& processName) : processName_(processName), processHandle_(nullptr), processId_(0) {}

Memory::~Memory() {
    if (processHandle_) {
        CloseHandle(processHandle_);
    }
}

bool Memory::Attach() {
    PROCESSENTRY32W entry = { sizeof(PROCESSENTRY32W) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, processName_.c_str()) == 0) {
                processId_ = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);

    if (processId_ == 0) {
        return false;
    }

    processHandle_ = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId_);
    if (processHandle_ == INVALID_HANDLE_VALUE) {
        processHandle_ = nullptr; // Ensure it is set to nullptr for safe cleanup
        return false;
    }
    return processHandle_ != nullptr;
}

bool Memory::ReadMemory(uintptr_t address, void* buffer, size_t size) const {
    if (!processHandle_) {
        return false;
    }
    return ReadProcessMemory(processHandle_, reinterpret_cast<LPCVOID>(address), buffer, size, nullptr) != 0;
}

bool Memory::WriteMemory(uintptr_t address, const void* buffer, size_t size) const {
    if (!processHandle_) {
        return false;
    }
    return WriteProcessMemory(processHandle_, reinterpret_cast<LPVOID>(address), buffer, size, nullptr) != 0;
}

uintptr_t Memory::GetModuleBase(DWORD processId, const std::wstring& moduleName) const {
    MODULEENTRY32W entry = { sizeof(MODULEENTRY32W) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    uintptr_t baseAddress = 0;
    if (Module32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szModule, moduleName.c_str()) == 0) {
                baseAddress = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
                break;
            }
        } while (Module32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return baseAddress;
}

bool Memory::IsProcessRunning() const {
    if (!processHandle_) {
        return false;
    }
    DWORD exitCode;
    if (GetExitCodeProcess(processHandle_, &exitCode)) {
        return exitCode == STILL_ACTIVE;
    }
    return false;
}

std::vector<MODULEENTRY32W> Memory::GetProcessModules() const {
    std::vector<MODULEENTRY32W> modules;
    MODULEENTRY32W entry = { sizeof(MODULEENTRY32W) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId_);

    if (snapshot == INVALID_HANDLE_VALUE) {
        return modules;
    }

    if (Module32FirstW(snapshot, &entry)) {
        do {
            modules.push_back(entry);
        } while (Module32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return modules;
}