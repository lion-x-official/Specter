#include "core/logger.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <cstdio>
#include <locale>
#include <codecvt>

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger()
    : consoleOutput_(true)
    , minLogLevel_(LogLevel::DEBUG)
    , logFormat_(L"[{timestamp}] {source}[{level}] {message}")
    , running_(true)
{
    std::locale::global(std::locale(""));
    logThread_ = std::thread(&Logger::ProcessLogQueue, this);
}

Logger::~Logger() {
    Shutdown();
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::SetLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex_);
    if (logFile_.is_open()) {
        logFile_.close();
    }
    logFile_.open(filename, std::ios::app);
    logFile_.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
    if (!logFile_.is_open()) {
        std::wcerr << L"Failed to open log file: " << std::wstring(filename.begin(), filename.end()) << std::endl;
    }
}

void Logger::CreateLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex_);
    if (logFile_.is_open()) {
        logFile_.close();
    }
    logFile_.open(filename, std::ios::out | std::ios::trunc);
    logFile_.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
    if (!logFile_.is_open()) {
        std::wcerr << L"Failed to create log file: " << std::wstring(filename.begin(), filename.end()) << std::endl;
    }
}

bool Logger::DeleteLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex_);
    if (logFile_.is_open()) {
        logFile_.close();
    }
    if (std::remove(filename.c_str()) == 0) {
        return true;
    } else {
        std::wcerr << L"Failed to delete log file: " << std::wstring(filename.begin(), filename.end()) << std::endl;
        return false;
    }
}

void Logger::SetMinLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex_);
    minLogLevel_ = level;
}

void Logger::SetConsoleOutput(bool enabled) {
    std::lock_guard<std::mutex> lock(logMutex_);
    consoleOutput_ = enabled;
}

void Logger::SetLogFormat(const std::string& format) {
    std::lock_guard<std::mutex> lock(logMutex_);
    logFormat_ = std::wstring(format.begin(), format.end());
}

void Logger::Log(LogLevel level, const std::wstring& message) {
    if (level < minLogLevel_) return;

    LogEntry entry = { level, message, L"", 0, GetTimestamp() };
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        logQueue_.push(entry);
    }
    queueCondVar_.notify_one();
}

void Logger::Log(LogLevel level, const std::wstring& file, const std::wstring& message) {
    if (level < minLogLevel_) return;

    LogEntry entry = { level, message, file, 0, GetTimestamp() };
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        logQueue_.push(entry);
    }
    queueCondVar_.notify_one();
}

void Logger::Log(LogLevel level, const std::wstring& file, int line, const std::wstring& message) {
    if (level < minLogLevel_) return;

    LogEntry entry = { level, message, file, line, GetTimestamp() };
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        logQueue_.push(entry);
    }
    queueCondVar_.notify_one();
}

void Logger::Log(const std::wstring& message) {
    Log(LogLevel::INFO, message);
}

void Logger::Shutdown() {
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        running_ = false;
    }
    queueCondVar_.notify_all();
    if (logThread_.joinable()) {
        logThread_.join();
    }
}

void Logger::ProcessLogQueue() {
    while (running_) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        queueCondVar_.wait(lock, [this] { return !logQueue_.empty() || !running_; });

        while (!logQueue_.empty()) {
            LogEntry entry = logQueue_.front();
            logQueue_.pop();
            lock.unlock();

            std::wstring logMessage = FormatMessage(entry);

            std::lock_guard<std::mutex> logLock(logMutex_);
            if (consoleOutput_) {
                std::wstring colorCode;
                switch (entry.level) {
                case LogLevel::DEBUG:   colorCode = L"\033[32m"; break; // Green
                case LogLevel::INFO:    colorCode = L"\033[34m"; break; // Blue
                case LogLevel::WARNING: colorCode = L"\033[33m"; break; // Yellow
                case LogLevel::ERR:     colorCode = L"\033[31m"; break; // Red
                default:                colorCode = L"\033[0m";  break; // Reset
                }
                std::wstring timestampColor = L"\033[37m"; // White
                std::wstring fileColor = L"\033[36m";      // Cyan
                std::wstring resetCode = L"\033[0m";

                std::wstring consoleMessage;
                if (!entry.file.empty() && entry.line > 0) {
                    consoleMessage = timestampColor + L"[" + entry.timestamp + L"]" + resetCode + L" " +
                        fileColor + entry.file + L":" + std::to_wstring(entry.line) + resetCode + L" " +
                        colorCode + L"[" + LogLevelToString(entry.level) + L"] " + entry.message + resetCode;
                }
                else if (!entry.file.empty()) {
                    consoleMessage = timestampColor + L"[" + entry.timestamp + L"]" + resetCode + L" " +
                        fileColor + entry.file + resetCode + L" " +
                        colorCode + L"[" + LogLevelToString(entry.level) + L"] " + entry.message + resetCode;
                }
                else {
                    consoleMessage = timestampColor + L"[" + entry.timestamp + L"]" + resetCode + L" " +
                        colorCode + L"[" + LogLevelToString(entry.level) + L"] " + entry.message + resetCode;
                }

                if (entry.level == LogLevel::ERR) {
                    std::wcerr << consoleMessage << std::endl;
                }
                else {
                    std::wcout << consoleMessage << std::endl;
                }
            }

            if (logFile_.is_open()) {
                logFile_ << logMessage << std::endl;
                logFile_.flush();
            }

            lock.lock();
        }
    }
}

std::wstring Logger::FormatMessage(const LogEntry& entry) const {
    std::wstring result = logFormat_;
    std::wstring source = entry.file.empty() ? L"" : (entry.line > 0 ? entry.file + L":" + std::to_wstring(entry.line) : entry.file);

    size_t pos;
    while ((pos = result.find(L"{timestamp}")) != std::wstring::npos) {
        result.replace(pos, 11, entry.timestamp);
    }
    while ((pos = result.find(L"{level}")) != std::wstring::npos) {
        result.replace(pos, 7, LogLevelToString(entry.level));
    }
    while ((pos = result.find(L"{source}")) != std::wstring::npos) {
        result.replace(pos, 8, source);
    }
    while ((pos = result.find(L"{message}")) != std::wstring::npos) {
        result.replace(pos, 9, entry.message);
    }
    return result;
}

std::wstring Logger::GetTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::wstringstream ss;
    ss << std::put_time(std::localtime(&time), L"%Y-%m-%d %H:%M:%S") << L"." << std::setfill(L'0') << std::setw(3) << ms.count();
    return ss.str();
}

std::wstring Logger::LogLevelToString(LogLevel level) const {
    switch (level) {
    case LogLevel::DEBUG: return L"DEBUG";
    case LogLevel::INFO: return L"INFO";
    case LogLevel::WARNING: return L"WARNING";
    case LogLevel::ERR: return L"ERROR";
    default: return L"UNKNOWN";
    }
}
