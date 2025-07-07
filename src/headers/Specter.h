#pragma once
#include <iostream>

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