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

	// Arch check
	if (CheckSystemArchitecture() != 64)
	{
		logger.Log(LogLevel::ERR, FILE_NAME, L"Unsupported architecture! Exiting...");
		ExitSpecter(logger);
		return EXIT_FAILURE; // Exit if the architecture is not supported
	}
	else
	{
		logger.Log(LogLevel::INFO, FILE_NAME, L"Supported architecture detected.");
	}

	logger.Log(LogLevel::INFO, FILE_NAME, L"Launching Specter!");
	std::this_thread::sleep_for(std::chrono::milliseconds(1200));
	system("cls");
	logger.Log(LogLevel::DEBUG, FILE_NAME, L"Console cleared");

	// Memory
	ProcessHelper memory = ProcessHelper(L"cs2.exe");
	logger.Log(LogLevel::DEBUG, FILE_NAME, L"Memory init for 'cs2.exe'");


	// Vars
	uint32_t processId;


	logger.Log(LogLevel::INFO, FILE_NAME, L"Waiting for CS2...");
	if (!ProcessCheck(memory))
	{
		logger.Log(LogLevel::ERR, FILE_NAME, L"Process not found!");
		ExitSpecter(logger);
		return EXIT_FAILURE; // Exit if the process is not found
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	system("cls");
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	processId = memory.GetProcessId();
	logger.Log(LogLevel::DEBUG, FILE_NAME, L"Process found! " + memory.GetProcessName() + L" -> " + std::to_wstring(memory.GetProcessId()));
	logger.Log(LogLevel::INFO, FILE_NAME, memory.GetProcessName() + L" process found! ");
	std::this_thread::sleep_for(std::chrono::milliseconds(600));


	logger.Log(LogLevel::INFO, FILE_NAME, L"Checking for main menu...");
	while (!MainMenuCheck(memory))
	{
		logger.Log(LogLevel::DEBUG, L"MainMenuCheck", L"Searching for main menu");
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait for 1 second before checking again
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(700));
	logger.Log(LogLevel::INFO, L"MainMenuCheck", L"Main menu found!");

	logger.Log(LogLevel::INFO, L"Offset Updater", L"Updating offsets...");
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	while (!Offsets::UpdateOffsets(memory))
	{
		logger.Log(LogLevel::DEBUG, L"Offset Updater", L"Failed to update offsets! Retrying...");
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait for 1 second before retrying
	}
	logger.Log(LogLevel::INFO, L"Offset Updater", L"Offsets updated!");

	std::wstringstream ss;
	ss.imbue(std::locale::classic());
	ss << L"client.dll -> 0x" << std::hex << std::setw(16) << std::setfill(L'0') << Offsets::client_dll;
	logger.Log(LogLevel::DEBUG, L"Offset Updater", ss.str());
	ss.clear();


	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	ExitSpecter(logger);
	return 0;
}