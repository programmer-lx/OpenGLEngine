#include "DelegateHandle.h"

std::uint64_t DelegateHandle::s_CurID = 0;

DelegateHandle DelegateHandle::getNextHandle()
{
    ++s_CurID;
    DelegateHandle ret;
    ret.m_ID = s_CurID;
    return ret;
}

bool DelegateHandle::operator==(const DelegateHandle& rhs) const
{
    return m_ID == rhs.m_ID;
}

bool DelegateHandle::operator<(const DelegateHandle& rhs) const
{
    return m_ID < rhs.m_ID;
}

bool DelegateHandle::operator>(const DelegateHandle& rhs) const
{
    return m_ID > rhs.m_ID;
}
