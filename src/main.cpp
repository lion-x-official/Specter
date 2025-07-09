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
		ExitSpecter(logger);
		return EXIT_FAILURE; // Exit if the process is not found
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	processId = memory.GetProcessId();

	logger.Log(LogLevel::INFO, FILE_NAME, L"Process found! " + memory.GetProcessName() + L" -> " + std::to_wstring(processId));


	logger.Log(L"Press any key to exit the Specter");
	system("pause > NULL");
	return 0;
}