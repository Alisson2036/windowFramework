struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
    float3 normals : Normals;
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
    //float3 normals = tex.Sample(samp, input.tex).xyz;
    //normals = normalize(normals);
    float3 normals = (0.0f, 0.0f, 1.0f);
    
    
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
        specular = pow(specular, 100);
        specular /= 2;
    }


    //calcula brilho da face
    diffuse = att * max(0.0f, dot(directionLight, normals));
    specular *= att;
    
    float4 color = tex.Sample(samp, input.tex);
    float4 specularColor = float4(1.0f, 1.0f, 1.0f, 1.0f) * specular;
    diffuse += 0.2f; //global illumination
    float4 finalColor = saturate(color * diffuse + specularColor);
    finalColor.a = color.a;
    normals = (0.f, 0.f, 1.f);
    float3 tempcolor = (0.f,0,1);
    return float4(0,0,1, 1.f); //saturate((tex.Sample(samp, input.tex) * (factor+0.2)) + float4(1.0f,1.0f,1.0f,1.0f)*specular);

}