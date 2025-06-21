#pragma once

template <typename T>
class ManagerBase
{
public:
    ManagerBase() = default;

public:
    ManagerBase(const ManagerBase&) = delete;
    ManagerBase(ManagerBase&&) = delete;

    virtual ~ManagerBase() = default;

    static T& instance()
    {
        static T s;
        return s;
    }
};
