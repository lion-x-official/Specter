#pragma once
#include <cstdint>
#include <Windows.h>

#include "logger.hpp"
#include "memory.hpp"

bool ProcessCheck(Logger& logger, Memory& memory)
{
	uint8_t attempts = 0;
	uint8_t maxAttempts = 10; // Maximum attempts to check for the process
	while (attempts < maxAttempts)
	{
		// Attempt to get a handle to the process
		// If the handle is not null, the process is running
		if (memory.Attach() && memory.GetProcessId())
		{
			logger.Log(LogLevel::DEBUG, L"ProcessCheck", L"Process ID: " + std::to_wstring(memory.GetProcessId()));
			return true; // Process is running
		}
		else
		{
			attempts++;
			logger.Log(LogLevel::INFO, L"Process not found, checking again... " + std::to_wstring(attempts) + L"/" + std::to_wstring(maxAttempts));
			std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait for 1 second before checking again
		}
	}
	logger.Log(LogLevel::ERR, L"Process not found after maximum attempts!");
	return false; // Process not found after maximum attempts
}

bool MainMenuCheck(Memory& memory) {
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
		return 64; // 64-bit
	}
	else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
		return 32; // 32-bit unsupported
	}
	else {
		return 11; // Unsupported
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