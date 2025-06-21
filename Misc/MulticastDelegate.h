#pragma once

#include "DelegateHandle.h"

#include <functional>
#include <map>

template <typename... Args>
class MulticastDelegate
{
private:
    std::map<DelegateHandle, std::function<void(Args...)>> m_Funcs;

public:
    DelegateHandle add(const std::function<void(Args...)>& func)
    {
        DelegateHandle handle = DelegateHandle::getNextHandle();
        m_Funcs.insert({handle, func});
        return handle;
    }

    bool remove(const DelegateHandle& handle)
    {
        auto iter = m_Funcs.find(handle);
        if (iter == m_Funcs.end())
            return false;

        m_Funcs.erase(iter);
        return true;
    }

    void invoke(const Args&... args)
    {
        for (auto& func : m_Funcs)
        {
            func.second(args...);
        }
    }

    [[nodiscard]]
    std::int32_t count() const
    {
        return m_Funcs.size();
    }
};
