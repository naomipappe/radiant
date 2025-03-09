#pragma once

#include <string_view>
#include <fmt/core.h>

namespace radiant
{
enum class LogLevel
{
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};
class Logger
{
  public:
    void log(std::string_view message) { fmt::print("{}", message); }

  private:
    LogLevel m_log_level{ LogLevel::Info };
    Logger(LogLevel log_level) : m_log_level(log_level) {}
};

} // namespace radiant