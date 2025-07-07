// Specter.cpp : Defines the entry point for the application.
//

#include "Specter.h"
#include <memory.hpp>
#include <logger.hpp>
#include <sstream>

constexpr auto FILE_NAME = L"main.cpp";

int main()
{
    LoggerInit();
    Logger& logger = Logger::GetInstance();
    logger.Log(LogLevel::INFO, L"logger.cpp", L"Logger successfully initialized!");

    logger.Log(L"Press any key to exit the Specter");
    system("pause > nil");
    return 0;
}

void LoggerInit() {
    Logger& logger = Logger::GetInstance();
    logger.SetLogFile("logs.txt");
    logger.SetMinLogLevel(LogLevel::DEBUG);
    logger.SetLogFormat("{timestamp} | {level} | {source} | {message}");

    logger.Log(LogLevel::INFO, L"logger.cpp", L"Logger successfully initialized!");
    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    logger.DeleteLogFile("logs.txt");
    logger.Log(LogLevel::INFO, L"logger.cpp", 32, L"Log file deleted -> -logs.txt");
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));

    logger.CreateLogFile("logs.txt");
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    logger.Log(LogLevel::INFO, L"logger.cpp", 36, L"Log file created successfully -> +logs.txt");
}