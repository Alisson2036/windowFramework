#include "MeshAsset.h"

MeshAsset::MeshAsset()
{

}

MeshAsset::MeshAsset(const std::filesystem::path& path)
{
	filePath = path;
	fileName = path.filename().string();

}

void MeshAsset::Load()
{
    if (filePath.empty()) return;

    objLoader obj;
    obj.fromFile(filePath.string());
    vCount = obj.faces.size() * 3;

    const bool pos = !obj.vertices.empty();
    const bool normals = !obj.normals.empty();
    const bool textureCoords = !obj.texCoord.empty();
    const bool tangents = textureCoords && normals;

    if (pos)
    {
        meshData["Position"] = { 12u, std::vector<char>(12 * vCount) };
    }
    if (normals)
    {
        meshData["Normals"] = { 12u, std::vector<char>(12 * vCount) };
    }
    if (textureCoords)
    {
        meshData["TexCoord"] = { 8u, std::vector<char>(8 * vCount) };
    }
    if (tangents)
    {
        meshData["Tangents"] = { 12u, std::vector<char>(12 * vCount) };
    }

    int index = 0;
    for (const objLoader::face& face : obj.faces)
    {
        for (int i = 0; i < 3; i++)
        {
            if (pos)
            {
                float* dst = reinterpret_cast<float*>(meshData["Position"].buffer.data() + (index * 12u));
                const vec3& v = obj.vertices[face.vertexIndex[i] - 1];
                dst[0] = v.x; dst[1] = v.y; dst[2] = v.z;
            }
            if (textureCoords)
            {
                float* dst = reinterpret_cast<float*>(meshData["TexCoord"].buffer.data() + (index * 8u));
                const vec2& uv = obj.texCoord[face.textureIndex[i] - 1];
                dst[0] = uv.x; dst[1] = uv.y;
            }
            if (normals)
            {
                float* dst = reinterpret_cast<float*>(meshData["Normals"].buffer.data() + (index * 12u));
                const vec3& n = obj.normals[face.normalIndex[i] - 1];
                dst[0] = n.x; dst[1] = n.y; dst[2] = n.z;
            }
            if (tangents)
            {
                // Cálculo igual ao original
                vec3 v0 = obj.vertices[face.vertexIndex[0] - 1];
                vec3 v1 = obj.vertices[face.vertexIndex[1] - 1];
                vec3 v2 = obj.vertices[face.vertexIndex[2] - 1];

                vec2 uv0 = obj.texCoord[face.textureIndex[0] - 1];
                vec2 uv1 = obj.texCoord[face.textureIndex[1] - 1];
                vec2 uv2 = obj.texCoord[face.textureIndex[2] - 1];

                vec3 deltaPos1 = v1 - v0;
                vec3 deltaPos2 = v2 - v0;
                vec2 deltaUV1 = uv1 - uv0;
                vec2 deltaUV2 = uv2 - uv0;

                float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
                vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

                float* dst = reinterpret_cast<float*>(meshData["Tangents"].buffer.data() + (index * 12u));
                dst[0] = tangent.x; dst[1] = tangent.y; dst[2] = tangent.z;
            }
            index++;
        }
    }
}

void MeshAsset::Unload()
{
}

bool MeshAsset::IsLoaded() const
{
    return false;
}

const std::filesystem::path& MeshAsset::GetFilePath() const
{
	return "";
}

const std::string& MeshAsset::GetFileName() const
{
	return "";
}

void MeshAsset::getData(inputBuffer* out)
{
    out->reserve(vCount);

    if (out->containsType("Position"))
    {
        auto& v = meshData["Position"];
        out->setArray(
            v.buffer.data(),
            v.buffer.size() / v.stride,
            "Position"
        );
    }
    if (out->containsType("TexCoord"))
    {
        auto& v = meshData["TexCoord"];
        out->setArray(
            v.buffer.data(),
            v.buffer.size() / v.stride,
            "TexCoord"
        );
    }
    if (out->containsType("Normals"))
    {
        auto& v = meshData["Normals"];
        out->setArray(
            v.buffer.data(),
            v.buffer.size() / v.stride,
            "Normals"
        );
    }
    if (out->containsType("Tangents"))
    {
        auto& v = meshData["Tangents"];
        out->setArray(
            v.buffer.data(),
            v.buffer.size() / v.stride,
            "Tangents"
        );
    }
    
}
