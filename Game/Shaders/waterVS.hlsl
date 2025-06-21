struct VS_Input
{
    float3 pos : Position;
    float2 tex : TexCoord;
    float3 normals : Normals;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
    float3 normals : Normals;
    float3 vertexPos : Position;
};

cbuffer buff : register(b0)
{
    matrix mat;
};
cbuffer projectionMatrix : register(b1)
{
    matrix projectionMat;
};
cbuffer timer : register(b3)
{
    float iTime;
};


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


VS_Output main(VS_Input input)
{
    VS_Output output;
    
    //matriz final
    output.position = mul(float4(input.pos, 1.0f), transpose(mat));
    
    //WATER CALCS
    float saida = 0.0f;
    float zoom = 100.f;
    float2 uv = float2(output.position.x, output.position.z);
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
        hash1 = pow(hash1, 2.0f); //hash o resto
        hash2 = pow(hash2, 8.0f); //hash para tamanho
        //cria mais um hash
        float hash3 = frac(hash1 * 5.0f);
        
        float2 speed = normalize(float2((hash1 * 2.0f) - 1.0f, (hash3 * 2.0f) - 1.0f)) * 1 / (hash2 + 0.3f);
        
        //newUv += vec2(hash1 * iTime, hash2 * iTime) * 0.05f ;
        
        float xVal = newUv.x * hash2 * zoom + 100.0f * sin(hash3 * 4.0f) + speed.x * iTime;
        float yVal = newUv.y * hash2 * zoom + 100.0f * sin(hash3 * 4.0f) + speed.y * iTime;
        
        //cor principal
        saida +=
            cos(xVal) * pow(1.0f - hash2, 3) +
            sin(yVal) * pow(1.0f - hash2, 3);
        
    }
    saida /= float(nWaves);
    
    //output.position.y += cos(output.position.x) * cos(output.position.z);
    output.position.y += saida * 3.0f;
    
    output.position = mul(output.position, transpose(projectionMat));


    float3x3 b = transpose((float3x3) mat);
    output.tex = input.tex;
    output.normals = mul(input.normals, b);

    output.vertexPos = (float3) mul(float4(input.pos, 1.0f), transpose(mat));
    

    return output;
}