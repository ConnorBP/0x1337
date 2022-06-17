#pragma once

#include <Windows.h>
#include <string>

#include "../valve/Color.hpp"

auto log_console(std::string_view msg, Color color) -> void;
