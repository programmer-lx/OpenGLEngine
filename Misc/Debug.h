#pragma once

#include <cinttypes>
#include <iostream>
#include <format>

class Debug
{
public:
    template<typename... Args>
    static void print(std::format_string<Args...> fmt, Args&&... args)
    {
#if FLX_DEBUG_BUILD
        std::cout << std::format(fmt, std::forward<Args>(args)...) << std::endl;
#endif
    }

    static void printShaderError(std::uint32_t shaderID, std::uint32_t shaderType, const char* filePath);
};
