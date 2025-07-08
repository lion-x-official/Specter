// main file

#include "main.hpp"

constexpr auto FILE_NAME = L"main.cpp";
constexpr auto LOG_FILE = L"logs.txt";
constexpr auto LOG_FORMAT = L"{timestamp} | {level} | {source} | {message}";
constexpr auto LOG_LEVEL = LogLevel::DEBUG; // Change to LogLevel::INFO for production

int main()
{
	Logger& logger = Logger::GetInstance();
	LoggerInit(logger);
	logger.SetMinLogLevel(LOG_LEVEL); // Updated to use LOG_LEVEL constant
	logger.Log(LogLevel::INFO, L"Minimum LogLevel: " + std::to_wstring(static_cast<int>(LOG_LEVEL)));

	logger.Log(LogLevel::INFO, FILE_NAME, L"Logger successfully initialized!");
	std::this_thread::sleep_for(std::chrono::milliseconds(600));
	logger.Log(LogLevel::INFO, FILE_NAME, L"Launching Specter!");
	std::this_thread::sleep_for(std::chrono::milliseconds(1200));
	system("cls");
	logger.Log(LogLevel::DEBUG, FILE_NAME, L"Console cleared");

	// Memory
	Memory memory = Memory(L"cs2.exe");
	logger.Log(LogLevel::DEBUG, FILE_NAME, L"Memory init for 'cs2.exe'");

	// Vars
	uint32_t processId;

	logger.Log(LogLevel::INFO, FILE_NAME, L"Checking for the process...");
	if (!ProcessCheck(logger, memory))
	{
		logger.Log(LogLevel::ERR, FILE_NAME, L"Process not found!");
		exitSpecter(logger);
		return EXIT_FAILURE; // Exit if the process is not found
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	processId = memory.GetProcessId();

	logger.Log(LogLevel::INFO, FILE_NAME, L"Process found! " + memory.GetProcessName() + L" -> " + std::to_wstring(processId));


	logger.Log(L"Press any key to exit the Specter");
	system("pause > nil");
	return 0;
}

bool ProcessCheck(Logger& logger, Memory& memory)
{
	logger.Log(LogLevel::INFO, L"Checking if the process is running...");
	uint8_t attempts = 0;
	uint8_t maxAttempts = 10; // Maximum attempts to check for the process
	while(attempts < maxAttempts)
	{
		// Attempt to get a handle to the process
		// If the handle is not null, the process is running
		if (memory.Attach() && memory.GetProcessId())
		{
			logger.Log(LogLevel::INFO,L"ProcessCheck", L"Process is running.");
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