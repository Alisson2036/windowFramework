#pragma once
#include <iostream>
#include <string>
#include "AssetManager.h"
#include "../ECS/Registry.h"
#include "../ECS/ComponentFactory.h"
#include "../thirdparty/pugixml/pugixml.hpp"
#include "../Core/exception.h"

class AssetLoader
{
public:
	AssetLoader(AssetManager* assetManager, Registry* registry);


	bool loadFromXML(const std::string& filepath);
private:
	std::wstring toWString(std::string& str);

private:
	AssetManager* m_assetManager;
	Registry* m_registry;

};