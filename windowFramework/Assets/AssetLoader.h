#pragma once
#include <iostream>
#include <string>
#include "AssetManager.h"
#include "../ECS/ComponentFactory.h"
#include "../thirdparty/pugixml/pugixml.hpp"
#include "../Core/exception.h"

class AssetLoader
{
public:
	AssetLoader(AssetManager* assetManager, ComponentFactory* factory);


	bool loadFromXML(const std::string& filepath);
private:
	std::wstring toWString(std::string& str);
	SpatialData toSpatialData(pugi::xml_node& node);

private:
	AssetManager* m_assetManager;
	ComponentFactory* m_factory;

};