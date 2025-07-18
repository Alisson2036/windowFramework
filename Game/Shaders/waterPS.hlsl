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
cbuffer timer : register(b2)
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

#define nWaves 100u

float4 main(VS_Output input) : SV_TARGET
{
    
    //return float4((normalize(input.normals) + 1)/2, 1.f);
    //return float4((normalize(input.tangents) + 1)/2, 1.f);
    
    //WATER CALCS
    float saida = 0.0f;
    float zoom = 100.f;
    float2 uv = float2(input.vertexPos.x, input.vertexPos.z);
    float2 normal2d = float2(0.0f, 0.0f);
    float dx = 0.0f;
    float dy = 0.0f;
    for (int i = 0; i < nWaves; i++)
    {
        float hash1 = (float(triple32(uint(i))) / 1000000.0f); //(3.0f*cos(float(i)));
        float hash2 = (float(triple32(uint(i * 2 + 1))) / 1000000.0f); //(3.0f*sin(float(i)));
        hash1 = frac(hash1);
        hash2 = frac(hash2);
        
        //rotaciona o UV
        float angle = hash1 * 3.14159f * 2.0f;
        float2x2 rotMat = float2x2(cos(angle), -sin(angle), sin(angle), cos(angle));
        float2 newUv = mul(uv, rotMat);
        
        //modifica distribuição
        hash1 = pow(hash1, 2.0f);//hash o resto
        hash2 = pow(hash2, 8.0f);//hash para tamanho
        //cria mais um hash
        float hash3 = frac(hash1 * 5.0f);
        
        float2 speed = normalize(float2((hash1 * 2.0f) - 1.0f, (hash3 * 2.0f) - 1.0f)) * 1 / (hash2+0.3f);
        
        //newUv += vec2(hash1 * iTime, hash2 * iTime) * 0.05f ;
        
        float xVal = (newUv.x * hash2 * zoom) + (100.0f * sin(hash3 * 4.0f) + speed.x * iTime);
        float yVal = (newUv.y * hash2 * zoom) + (100.0f * sin(hash3 * 4.0f) + speed.y * iTime);
        
        //cor principal
        saida +=
            cos(xVal) * pow(1.0f - hash2,3) +
            sin(yVal) * pow(1.0f - hash2,3);
        
        //derivada para calcular normal
        //note q o X ta invertido (nao sei pq mas assim fica correto)
        
        float2 d = float2(
            -sin(xVal) * pow(1.0f - hash2, 5) * hash2 * zoom,
            cos(yVal) * pow(1.0f - hash2, 5) * hash2 * zoom
        );
        d = mul(d, transpose(rotMat));
        
        dx += d.x;
        dy += d.y;
        
        //dx += -sin(xVal) * (1.0f - hash2) * hash2 * zoom;
        //dy += cos(yVal) * (1.0f - hash2) * hash2 * zoom;
        
    }
    saida /= float(nWaves);
    dx /= float(nWaves);
    dy /= float(nWaves);
    
    float3 normals = cross(
        normalize(float3(1.0f, dx, 0.0f)),
        normalize(float3(0.0f, dy, 1.0f))
    );
    normals = normalize(-normals);
    
    //saida agora varia entre (0~1) e nao entre (-1~1)
    saida += 1.0f;
    saida /= 2.0f;
    
    float4 color = lerp(
        float4(0.18f, 0.30f, 0.7f, 1.0f),
        float4(0.24f, 0.43f, 0.8f, 1.0f),
        //float4(1.0f,0.0f,0.0f,1.0f),
        pow(saida, 3)
    );
    color += float4(1.f, 1.f, 1.f, 1.f) * pow(saida, 7) * 3;
    color.a = 1.0f;
   
    
    
    //PHONG SHADER
    float3 pos = input.vertexPos;
    float3 directionLight = normalize(lightPos - pos);
    //float3 directionLight = float3(0.0f, 0.5f, 0.85f);
    float3 cameraVertexDif = normalize(cameraPos - input.vertexPos);
    
    float diffuse = 0.0f;
    float specular = 0.0f;

    //calcula atenuacao da luz de acordo com a distancia 
    float att = 1.0f;
    
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
    //diffuse = dot(normals, float3(3.f/5.f,4.f/5.f,0.f));
    diffuse = 1.0f; //TIRAR ISSO AQUI
    //specular *= att;
    
    //float4 color = tex.Sample(samp, input.tex);
    float4 specularColor = float4(1.0f, 1.0f, 1.0f, 1.0f) * specular;
    float4 finalColor = saturate(color * diffuse + specularColor);
    finalColor.a = 1.0f;
    
    return finalColor;
    //return float4(saida, 0.0f, specular*8.f, 1.0f); //saturate((tex.Sample(samp, input.tex) * (factor+0.2)) + float4(1.0f,1.0f,1.0f,1.0f)*specular);

}