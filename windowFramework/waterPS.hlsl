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
cbuffer cameraPosition : register(b2)
{
    float iTime;
};

Texture2D tex : register(t0);
Texture2D normal : register(t1);

SamplerState samp;

uint triple32(uint x)
{
    x ^= x >> 17;
    x *= 0xed5ad4bbU;
    x ^= x >> 11;
    x *= 0xac4c1b51U;
    x ^= x >> 15;
    x *= 0x31848babU;
    x ^= x >> 14;
    return x;
}

float4 main(VS_Output input) : SV_TARGET
{
    
    //return float4((normalize(input.normals) + 1)/2, 1.f);
    //return float4((normalize(input.tangents) + 1)/2, 1.f);
    
    //WATER CALCS
    float saida = 0.0f;
    float zoom = 0.1f;
    float2 uv = float2(input.vertexPos.x, input.vertexPos.z);
    //float iTime = cameraPos.x / 3.0f;
    float2 normal2d = float2(0.0f, 0.0f);
    float dx = 0.0f;
    float dy = 0.0f;
    for (int i = 0; i < 100; i++)
    {
        float hash1 = (float(triple32(uint(i))) / 1000000.0f); //(3.0f*cos(float(i)));
        float hash2 = (float(triple32(uint(i * 2 + 1))) / 1000000.0f); //(3.0f*sin(float(i)));
        hash1 = frac(hash1);
        hash2 = frac(hash2);
        
        
        float angle = hash1 * 3.14159f * 2.0f;
        float2x2 rotMat = float2x2(cos(angle), -sin(angle), sin(angle), cos(angle));
        float2 newUv = mul(uv, rotMat);
        
        //modifica distribuição
        hash1 = pow(hash1, 2.0f);
        hash2 = pow(hash2, 7.0f);
        
        float hash3 = frac(hash1 * 5.0f);
        
        float2 speed = float2((hash1 * 2.0f) - 1.0f, (hash3 * 2.0f) - 1.0f) * 3.0f;
        
        //newUv += vec2(hash1 * iTime, hash2 * iTime) * 0.05f ;
        
        float xVal = newUv.x * hash2 * 220.0f * zoom + 100.0f * sin(hash3 * 4.0f) + speed.x * iTime;
        float yVal = newUv.y * hash2 * 220.0f * zoom + 100.0f * sin(hash3 * 4.0f) + speed.y * iTime;
        
        saida +=
            cos(xVal) * (1.0f - hash2) +
            sin(yVal) * (1.0f - hash2);
        
        dx += -sin(xVal) * (1.0f - hash2);
        dy += cos(yVal) * (1.0f - hash2);
        
        //saida += 
        //    cos(newUv.x*hash1*220.0f + 5.0f*sin(hash1*57.0f + iTime) + iTime) +
        //    sin(newUv.y*hash2*220.0f + 5.0f*cos(hash2*23.0f + iTime) + iTime);
    }
    saida /= 100.0f;
    dx /= 100.0f;
    dy /= 100.0f;
    
    float3 normals = cross(
        normalize(float3(1.0f, 0.0f, dx)),
        normalize(float3(0.0f, 1.0f, dy))
    );
    normals = normalize(normals);
    
    saida += 1.0f;
    saida /= 2.0f;
    
    float4 color = float4(0.1f, 0.3f, 0.7f, 1.0f) * lerp(0.5f, 1.0f, saida);
    color.a = 1.0f;
    
    
    //calcula normal
    float3 bitan = cross(input.normals, input.tangents);
    //float3 normals = normal.Sample(samp, input.tex).xyz; //input.normals;
    //normals.x = (normals.x * 2) - 1;
    //normals.y = (normals.y * 2) - 1;
    
    float3x3 tangentSpaceTransform = float3x3(
        normalize(input.tangents),
        normalize(bitan),
        normalize(input.normals)
    );
    normals = mul(normals, tangentSpaceTransform);
    
    
    //PHONG SHADER
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


    if (dot(input.normals, directionLight) < 0.0f)
        specular = 0.0f;
    else
    {
        specular = pow(specular, 5);
        specular /= 4;
    }


    //calcula brilho da face
    //diffuse = att * max(0.0f, dot(directionLight, normals));
    diffuse = normals.y;
    //diffuse = 1.0f; //TIRAR ISSO AQUI
    specular *= att;
    
    //float4 color = tex.Sample(samp, input.tex);
    float4 specularColor = float4(1.0f, 1.0f, 1.0f, 1.0f) * specular;
    diffuse += 0.2f; //global illumination
    float4 finalColor = saturate(color * diffuse + specularColor);
    finalColor.a = color.a;
    
    return finalColor; //saturate((tex.Sample(samp, input.tex) * (factor+0.2)) + float4(1.0f,1.0f,1.0f,1.0f)*specular);

}