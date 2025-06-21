#include "File.h"

#include "Misc/Debug.h"

#include <sstream>

File::File(const std::string& path, int stdOpenMode, int stdOpenRule)
{
    m_Stream.open(path, stdOpenMode, stdOpenRule);
    if (!m_Stream.is_open())
    {
        Debug::print("Can not open the file: `{}`", path);
    }
}

std::string File::getString() const
{
    if (!isValid())
        return "";

    std::stringstream ss;
    ss << m_Stream.rdbuf();

    return ss.str();
}
