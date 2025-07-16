// main file
// Entry point for Specter application
// Handles initialization, process checks, logging, and main loop

#include "main.hpp"

// File and logging configuration
constexpr auto FILE_NAME = L"main.cpp";
constexpr auto LOG_FILE = L"logs.txt";
constexpr auto LOG_FORMAT = L"{timestamp} | {level} | {source} | {message}";
constexpr auto SPECTER_VERSION = "Specter - v0.0.8 Pre_Alpha";

int main()
{
    // Set console window title
    SetConsoleTitle(SPECTER_VERSION);

    // Initialize logger singleton and set minimum log level
    Logger& logger = Logger::GetInstance();
    logger.SetMinLogLevel(Globals::LOG_LEVEL); // Use global log level
    LoggerInit(logger);
    logger.Log(LogLevel::INFO, L"Minimum LogLevel: " + std::to_wstring(static_cast<int>(Globals::LOG_LEVEL)));

    // Log successful logger initialization
    logger.Log(LogLevel::INFO, FILE_NAME, L"Logger successfully initialized!");
    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    // Check system architecture (must be 64-bit)
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

    // Log launch and clear console
    logger.Log(LogLevel::INFO, FILE_NAME, L"Launching Specter!");
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    system("cls");
    logger.Log(LogLevel::DEBUG, FILE_NAME, L"Console cleared");

    // Initialize process helper for CS2
    ProcessHelper memory = ProcessHelper(L"cs2.exe");
    Globals::processHelper = &memory; // Set the global process helper pointer
    logger.Log(LogLevel::DEBUG, FILE_NAME, L"Memory init for 'cs2.exe'");

    // Vars
    uint32_t processId;

    // Wait for CS2 process to start
    logger.Log(LogLevel::INFO, FILE_NAME, L"Waiting for CS2...");
    while (!ProcessCheck())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Globals::cs2ProcessId = Globals::processHelper->GetProcessId();
    logger.Log(LogLevel::DEBUG, FILE_NAME, L"Process found! " + Globals::processHelper->GetProcessName() + L" -> " + std::to_wstring(Globals::processHelper->GetProcessId()));
    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    // Wait for main menu to appear in CS2
    logger.Log(LogLevel::DEBUG, FILE_NAME, L"Checking for main menu...");
    while (!MainMenuCheck())
    {
        logger.Log(LogLevel::DEBUG, L"MainMenuCheck", L"Searching for main menu");
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait for 0.5 second before checking again
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
    logger.Log(LogLevel::DEBUG, L"MainMenuCheck", L"Main menu found!");
    logger.Log(LogLevel::INFO, FILE_NAME, L"CS2 found!");
    std::this_thread::sleep_for(std::chrono::milliseconds(700));

    // Game initialization
    logger.Log(LogLevel::INFO, L"Game", L"Game initialization...");
    Game::Initialize(memory);

    // Log updated offsets for debugging
    std::wstringstream ss;
    ss.imbue(std::locale::classic());
    ss << L"client.dll -> 0x" << std::hex << std::setw(16) << std::setfill(L'0') << Offsets::client_dll;
    logger.Log(LogLevel::DEBUG, L"Game", ss.str());
    ss.clear();

    // Update game state
    Game::Update(memory);

    // Initialize entity list
    Entity::Initialize(memory);

    // Log important LocalPlayerPawn offsets
    logger.Log(LogLevel::DEBUG, L"Offsets", L"LocalPlayerPawn -> " + std::to_wstring(GameVars::LocalPlayerPawn::address));
    logger.Log(LogLevel::DEBUG, L"Offsets", L"LocalPlayerPawn Health -> " + std::to_wstring(GameVars::LocalPlayerPawn::health));
    logger.Log(LogLevel::DEBUG, L"Offsets", L"LocalPlayerPawn MaxHealth -> " + std::to_wstring(GameVars::LocalPlayerPawn::maxHealth));
    logger.Log(LogLevel::DEBUG, L"Offsets", L"LocalPlayerPawn TeamNum -> " + std::to_wstring(GameVars::LocalPlayerPawn::teamNum));
    logger.Log(LogLevel::DEBUG, L"Offsets", L"LocalPlayerPawn fFlags -> " + std::to_wstring(GameVars::LocalPlayerPawn::flags));

    // Game initialization complete
    logger.Log(LogLevel::INFO, L"Game", L"Game initialization successfully!");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    logger.Log(LogLevel::INFO, FILE_NAME, L"Loading main loop");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    system("cls");


    // Main loop: runs until F1 is pressed
    while (!GetAsyncKeyState(VK_F1))
    {
        // Check if CS2 process is still running
        if (Globals::processHelper->IsProcessRunning()) {
            if (Globals::processHelper->GetProcessId() != Globals::cs2ProcessId)
            {
                logger.Log(LogLevel::DEBUG, FILE_NAME, L"Process ID changed! Updating...");
                Globals::cs2ProcessId = Globals::processHelper->GetProcessId();
                logger.Log(LogLevel::DEBUG, FILE_NAME, L"New Process ID: " + std::to_wstring(Globals::cs2ProcessId));
                Globals::cs2Running = true;
            }
            Globals::cs2Running = true;
        }
        else
        {
            logger.Log(LogLevel::INFO, FILE_NAME, L"CS2 process not found! Exiting...");
            break; // Exit the loop if the process is not running
        }

        // Update game and entity list each loop
        Game::Update(memory);
        Entity::UpdateEntityList(memory);

        // Log entity positions and health
        for (const auto& entity : Entity::GetEntityList()) {
            std::lock_guard<std::mutex> lock(Globals::globalMutex); // Thread safety for entity access
            std::wstringstream entityPosStream;
            entityPosStream << L"Entity position: x" << entity.position[0]
                            << L", y" << entity.position[1]
                            << L", z" << entity.position[2];

            logger.Log(LogLevel::DEBUG, FILE_NAME, entityPosStream.str());
            logger.Log(LogLevel::DEBUG, FILE_NAME, L"Entity Health: " + std::to_wstring(entity.health));
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Small delay for logging
        }

        // Uncomment to log local player position
        /*std::wstringstream localPosStream;
        localPosStream << L"Local Player Position: x" << GameVars::LocalPlayerPawn::position[0]
                       << L", y" << GameVars::LocalPlayerPawn::position[1]
                       << L", z" << GameVars::LocalPlayerPawn::position[2];
        logger.Log(LogLevel::DEBUG, localPosStream.str());*/
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Main loop delay
    }

    // Exit sequence
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ExitSpecter(logger);
    return 0;
}
