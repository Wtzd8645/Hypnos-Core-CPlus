#pragma once

#include <cstdarg>
#include <cstdio>

namespace Blanketmen {

constexpr const char* CONSOLE_LOG_STYLE_RESET          = "\033[0;0m\n";
constexpr const char* CONSOLE_LOG_STYLE_REGULAR_NORMAL = "\033[0;0m";
constexpr const char* CONSOLE_LOG_STYLE_REGULAR_BLACK  = "\033[0;30m";
constexpr const char* CONSOLE_LOG_STYLE_REGULAR_RED    = "\033[0;31m";
constexpr const char* CONSOLE_LOG_STYLE_REGULAR_GREEN  = "\033[0;32m";
constexpr const char* CONSOLE_LOG_STYLE_REGULAR_YELLOW = "\033[0;33m";
constexpr const char* CONSOLE_LOG_STYLE_REGULAR_BLUE   = "\033[0;34m";
constexpr const char* CONSOLE_LOG_STYLE_REGULAR_PURPLE = "\033[0;35m";
constexpr const char* CONSOLE_LOG_STYLE_REGULAR_CYAN   = "\033[0;36m";
constexpr const char* CONSOLE_LOG_STYLE_REGULAR_WHITE  = "\033[0;37m";

struct ILogger
{
    virtual ~ILogger() = default;
    virtual void Info(const char* format, va_list args) = 0;
    virtual void Warning(const char* format, va_list args) = 0;
    virtual void Error(const char* format, va_list args) = 0;
};

struct StdoutLogger : public ILogger
{
    inline void Info(const char* format, va_list args) override
    {
        std::vfprintf(stdout, format, args);
        std::fprintf(stdout, "\n");
    }

    inline void Warning(const char* format, va_list args) override
    {
        std::fprintf(stdout, CONSOLE_LOG_STYLE_REGULAR_YELLOW);
        std::vfprintf(stdout, format, args);
        std::fprintf(stdout, CONSOLE_LOG_STYLE_RESET);
    }

    inline void Error(const char* format, va_list args) override
    {
        std::fprintf(stderr, CONSOLE_LOG_STYLE_REGULAR_RED);
        std::vfprintf(stderr, format, args);
        std::fprintf(stderr, CONSOLE_LOG_STYLE_RESET);
    }
};

class Logging
{
public:
    inline static void Configure(ILogger* logger)
    {
        Logging::logger = logger != nullptr ? logger : &stdoutLogger;
    }

    inline static void Info(const char* const format, ...) noexcept
    {
        va_list args { };
        va_start(args, format);
        logger->Info(format, args);
        va_end(args);
    }

    inline static void Warning(const char* const format, ...) noexcept
    {
        va_list args { };
        va_start(args, format);
        logger->Warning(format, args);
        va_end(args);
    }

    inline static void Error(const char* const format, ...) noexcept
    {
        va_list args { };
        va_start(args, format);
        logger->Error(format, args);
        va_end(args);
    }

private:
    inline static StdoutLogger stdoutLogger;
    inline static ILogger* logger = &stdoutLogger;
};

} // namespace Blanketmen