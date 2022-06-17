#include "Logging.h"

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/494044-console-logging-loggingsystem.html
// https://gitlab.com/KittenPopo/csgo-2018-source/-/blob/main/public/tier0/logging.h#L143
auto log_console(std::string_view msg, Color color) -> void
{
    static constexpr int LS_MESSAGE = 0;

    // https://gitlab.com/KittenPopo/csgo-2018-source/-/blob/main/tier0/logging.cpp#L566
    using find_channel_fn_t = int(__cdecl*)(const char* name);
    static const auto s_channel_id = ((find_channel_fn_t)GetProcAddress(GetModuleHandleA("tier0.dll"), "LoggingSystem_FindChannel"))("Console");

    // https://gitlab.com/KittenPopo/csgo-2018-source/-/blob/main/tier0/logging.cpp#L675
    using log_direct_fn_t = int(__cdecl*)(int id, int severity, Color color, const char* msg);
    static const auto s_log_direct = (log_direct_fn_t)GetProcAddress(GetModuleHandleA("tier0.dll"), "LoggingSystem_LogDirect");

    s_log_direct(s_channel_id, LS_MESSAGE, color, msg.data());
}