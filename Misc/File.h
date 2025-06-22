#pragma once

#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

class File
{
private:
    std::ifstream m_Stream;
    std::streamsize m_Size;

public:
    explicit File(const std::filesystem::path& path,
                  int stdOpenMode = std::ios_base::in | std::ios_base::out,
                  int stdOpenRule = std::ios_base::_Default_open_prot);

    bool isValid() const
    {
        return m_Stream.is_open();
    }

    std::string getString();
    std::vector<char> getBytes();
};
