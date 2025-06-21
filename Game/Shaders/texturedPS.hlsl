struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
    float3 normals : Normals;
    float3 tangents : Tangents;
    float3 vertexPos : Position;
    float4 shadowPos : shadowPos;
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
Texture2D<float> shadowMap : register(t2);
SamplerState samp;

float4 main(VS_Output input) : SV_TARGET
{
    float3 pos = input.vertexPos;
    //float3 directionLight = normalize(lightPos - pos);
    float3 directionLight = float3(0.0f, 1.0f, 0.0f);
    float3 cameraVertexDif = normalize(cameraPos - input.vertexPos);
    
    float diffuse = 0.0f;
    float specular = 0.0f;

    //calcula atenuacao da luz de acordo com a distancia 
    float att = max(0.0f, 1 - (distance(input.vertexPos, lightPos)*0.03));
    
    //calcula especular 
    float3 refraction = reflect(cameraVertexDif, input.normals);
    specular = max(0.0f, dot(refraction, -directionLight));


    if (dot(input.normals, directionLight) < 0.0f)
        specular = 0.0f;
    else
    {
        specular = pow(specular, 100);
        specular /= 2;
    }

    //calcula shadow
    float4 shadowPos = input.shadowPos; //mul(float4(input.vertexPos, 1.0f), transpose(lightMat));
    shadowPos /= shadowPos.w;
    //shadowPos = (shadowPos + 1) / 2;
    //shadowPos.y = -shadowPos.y;
    float2 shadowSamplePos = float2(shadowPos.x, -shadowPos.y);
    shadowSamplePos = (shadowSamplePos + 1) / 2;
    float shadowDist = shadowMap.Sample(samp, shadowSamplePos);
    float shadow = 1.0f;
    if (shadowPos.z - shadowDist > 0.005f)
        shadow = 0.0f;

    //calcula brilho da face
    diffuse = att * max(0.0f, dot(directionLight, input.normals));
    specular *= att;
    
    //tira iluminação nas sombras
    diffuse *= shadow;
    specular *= shadow;
    
    float4 color = tex.Sample(samp, input.tex);
    float4 specularColor = float4(1.0f, 1.0f, 1.0f, 1.0f) * specular;
    diffuse += 0.2f; //global illumination
    float4 finalColor = saturate(color * diffuse + specularColor);
    finalColor.a = 1.0f;
    
    
    //return float4(shadowPos.z - shadowDist, 0.0f, 0.0f, 1.0f); //saturate((tex.Sample(samp, input.tex) * (factor+0.2)) + float4(1.0f,1.0f,1.0f,1.0f)*specular);
    return finalColor;
}