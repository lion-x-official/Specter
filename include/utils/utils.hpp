#pragma once
#include <cstdint>
#include <Windows.h>

#include "core/logger.hpp"
#include "core/ProcessHelper.hpp"
#include "utils/globals.hpp"

bool ProcessCheck()
{
	// Attempt to get a handle to the process
	// If the handle is not null, the process is running
	if (Globals::processHelper->Attach() && Globals::processHelper->Attach()) return true; // Process is running
	else return false; // Process not found
}

bool MainMenuCheck() {
	// Process Checks
	if (Globals::processHelper->GetProcessId() == 0) return false;
	if (Globals::processHelper->GetProcessHandle() == nullptr) return false;

	// Checking client_dll (loads in main menu)
	if (Globals::processHelper->GetModuleBase(Globals::processHelper->GetProcessId(), L"client.dll") == 0) {
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
	logger.Log(L"Press any key to exit the Specter...");
	system("pause > nul");
}

void LoggerInit(Logger& logger) {
	system("cls");
	std::this_thread::sleep_for(std::chrono::milliseconds(600));

	if (logger.IsLogFileExists("logs.txt")) {
		logger.Log(LogLevel::DEBUG, L"logger.cpp", 24, L"Log file exists -> logs.txt");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		logger.DeleteLogFile("logs.txt");
		logger.Log(LogLevel::DEBUG, L"logger.cpp", 32, L"Log file deleted -> -logs.txt");
		std::this_thread::sleep_for(std::chrono::milliseconds(1200));
	} else {
		logger.Log(LogLevel::DEBUG, L"logger.cpp", 26, L"Log file does not exist -> logs.txt");
	}

	logger.CreateLogFile("logs.txt");
	std::this_thread::sleep_for(std::chrono::milliseconds(60));
	logger.Log(LogLevel::DEBUG, L"logger.cpp", 36, L"Log file created successfully -> +logs.txt");

	logger.SetLogFile("logs.txt");

	logger.Log(LogLevel::INFO, L"logger.cpp", L"Logger successfully initialized!");
}
