// main file

#include "main.hpp"

constexpr auto FILE_NAME = L"main.cpp";

int main()
{
    LoggerInit();
    Logger& logger = Logger::GetInstance();
    logger.Log(LogLevel::INFO, L"main.cpp", L"Logger successfully initialized!");

    logger.Log(L"Press any key to exit the Specter");
    system("pause > nil");
    return 0;
}