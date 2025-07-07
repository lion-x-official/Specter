// main file

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