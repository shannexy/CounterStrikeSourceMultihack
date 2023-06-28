#include "memory.hpp"

#include "../core/console.hpp"
#include <Windows.h>

void memory::Setup() noexcept
{
    allocKeyValuesClient = PatternScan("client.dll", "FF 52 04 85 C0 74 0C 56") + 3;
    allocKeyValuesEngine = PatternScan("engine.dll", "FF 52 04 85 C0 74 0C 56") + 3;
    insertIntoTree = PatternScan("client.dll", "56 52 FF 50 18") + 5;

    keyValuesFromString = PatternScan("client.dll", "E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 04 8B F8 8B 11") + 1;
}

unsigned long memory::PatternSearch(std::string moduleName, std::string Pattern, std::string mask, unsigned long uCodeBase, unsigned long uSizeOfCode) noexcept
{
    bool bPatternDidMatch = false;
    HMODULE hModule = GetModuleHandle(moduleName.c_str());

    if (!hModule)
        return 0x0;

    PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER(hModule);
    PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS(LONG(hModule) + pDsHeader->e_lfanew);
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

    if (uCodeBase == 0x0)
        uCodeBase = (unsigned long)hModule + pOptionalHeader->BaseOfCode;

    if (uSizeOfCode == 0x0)
        uSizeOfCode = pOptionalHeader->SizeOfCode;

    unsigned long uArraySize = mask.length();

    if (!uCodeBase || !uSizeOfCode || !uArraySize)
        return 0x0;

    for (size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++)
    {
        for (size_t t = 0; t < uArraySize; t++)
        {
            if (*((PBYTE)i + t) == Pattern[t] || mask.c_str()[t] == '?')
                bPatternDidMatch = TRUE;

            else
            {
                bPatternDidMatch = FALSE;
                break;
            }
        }

        if (bPatternDidMatch)
            return i;
    }

    return 0x0;
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

std::uint8_t* memory::PatternScan(void* module, const char* signature) noexcept
{
    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dosHeader = (PIMAGE_DOS_HEADER)module;
    auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

    auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = pattern_to_byte(signature);
    auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

    auto s = patternBytes.size();
    auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i) {
        bool found = true;
        for (auto j = 0ul; j < s; ++j) {
            if (scanBytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found) {
            return &scanBytes[i];
        }
    }

    // Afterwards call server to stop dispatch of cheat and to alert us of update.
    console::print("A pattern has outtdated: ", signature);
    return nullptr;
}
