#pragma once
#include <cstdint>
#include "logger.hpp"
#include "memory.hpp"


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

bool ProcessCheck(Logger& logger, Memory& memory)
{
	logger.Log(LogLevel::INFO, L"Checking if the process is running...");
	uint8_t attempts = 0;
	uint8_t maxAttempts = 10; // Maximum attempts to check for the process
	while (attempts < maxAttempts)
	{
		// Attempt to get a handle to the process
		// If the handle is not null, the process is running
		if (memory.Attach() && memory.GetProcessId())
		{
			logger.Log(LogLevel::INFO, L"ProcessCheck", L"Process is running.");
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
	if (!memory.Attach()) return false;
	if (memory.GetProcessId() == 0) return false;

	// Checking client_dll (loads in main menu)
	if (memory.GetModuleBase(memory.GetProcessId(), L"client.dll") == 0) {
		return false; // client.dll not found, likely not in main menu
	}

	return true;
}

void ExitSpecter(Logger& logger) {
	logger.Log(LogLevel::INFO, L"Exiting Specter...");
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	system("cls");
	logger.Log(LogLevel::INFO, L"Specter exited successfully.");
}