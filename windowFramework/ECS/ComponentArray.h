#pragma once
#include <limits>
#include "..\Utils\PagedSparseSet.h"



class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void remove(Entity entity) = 0;
    virtual size_t getArraySize() = 0;
    virtual bool containsEntity(Entity entity) = 0;
    virtual Entity indexToEntity(size_t index) = 0;
};

template<typename Component>
class ComponentArray : public IComponentArray {
public:
    ComponentArray()
        :
        m_entityToIndex((size_t)(std::numeric_limits<size_t>::max)())
    {}
    void insert(Entity entity, Component component) 
    {
        if (!containsEntity(entity)) {
            m_entityToIndex[entity] = m_components.size();
            m_indexToEntity.push_back(entity);
            m_components.push_back(component);
        }
    }
    void remove(Entity entity) override 
    {
        // Swap and pop
        size_t index = m_entityToIndex[entity];
        size_t lastIndex = m_components.size() - 1;
        Entity lastEntity = m_indexToEntity[lastIndex];
        m_components[index] = m_components[lastIndex];
		m_indexToEntity[index] = lastEntity;
        m_entityToIndex[lastEntity] = index;
        m_components.pop_back();
        m_indexToEntity.pop_back();
        m_entityToIndex[entity] = (std::numeric_limits<size_t>::max)();
    }
    Component* get(Entity entity) {
        // MELHORAR ISSO AQUI
        return &m_components[m_entityToIndex[entity]];
    }
    Component* getByIndex(size_t index)
    {
        return &m_components[index];
    }
    bool containsEntity(Entity entity) override
    {
        auto* index = m_entityToIndex.get(entity);
        return index != nullptr && (*index) != (std::numeric_limits<size_t>::max)();
    }
    std::vector<Component>& getArray()
    {
        return m_components;
    }
    size_t getArraySize() override
    {
        return m_components.size();
    }
    Entity indexToEntity(size_t index) override
    {
        if (index >= m_indexToEntity.size()) return 0;
        return m_indexToEntity[index];
    }
private:
    std::vector<Component> m_components;
    PagedSparseSet<size_t> m_entityToIndex;
    std::vector<Entity> m_indexToEntity;
};