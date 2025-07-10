#pragma once
#include <cstdint>
#include <Windows.h>

#include "logger.hpp"
#include "ProcessHelper.hpp"

bool ProcessCheck(ProcessHelper& memory)
{
	// Attempt to get a handle to the process
	// If the handle is not null, the process is running
	if (memory.Attach() && memory.GetProcessId()) return true; // Process is running
	else return false; // Process not found
}

bool MainMenuCheck(ProcessHelper& memory) {
	// Process Checks
	if (memory.GetProcessId() == 0) return false;
	if (memory.GetProcessHandle() == nullptr) return false;

	// Checking client_dll (loads in main menu)
	if (memory.GetModuleBase(memory.GetProcessId(), L"client.dll") == 0) {
		return false; // client.dll not found, likely not in main menu
	}

	return true;
}

// Architecture check
uint8_t CheckSystemArchitecture() {
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);

	if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
		return 64; // 64-bit supported arch
	}
	else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
		return 32; // 32-bit unsupported arch
	}
	else {
		return 11; // Unsupported arch
	}
}

void ExitSpecter(Logger& logger) {
	logger.Log(LogLevel::INFO, L"Exiting Specter...");
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	logger.Log(L"Press any key to exit the Specter");
	system("pause > nul");
}

void LoggerInit(Logger& logger) {
	system("cls");
	logger.SetLogFile("logs.txt");

	logger.Log(LogLevel::INFO, L"logger.cpp", L"Logger successfully initialized!");
	std::this_thread::sleep_for(std::chrono::milliseconds(600));

	logger.DeleteLogFile("logs.txt");
	logger.Log(LogLevel::DEBUG, L"logger.cpp", 32, L"Log file deleted -> -logs.txt");
	std::this_thread::sleep_for(std::chrono::milliseconds(1200));

	logger.CreateLogFile("logs.txt");
	std::this_thread::sleep_for(std::chrono::milliseconds(60));
	logger.Log(LogLevel::DEBUG, L"logger.cpp", 36, L"Log file created successfully -> +logs.txt");
}