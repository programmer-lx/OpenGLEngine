#pragma once

#include <memory>
#include <string>

#define FCOMPONENT(className) \
    public: \
    static std::string staticComponentClassName() { return #className; } \
    virtual std::string componentClassName() { return #className; } \
    private:

#define FCOMPONENT_DECLARE_CREATE_FUNC_PARAM0(className) \
    public: \
    static std::shared_ptr<className> create(); \
    private:

#define FCOMPONENT_DECLARE_CREATE_FUNC_PARAM1(className, type1, param1) \
    public: \
    static std::shared_ptr<className> create(type1 param1); \
    private:

#define FCOMPONENT_DECLARE_CREATE_FUNC_PARAM2(className, type1, param1, type2, param2) \
    public: \
    static std::shared_ptr<className> create(type1 param1, type2 param2); \
    private:

#define FCOMPONENT_DECLARE_CREATE_FUNC_PARAM3(className, type1, param1, type2, param2, type3, param3) \
    public: \
    static std::shared_ptr<className> create(type1 param1, type2 param2, type3 param3); \
    private:


class Transform;

class ComponentBase
{
    friend class GameObject;

    FCOMPONENT(ComponentBase)

private:
    static std::uint64_t s_ID;

    bool m_Enabled = true;

    std::uint64_t m_ID;
    GameObject* m_OwnerGameObject = nullptr;

    void setOwner(GameObject* ownerGameObject);

public:
    ComponentBase();

    virtual ~ComponentBase() = default;

    bool operator==(const ComponentBase& rhs) const;

    bool operator<(const ComponentBase& rhs) const;

    bool operator>(const ComponentBase& rhs) const;

public:
    virtual void awake()
    {
    }

    virtual void fixedTick()
    {
    }

    virtual void tick()
    {
    }

    virtual void onEnable()
    {
    }

    virtual void onDisable()
    {
    }

    virtual void onDestroy()
    {
    }

    virtual void onInspectorGUI();

public:
    [[nodiscard]]
    GameObject* gameObject() const
    {
        return m_OwnerGameObject;
    }

    [[nodiscard]]
    Transform& transform();

    [[nodiscard]]
    const Transform& transform() const;

    [[nodiscard]]
    std::uint64_t componentID() const
    {
        return m_ID;
    }

    bool isEnabled() const;

    void setEnable(bool enable);
};
