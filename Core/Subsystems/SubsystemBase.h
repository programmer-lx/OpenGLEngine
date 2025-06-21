#pragma once

class AbstractSubsystem
{
public:
    virtual void tick() = 0;
    virtual void onDestroy() {}
    virtual ~AbstractSubsystem() = default;
};

template<typename T>
class SubsystemBase : public AbstractSubsystem
{
private:
    static T* s_Instance;

public:
    SubsystemBase()
    {
        s_Instance = (T*)this;
    }

    virtual ~SubsystemBase() override = default;

    static T* instance()
    {
        return s_Instance;
    }
};

template<typename T>
T* SubsystemBase<T>::s_Instance = nullptr;