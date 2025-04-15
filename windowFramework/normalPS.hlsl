struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
    float3 normals : Normals;
    float3 tangents : Tangents;
    float3 vertexPos : Position;
};


cbuffer light : register(b0)
{
    float3 lightPos;
};
cbuffer cameraPosition : register(b1)
{
    float3 cameraPos;
};

Texture2D tex : register(t0);
Texture2D normal : register(t1);

SamplerState samp;

float4 main(VS_Output input) : SV_TARGET
{
    //calcula normal
    float3 bitan = cross(input.normals, input.tangents);
    float3 normals = normal.Sample(samp, input.tex).xyz; //input.normals;
    normals.x = (normals.x * 2) - 1;
    normals.y = (normals.y * 2) - 1;
    
    float3x3 tangentSpaceTransform = float3x3(
        normalize(input.tangents),
        normalize(bitan), 
        normalize(input.normals)
    ); 
    normals = mul(normals, tangentSpaceTransform);
    
    //return float4((normalize(input.normals) + 1)/2, 1.f);
    //return float4((normalize(input.tangents) + 1)/2, 1.f);
    
    float3 pos = input.vertexPos;
    float3 directionLight = normalize(lightPos - pos);
    float3 cameraVertexDif = normalize(cameraPos - input.vertexPos);
    
    float diffuse = 0.0f;
    float specular = 0.0f;

    //calcula atenuacao da luz de acordo com a distancia 
    float att = max(0.0f, 1 - (distance(input.vertexPos, lightPos) * 0.03));
    
    //calcula especular 
    float3 refraction = reflect(cameraVertexDif, normals);
    specular = max(0.0f, dot(refraction, -directionLight));


    if (dot(normals, directionLight) < 0.0f)
        specular = 0.0f;
    else
    {
        specular = pow(specular, 5);
        specular /= 4;
    }


    //calcula brilho da face
    diffuse = att * max(0.0f, dot(directionLight, normals));
    specular *= att;
    
    float4 color = tex.Sample(samp, input.tex);
    float4 specularColor = float4(1.0f, 1.0f, 1.0f, 1.0f) * specular;
    diffuse += 0.2f; //global illumination
    float4 finalColor = saturate(color * diffuse + specularColor);
    finalColor.a = color.a;
    
    return finalColor; //saturate((tex.Sample(samp, input.tex) * (factor+0.2)) + float4(1.0f,1.0f,1.0f,1.0f)*specular);

}