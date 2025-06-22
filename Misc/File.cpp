#include "File.h"

#include "Misc/Debug.h"

#include <sstream>

File::File(const std::filesystem::path& path, int stdOpenMode, int stdOpenRule)
{
    m_Stream.open(path, stdOpenMode, stdOpenRule);
    if (!m_Stream.is_open())
    {
        Debug::print("Can not open the file: `{}`", path.string());
    }

    // get byte count
    m_Stream.seekg(0, std::ios::end);
    m_Size = m_Stream.tellg();
    m_Stream.seekg(0, std::ios::beg);
}

std::string File::getString()
{
    if (!isValid())
        return "";

    std::stringstream ss;
    ss << m_Stream.rdbuf();

    return ss.str();
}

std::vector<char> File::getBytes()
{
    if (!isValid())
        return {};

    std::vector<char> buffer(m_Size);
    if (!m_Stream.read(buffer.data(), m_Size))
    {
        return {};
    }

    return buffer;
}
