#include "AssetLoader.h"

AssetLoader::AssetLoader(AssetManager* assetManager, Registry* registry)
	:
	m_registry(registry),
	m_assetManager(assetManager)
{}

bool AssetLoader::loadFromXML(const std::string & filepath)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filepath.c_str());

    if (!result) {
        _throwMsg(std::string("Error reading XML: ") + result.description());
        return false;
    }

    pugi::xml_node root = doc.child("Assets");

    // LOAD MESHES
    for (pugi::xml_node meshNode : root.child("Meshes").children("Mesh")) {
        std::string name = meshNode.attribute("name").as_string();
        pugi::xml_attribute pathAttr = meshNode.attribute("path");

        if (pathAttr) {
            m_assetManager->CreateAsset<MeshAsset>(name, pathAttr.as_string());
        }
        else {
            // Create asset only
            m_assetManager->CreateAsset<MeshAsset>(name);
        }
    }

    // LOAD TEXTURES
    for (pugi::xml_node texNode : root.child("Textures").children("Texture")) {
        std::string name = texNode.attribute("name").as_string();
        std::string path = texNode.attribute("path").as_string();
        bool srgb = texNode.attribute("mipmap").as_bool(true);

        m_assetManager->CreateAsset<TextureAsset>(name, path, srgb);
    }

    // LOAD SHADERS
    for (pugi::xml_node shaderNode : root.child("Shaders").children("Shader")) {
        std::string name = shaderNode.attribute("name").as_string();
        std::string vs = shaderNode.attribute("vs").as_string();
        std::string vscso = shaderNode.attribute("vscso").as_string();
        std::string ps = shaderNode.attribute("ps").as_string();
        std::string pscso = shaderNode.attribute("pscso").as_string();

        m_assetManager->CreateAsset<ShaderAsset>(name, vs, vscso, ps, pscso);
    }

    // LOAD MATERIALS
    for (pugi::xml_node matNode : root.child("Materials").children("Material")) {
        std::string name = matNode.attribute("name").as_string();
        std::string shaderName = matNode.attribute("shader").as_string();

        auto* shader = m_assetManager->getAsset<ShaderAsset>(shaderName);
        if (!shader) _throwMsg(std::string("Shader does not exist:") + shaderName);

        // Verify texture attribute
        pugi::xml_attribute texAttr = matNode.attribute("texture");
        if (texAttr) {
            auto* texture = m_assetManager->getAsset<TextureAsset>(texAttr.as_string());

            // Shader with normal map
            pugi::xml_attribute normalAttr = matNode.attribute("normalMap");
            if (normalAttr) {
                auto* normalTex = m_assetManager->getAsset<TextureAsset>(normalAttr.as_string());
                m_assetManager->CreateAsset<MaterialAsset>(name, shader, texture, normalTex);
            }
            else {
                m_assetManager->CreateAsset<MaterialAsset>(name, shader, texture);
            }
        }
        else {
            // Shader without texture
            m_assetManager->CreateAsset<MaterialAsset>(name, shader);
        }

    }

    return true;
}

std::wstring AssetLoader::toWString(std::string & str)
{
	return std::wstring(str.begin(), str.end());
}


