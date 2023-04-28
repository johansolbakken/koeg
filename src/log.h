#pragma once

#include <fmt/core.h>
#include <fmt/color.h>

#include <string>

// variable arguments
template <typename... Args>
inline void log(const std::string &scope, fmt::color color, const Args &...args)
{
    fmt::print(fmt::fg(color), "[{}] {}: ", __TIME__, scope);
    fmt::print(fmt::fg(color), args...);
    fmt::println("");
}

#define LOG_INFO(...) log("INFO", fmt::color::white, __VA_ARGS__)
#define LOG_WARN(...) log("WARN", fmt::color::yellow, __VA_ARGS__)
#define LOG_ERROR(...) log("ERROR", fmt::color::crimson, __VA_ARGS__)
#define LOG_FATAL(...) log("FATAL", fmt::color::red, __VA_ARGS__)
