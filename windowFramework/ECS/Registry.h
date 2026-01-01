#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include <algorithm>

#include "EntityManager.h"
#include "ComponentArray.h"


class Registry
{
public:
	Registry() = default;

	class View
	{
		friend class Registry;
	public:
		View() : mainComponent(typeid(void)) {};

		// --Iterator class--
		class Iterator
		{
		public:
			Iterator(View* view, size_t index) :
				m_view(view),
				m_index(index)
			{}
			Iterator& operator++()
			{
				m_index = m_view->next(m_index);
				return *this;
			}
			friend bool operator!=(const Iterator& a, const Iterator& b)
			{
				return a.m_index != b.m_index or a.m_view != a.m_view;
			}
			Iterator& operator*()
			{
				return *this;
			}
			template<typename Component>
			Component* get()
			{
				return m_view->get<Component>(m_index);
			}
			Entity getEntity()
			{
				return m_view->getEntity(m_index);
			}
		private:
			View* m_view;
			size_t m_index;
		};

		// --Public member functions--
		Iterator begin()
		{
			return Iterator(this, 0);
		}
		Iterator end()
		{
			if (pArrays.empty())
				return Iterator(this, 0);

			return Iterator(this, pArrays.at(mainComponent)->getArraySize() + 1u);
		}


	private:

		size_t next(size_t index)
		{
			do
			{
				index += 1;
				if (index == pArrays[mainComponent]->getArraySize())
					return pArrays[mainComponent]->getArraySize() + 1;
			} while (!checkIndex(index));
			return index;
		}
		bool checkIndex(const size_t index) const
		{
			const Entity entity = getEntity(index);
			for (const auto& pair : pArrays)
			{
				if (!pair.second->containsEntity(entity))
					return false;
			}
			return true;
		}
		Entity getEntity(const size_t index) const
		{
			return pArrays.at(mainComponent)->indexToEntity(index);
		}
		template<typename Comp>
		Comp* get(size_t index)
		{
			std::type_index type = typeid(Comp);
			if (type == mainComponent)
			{
				return static_cast<ComponentArray<Comp>*>(pArrays.at(mainComponent))
					->getByIndex(index);
			}
			Entity ent = getEntity(index);
			IComponentArray* pComp = pArrays.find(type)->second;
			ComponentArray<Comp>* pCompTyped = static_cast<ComponentArray<Comp>*>(pComp);
			return pCompTyped->get(ent);
		}

		// Adiciona pointer para componente
		// Atenção: todos os componentes devem
		// ser adicionados antes de qualquer
		// outra chamada de função.
		template<typename Comp> 
		void addComp(ComponentArray<Comp>* component)
		{
			// Nao adiciona componente nulo
			if (component == nullptr) return;

			// Adiciona pointer
			std::type_index type = typeid(Comp);
			pArrays[type] = component;

			// Define tipo principal
			auto it = pArrays.find(mainComponent);
			if (it != pArrays.end())
			{
				if (it->second->getArraySize() > pArrays[type]->getArraySize())
					mainComponent = type;
				return;
			}
			mainComponent = type;
		}

	private:
		std::unordered_map<std::type_index, IComponentArray*> pArrays;
		std::type_index mainComponent;
	};

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

    template<typename Component, typename... Others>
    View getView()
    {
		View view{};
        (view.addComp(getComponent<Component>()), ..., view.addComp(getComponent<Others>()));
        return view;
    }

private:
	template<typename Component>
	ComponentArray<Component>* getComponent()
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

