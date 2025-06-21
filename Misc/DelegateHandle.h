#pragma once

#include <cinttypes>

class DelegateHandle
{
private:
    DelegateHandle() = default;

private:
    static std::uint64_t s_CurID;
    std::uint64_t m_ID = 0;

public:
    static DelegateHandle getNextHandle();

    bool operator==(const DelegateHandle& rhs) const;
    bool operator<(const DelegateHandle& rhs) const;
    bool operator>(const DelegateHandle& rhs) const;
};
