#pragma once

#include <fstream>
#include <string>

class File
{
private:
    std::fstream m_Stream;

public:
    explicit File(const std::string& path,
                  int stdOpenMode = std::ios_base::in | std::ios_base::out,
                  int stdOpenRule = std::ios_base::_Default_open_prot);

    bool isValid() const
    {
        return m_Stream.is_open();
    }

    std::string getString() const;
};
