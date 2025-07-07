#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

#include <codecvt> // for codecvt_utf8
#include <locale>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERR
};

class Logger {
public:
    static Logger& GetInstance();
    void SetLogFile(const std::string& filename);
    void SetMinLogLevel(LogLevel level);
    void SetConsoleOutput(bool enabled);
    void SetLogFormat(const std::string& format);
    void Log(LogLevel level, const std::wstring& message);
    void Log(LogLevel level, const std::wstring& file, const std::wstring& message);
    void Log(LogLevel level, const std::wstring& file, int line, const std::wstring& message);
    void Log(const std::wstring& message);
    void Shutdown();

    // Create a new log file (overwrite if exists)
    void CreateLogFile(const std::string& filename);
    // Delete the current log file (if open) or by filename
    bool DeleteLogFile(const std::string& filename);

private:
    Logger();
    ~Logger();

    struct LogEntry {
        LogLevel level;
        std::wstring message;
        std::wstring file;
        int line;
        std::wstring timestamp;
    };

    std::wofstream logFile_;
    std::mutex logMutex_;
    std::mutex queueMutex_;
    std::queue<LogEntry> logQueue_;
    std::condition_variable queueCondVar_;
    std::atomic<bool> running_;
    std::thread logThread_;
    bool consoleOutput_;
    LogLevel minLogLevel_;
    std::wstring logFormat_;

    std::wstring GetTimestamp() const;
    std::wstring LogLevelToString(LogLevel level) const;
    std::wstring FormatMessage(const LogEntry& entry) const;
    void ProcessLogQueue();
};

// Convenience macro for logging with file and line number
#define LOG(level, message) Logger::GetInstance().Log(level, __FILEW__, __LINE__, message)