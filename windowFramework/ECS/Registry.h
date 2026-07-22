#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include <algorithm>

#include "EntityManager.h"
#include "ComponentArray.h"

template<typename... Components>
class View
{
public:
	View(ComponentArray<Components>*... arrays)
		: pArrays(arrays...)
	{
		// Validates array types
		m_isValid = (... && (arrays != nullptr));

		if (m_isValid)
		{
			// Calculates lead_array.
			// lead_array is always the compArray with less entities.
			std::array<size_t, sizeof...(Components)> sizes = { arrays->getArraySize()... };
			m_mainComponentIdx = std::distance(sizes.begin(), std::min_element(sizes.begin(), sizes.end()));
		}
	}

	// Internal iterator
	template<typename Func>
	void each(Func func)
	{
		if (!m_isValid) return; // Return if view is not valid

		auto dispatch = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
			(..., [&]() {
				if (Is == m_mainComponentIdx) {
					iterate_with_lead<Is>(func);
				}
				}());
		};

		dispatch(std::make_index_sequence<sizeof...(Components)>{});
	}

private:
	std::tuple<ComponentArray<Components>*...> pArrays;
	bool m_isValid = false;
	size_t m_mainComponentIdx = 0;

	// Iteration loop
	template<size_t LeadIdx, typename Func>
	void iterate_with_lead(Func& func)
	{
		auto* lead_array = std::get<LeadIdx>(pArrays);
		const size_t size = lead_array->getArraySize();

		// Iterates using lead_array and calls function
		for (size_t i = 0; i < size; ++i)
		{
			const Entity entity = lead_array->indexToEntity(i);

			if (checkIndex<LeadIdx>(entity))
			{
				func(entity, *(std::get<ComponentArray<Components>*>(pArrays)->get(entity))...);
			}
		}
	}

	// Check if every compArray contain Entity e (except main compArray)
	template<size_t LeadIdx>
	bool checkIndex(Entity e) const
	{
		return[&]<std::size_t... Is>(std::index_sequence<Is...>) {
			return (... && (Is == LeadIdx || std::get<Is>(pArrays)->containsEntity(e)));
		}(std::make_index_sequence<sizeof...(Components)>{});
	}
};


class Registry
{
public:
	Registry() = default;

	Entity createEntity()
	{
		return entManager.createEntity();
	}
	void removeEntity(Entity entity)
	{
		entManager.removeEntity(entity);
		for (auto& i : components)
		{
			i.second->remove(entity);
		}
	}

	template<typename Component>
	void addComponent(Entity entity, Component comp)
	{
		std::type_index type = typeid(Component);

		// Component type is not known, create array
		if (components.find(type) == components.end())
			components[type] = std::make_unique<ComponentArray<Component>>();

		ComponentArray<Component>* arr = static_cast<ComponentArray<Component>*>(components[type].get());
		arr->insert(entity, comp);
	}
	template<typename Component>
	void removeComponent(Entity entity)
	{
		std::type_index type = typeid(Component);
		if(components.find(type) != components.end())
			components[type]->remove(entity);
	}
	
	template<typename Component>
	Component* getComponent(Entity entity)
	{
		std::type_index type = typeid(Component);
		if (components.find(type) != components.end())
		{
			ComponentArray<Component>* pComp = static_cast<ComponentArray<Component>*>(components.at(type).get());
			if (pComp->containsEntity(entity))
				return pComp->get(entity);
		}
		return nullptr;
	}

	template<typename Component>
	size_t getComponentCount() {
		auto* comp = getComponentArray<Component>();
		if (comp) return comp->getArraySize();
		return 0;
	}

	template<typename Component, typename... Others>
	View<Component, Others...> getView()
	{
		// Coletamos todos os ponteiros tipados
		// e deixamos o construtor da View fazer a validação e encontrar o menor!
		return View<Component, Others...>(
			getComponentArray<Component>(),
			getComponentArray<Others>()...
		);
	}

private:
	template<typename Component>
	ComponentArray<Component>* getComponentArray()
	{
		std::type_index type = typeid(Component);
		auto res = components.find(type);
		if (res == components.end())
			return nullptr;
		//return res->second().get();
		return static_cast<ComponentArray<Component>*>(res->second.get());
	}
private:

	// Note to self: dont expose entManager
	EntityManager entManager;

	std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> components;

};

