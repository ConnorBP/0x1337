#include "memory.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>

void memory::Setup() noexcept
{
    allocKeyValuesClient = PatternScan("client.dll", "FF 52 04 85 C0 74 0C 56") + 3;
    allocKeyValuesEngine = PatternScan("engine.dll", "FF 52 04 85 C0 74 0C 56") + 3;
    insertIntoTree = PatternScan("client.dll", "56 52 FF 50 18") + 5;

    keyValuesFromString = PatternScan("client.dll", "E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 04 8B F8 8B 11") + 1;

    glowManager = PatternScan("client.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3;

    /*
    return address check bypass
    https://www.unknowncheats.me/forum/counterstrike-global-offensive/500729-trouble-hooking-createmove.html
        "client.dll", "55 8B EC 83 E4 F8 83 EC 34 56 C7"
        "engine.dll", "55 8B EC 56 8B F1 33 C0 57"
        "studiorender.dll", "55 8B EC 56 8B F1 33"
        "materialsystem.dll", "55 8B EC 56 8B F1 33 C0"
        Code:
        bool __fastcall verify_return_address_hook(void* ecx, void* edx, const char* module_name)
        {
            return true;
        }
    */
    clientReturnCheck = PatternScan("client.dll", "55 8B EC 83 E4 F8 83 EC 34 56 C7");
    engineReturnCheck = PatternScan("engine.dll", "55 8B EC 56 8B F1 33 C0 57");
    studiorenderReturnCheck = PatternScan("studiorender.dll", "55 8B EC 56 8B F1 33");
    materialsystemReturnCheck = PatternScan("materialsystem.dll", "55 8B EC 56 8B F1 33 C0");

    clientGadgetAddress = PatternScan("client.dll", "FF 23 E8 ? ? ? ?");

}

std::uint8_t* memory::PatternScan(const char* moduleName, const char* pattern) noexcept
{
    static auto patternToByte = [](const char* pattern) noexcept -> const std::vector<std::int32_t>
    {
        std::vector<std::int32_t> bytes = std::vector<std::int32_t>{ };
        char* start = const_cast<char*>(pattern);
        const char* end = const_cast<char*>(pattern) + std::strlen(pattern);

        for (auto current = start; current < end; ++current)
        {
            if (*current == '?')
            {
                ++current;

                if (*current == '?')
                    ++current;

                bytes.push_back(-1);
            }
            else
                bytes.push_back(std::strtoul(current, &current, 16));
        }

        return bytes;
    };

    const HINSTANCE handle = GetModuleHandle(moduleName);

    if (!handle)
        return nullptr;

    const PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(handle);
    const PIMAGE_NT_HEADERS ntHeaders =
        reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(handle) + dosHeader->e_lfanew);

    const std::uintptr_t size = ntHeaders->OptionalHeader.SizeOfImage;
    const std::vector<std::int32_t> bytes = patternToByte(pattern);
    std::uint8_t* scanBytes = reinterpret_cast<std::uint8_t*>(handle);

    const std::size_t s = bytes.size();
    const std::int32_t* d = bytes.data();

    for (std::size_t i = 0ul; i < size - s; ++i)
    {
        bool found = true;

        for (std::size_t j = 0ul; j < s; ++j)
        {
            if (scanBytes[i + j] != d[j] && d[j] != -1)
            {
                found = false;
                break;
            }
        }

        if (found)
            return &scanBytes[i];
    }

    return nullptr;
}
