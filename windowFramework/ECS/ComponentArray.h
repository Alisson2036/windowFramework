#pragma once
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
    void insert(Entity entity, Component component) 
    {
        if (entityToIndex.find(entity) == entityToIndex.end()) {
            entityToIndex[entity] = components.size();
            m_indexToEntity[components.size()] = entity;
            components.push_back(component);
        }
    }
    void remove(Entity entity) override 
    {
        // Remoção eficiente mantendo o vetor denso
        size_t index = entityToIndex[entity];
        size_t lastIndex = components.size() - 1;
        components[index] = components[lastIndex];
        Entity lastEntity = m_indexToEntity[lastIndex];
        entityToIndex[lastEntity] = index;
        m_indexToEntity[index] = lastEntity;
        components.pop_back();
        entityToIndex.erase(entity);
        m_indexToEntity.erase(lastIndex);
    }
    Component* get(Entity entity) {
        // MELHORAR ISSO AQUI
        return &components[entityToIndex[entity]];
    }
    Component* getByIndex(size_t index)
    {
        return &components[index];
    }
    bool containsEntity(Entity entity) override
    {
        return entityToIndex.find(entity) != entityToIndex.end();
    }
    std::vector<Component>& getArray()
    {
        return components;
    }
    size_t getArraySize() override
    {
        return components.size();
    }
    Entity indexToEntity(size_t index) override
    {
        auto it = m_indexToEntity.find(index);
        if (it == m_indexToEntity.end()) return 0;
        return m_indexToEntity[index];
    }
private:
    std::vector<Component> components;
    std::unordered_map<Entity, size_t> entityToIndex;
    std::unordered_map<size_t, Entity> m_indexToEntity;
};