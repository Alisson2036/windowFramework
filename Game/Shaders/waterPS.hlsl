

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
    double iTime;
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

float ProceduralWaterNoise(float2 pos, float time)
{
    float iter = 0.0;
    float frequency = 1.0;
    float timeMultiplier = 2.0;
    float weight = 1.0;
    float sumOfValues = 0.0;
    float sumOfWeights = 0.0;
    
    [unroll]
    for (int i = 0; i < 5; i++)
    {
        float2 p = float2(sin(iter), cos(iter));
        
        float x = dot(p, pos) * frequency + time * timeMultiplier;
        float wave = exp(sin(x) - 1.0);

        sumOfValues += wave * weight;
        sumOfWeights += weight;

        weight = lerp(weight, 0.0, 0.2);
        frequency *= 1.1;
        timeMultiplier *= 1.07;
        iter += 1232.399963;
    }
    
    return sumOfValues / sumOfWeights;
}

// 2. Extrai a Normal calculando a inclinação do ruído
float3 GetProceduralNormal(float2 pos, float time, float detailScale, float detailHeight)
{
    float e = 0.02; // Tamanho da amostra (menor = normal mais afiada)
    float2 ex = float2(e, 0.0);
    
    // Multiplica a posição por uma escala para os detalhes ficarem pequenos
    float2 samplePos = pos * detailScale;

    // Amostra a altura no ponto atual e em pontos vizinhos (X e Z)
    float H = ProceduralWaterNoise(samplePos, time) * detailHeight;
    float Hx = ProceduralWaterNoise(samplePos - ex, time) * detailHeight;
    float Hz = ProceduralWaterNoise(samplePos - float2(0.0, e), time) * detailHeight;

    // Calcula os vetores tangentes com base na diferença de altura
    float3 ddx = float3(e, H - Hx, 0.0);
    float3 ddz = float3(0.0, H - Hz, e);

    // Produto vetorial retorna a Normal 3D apontando para cima (Y)
    return normalize(cross(ddz, ddx));
}


float4 main(VS_Output input) : SV_TARGET
{
    // 1. Vetores Básicos
    float3 worldNormal = normalize(input.normals);
    float3 V = normalize(cameraPos - input.vertexPos);
    float3 L = normalize(lightPos - input.vertexPos);
    
    // Distância da câmera até o pixel
    float distToCamera = length(cameraPos - input.vertexPos);

    // 2. Ruído Procedural (Ondas do Shadertoy)
    // Parâmetros: Pos, Time, Escala do Ruído, Altura do Ruído
    float3 detailNormal = GetProceduralNormal(input.vertexPos.xz, (float) iTime, 5.0f, 0.45f);

    // 3. Atenuação por Distância (Anti-Aliasing)
    // Faz o detalhe procedural sumir suavemente após certa distância
    float fadeFactor = saturate(distToCamera * 0.015f); // Ajuste 0.015 para fazer sumir mais perto ou mais longe
    
    // Mistura a normal detalhada de volta ao "liso" (0,1,0) baseada na distância
    detailNormal = lerp(detailNormal, float3(0, 1, 0), fadeFactor);

    // 4. MISTURA DAS NORMAIS (Geometria + Procedural)
    // Como ambas estão no World Space e apontam majoritariamente para Y, somá-las funciona bem
    float3 N = normalize(worldNormal + detailNormal * 0.6f); // 0.6 = Força da mistura

    // --- Cálculo de Fresnel e Iluminação ---
    
    float NdotV = saturate(dot(N, V));
    float R0 = 0.02;
    float fresnel = R0 + (1.0 - R0) * pow(1.0 - NdotV, 5.0);

    float NdotL = saturate(dot(N, L));
    float3 waterBaseColor = float3(0.02, 0.15, 0.35);
    float3 diffuse = waterBaseColor * (0.2 + 0.8 * NdotL);

    float3 H = normalize(L + V);
    float NdotH = saturate(dot(N, H));
    
    float specularFactor = pow(NdotH, 256.0f);
    float3 specular = float3(1.0, 1.0, 1.0) * specularFactor * 1.5f;

    float3 skyColor = float3(0.4, 0.65, 0.95);
    float3 colorWithFresnel = lerp(diffuse, skyColor, fresnel);
    
    float3 finalColor = colorWithFresnel + specular;

    return float4(saturate(finalColor), 1.0f);
}