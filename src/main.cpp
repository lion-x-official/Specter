// main file

#include "main.hpp"

constexpr auto FILE_NAME = L"main.cpp";
constexpr auto LOG_FILE = L"logs.txt";
constexpr auto LOG_FORMAT = L"{timestamp} | {level} | {source} | {message}";
constexpr auto LOG_LEVEL = LogLevel::INFO; // Change to LogLevel::INFO for production  Change to LogLevel::DEBUG for development

int main()
{
	SetConsoleTitle("Specter - v0.0.5 Pre_Alpha");
	Logger& logger = Logger::GetInstance();
	logger.SetMinLogLevel(LOG_LEVEL); // Updated to use LOG_LEVEL constant
	LoggerInit(logger);
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

	// checking for process
    logger.Log(LogLevel::INFO, FILE_NAME, L"Waiting for CS2...");
    while (!ProcessCheck(memory))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	processId = memory.GetProcessId();
	logger.Log(LogLevel::DEBUG, FILE_NAME, L"Process found! " + memory.GetProcessName() + L" -> " + std::to_wstring(memory.GetProcessId()));
	std::this_thread::sleep_for(std::chrono::milliseconds(600));


	logger.Log(LogLevel::DEBUG, FILE_NAME, L"Checking for main menu...");
	while (!MainMenuCheck(memory))
	{
		logger.Log(LogLevel::DEBUG, L"MainMenuCheck", L"Searching for main menu");
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait for 0.5 second before checking again
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(700));
	logger.Log(LogLevel::DEBUG, L"MainMenuCheck", L"Main menu found!");
	logger.Log(LogLevel::INFO, FILE_NAME, L"CS2 found!");
	std::this_thread::sleep_for(std::chrono::milliseconds(700));

	logger.Log(LogLevel::INFO, FILE_NAME, L"Waiting for full main menu.");
	std::this_thread::sleep_for(std::chrono::seconds(8));


	// Game Init
	logger.Log(LogLevel::INFO, L"Game", L"Game initialization...");


	// Log updated offsets
	std::wstringstream ss;
	ss.imbue(std::locale::classic());
	ss << L"client.dll -> 0x" << std::hex << std::setw(16) << std::setfill(L'0') << Offsets::client_dll;
	logger.Log(LogLevel::DEBUG, L"Game", ss.str());
	ss.clear();


	logger.Log(LogLevel::INFO, L"Game", L"Game initialization successfully!");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	logger.Log(LogLevel::INFO, FILE_NAME, L"Loading main loop");
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	system("cls");

	// Main loop
	while (true)
	{
		if (memory.IsProcessRunning()) {
			if (memory.GetProcessId() != processId)
			{
				logger.Log(LogLevel::DEBUG, FILE_NAME, L"Process ID changed! Updating...");
				processId = memory.GetProcessId();
				logger.Log(LogLevel::DEBUG, FILE_NAME, L"New Process ID: " + std::to_wstring(processId));
			}
		}
		else
		{
			logger.Log(LogLevel::INFO, FILE_NAME, L"CS2 process not found! Exiting...");
			break; // Exit the loop if the process is not running
		}

		Game::Update(memory);

		logger.Log(LogLevel::INFO, L"Game", L"Local Player Health: " + std::to_wstring(GameVars::LocalPlayerPawn::health));

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}


	// Exit
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	ExitSpecter(logger);
	return 0;
}
