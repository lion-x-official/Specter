#pragma once
#include <iostream>

#include "core/logger.hpp"
#include "core/memory.hpp"

#include <chrono>
#include <thread>
#include <cstdint>

void LoggerInit(Logger& logger) {
	system("cls");
    logger.SetLogFile("logs.txt");
    logger.SetLogFormat("{timestamp} | {level} | {source} | {message}");

    logger.Log(LogLevel::INFO, L"logger.cpp", L"Logger successfully initialized!");
    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    logger.DeleteLogFile("logs.txt");
    logger.Log(LogLevel::DEBUG, L"logger.cpp", 32, L"Log file deleted -> -logs.txt");
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));

    logger.CreateLogFile("logs.txt");
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    logger.Log(LogLevel::DEBUG, L"logger.cpp", 36, L"Log file created successfully -> +logs.txt");
}

bool ProcessCheck(Logger& logger, Memory& memory);

void exitSpecter(Logger& logger) {
    logger.Log(LogLevel::INFO, L"Exiting Specter...");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    system("cls");
    logger.Log(LogLevel::INFO, L"Specter exited successfully.");
}