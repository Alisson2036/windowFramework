#include "AssetLoader.h"

AssetLoader::AssetLoader(AssetManager* assetManager, ComponentFactory* factory)
	:
    m_factory(factory),
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

    pugi::xml_node assetRoot = doc.child("Scene").child("Assets");
    pugi::xml_node objRoot = doc.child("Scene").child("Objects");

    // LOAD MESHES
    for (pugi::xml_node meshNode : assetRoot.child("Meshes").children("Mesh")) {
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
    for (pugi::xml_node texNode : assetRoot.child("Textures").children("Texture")) {
        std::string name = texNode.attribute("name").as_string();
        std::string path = texNode.attribute("path").as_string();
        bool srgb = texNode.attribute("mipmap").as_bool(true);

        m_assetManager->CreateAsset<TextureAsset>(name, path, srgb);
    }

    // LOAD SHADERS
    for (pugi::xml_node shaderNode : assetRoot.child("Shaders").children("Shader")) {
        std::string name = shaderNode.attribute("name").as_string();
        std::string vs = shaderNode.attribute("vs").as_string();
        std::string vscso = shaderNode.attribute("vscso").as_string();
        std::string ps = shaderNode.attribute("ps").as_string();
        std::string pscso = shaderNode.attribute("pscso").as_string();

        m_assetManager->CreateAsset<ShaderAsset>(name, vs, vscso, ps, pscso);
    }

    // LOAD MATERIALS
    for (pugi::xml_node matNode : assetRoot.child("Materials").children("Material")) {
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

    // LOAD OBJECTS
    for (pugi::xml_node objNode : objRoot.children("Object")) {

        // Loading object
        SpatialData sd = toSpatialData(objNode);

        auto material = m_assetManager->getAsset<MaterialAsset>(
            objNode.attribute("material").as_string()
        ); 
        auto mesh = m_assetManager->getAsset<MeshAsset>(
            objNode.attribute("mesh").as_string()
        );

        if (!(material && mesh)) _throwMsg(
            std::string("Attempting to create object without required assets:") +
            objNode.attribute("material").as_string() +
            objNode.attribute("mesh").as_string()
        );

        // Checking repeat
        int amountX = 1, amountY = 1, amountZ = 1;
        float distX = 0.0f, distY = 0.0f, distZ = 0.0f;

        pugi::xml_node repeatNode = objNode.child("Repeat");
        if (repeatNode) {
            if (pugi::xml_node xNode = repeatNode.child("X")) {
                amountX = xNode.attribute("amount").as_int(1);
                distX = xNode.attribute("distance").as_float(0.0f);
            }
            if (pugi::xml_node yNode = repeatNode.child("Y")) {
                amountY = yNode.attribute("amount").as_int(1);
                distY = yNode.attribute("distance").as_float(0.0f);
            }
            if (pugi::xml_node zNode = repeatNode.child("Z")) {
                amountZ = zNode.attribute("amount").as_int(1);
                distZ = zNode.attribute("distance").as_float(0.0f);
            }
        }

        // Repeat logic
        for (int nx = 0; nx < amountX; nx++) {
        for (int ny = 0; ny < amountY; ny++) {
        for (int nz = 0; nz < amountZ; nz++) {
            SpatialData newSD = sd;
            newSD.move(vec3(distX* nx, distY* ny, distZ* nz));
            Entity e = m_factory->createObject(
                material,
                mesh,
                newSD
            );
        }
        }
        }
    }


    return true;
}

std::wstring AssetLoader::toWString(std::string & str)
{
	return std::wstring(str.begin(), str.end());
}

SpatialData AssetLoader::toSpatialData(pugi::xml_node& node)
{
    SpatialData sd;
    
    // Position
    pugi::xml_node posNode = node.child("Position");
    float px = posNode.attribute("x").as_float(5.0f);
    float py = posNode.attribute("y").as_float(5.0f);
    float pz = posNode.attribute("z").as_float(5.0f);

    // Rotation
    pugi::xml_node rotNode = node.child("Rotation");
    float rx = rotNode.attribute("x").as_float(0.0f);
    float ry = rotNode.attribute("y").as_float(0.0f);
    float rz = rotNode.attribute("z").as_float(0.0f);

    // Scale
    pugi::xml_node sclNode = node.child("Scale");
    float sx = sclNode.attribute("x").as_float(1.0f);
    float sy = sclNode.attribute("y").as_float(1.0f);
    float sz = sclNode.attribute("z").as_float(1.0f);

    sd.set(vec3(px, py, pz), vec3(rx, ry, rz));
    sd.setScale(vec3(sx, sy, sz));

    return sd;
}


