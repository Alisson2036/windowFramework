

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
float hash21(float2 p)
{
    p = frac(p * float2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return frac(p.x * p.y);
}

float Noise2D(float2 p)
{
    float2 i = floor(p);
    float2 f = frac(p);
    f = f * f * (3.0 - 2.0 * f); // Interpolação Smoothstep

    float a = hash21(i);
    float b = hash21(i + float2(1.0, 0.0));
    float c = hash21(i + float2(0.0, 1.0));
    float d = hash21(i + float2(1.0, 1.0));

    return lerp(lerp(a, b, f.x), lerp(c, d, f.x), f.y);
}

float FBM(float2 p)
{
    float f = 0.0;
    float weight = 0.5;
    
    [unroll]
    for (int i = 0; i < 3; i++)
    {
        f += weight * Noise2D(p);
        p *= 2.02f; // Dobra a frequência para a próxima camada
        weight *= 0.5f; // Cai a intensidade pela metade
    }
    return f;
}

float ProceduralWaterNoise(float2 pos, float time)
{
    float iter = 0.0;
    float frequency = 1.0;
    float timeMultiplier = 2.0;
    float weight = 1.0;
    float sumOfValues = 0.0;
    float sumOfWeights = 0.0;
    
    // Frequência e velocidade do ruído
    float2 noiseUV = pos.xy * 0.8f + float2(time * 0.1, time * 0.05);

    // Fffset 2D entre -1.0 e 1.0
    float noiseX = Noise2D(noiseUV) * 2.0 - 1.0;
    float noiseZ = Noise2D(noiseUV+ float2(17.5, 31.2)) * 2.0 - 1.0; // Offset de amostragem para desvincular X de Z

    // Posição distorcida 
    pos.xy += float2(noiseX, noiseZ) * 0.5;
    
    [unroll]
    for (int i = 0; i < 5; i++)
    {
        float2 p = float2(sin(iter), cos(iter));
        
        float x = dot(p, pos + float2(iter%3.0f, iter%7.0f)) * frequency + time * timeMultiplier;
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
    
    float foamNoise = ProceduralWaterNoise(input.vertexPos.xz * 1.5, (float) iTime * 1.5);

    // A espuma aparece onde a altura (vertexPos.y) é alta E o ruído de turbulência é forte.
    // Ajuste o threshold (0.4) para controlar a quantidade de espuma.
    float foamThreshold = 0.05;

    // Smoothstep cria uma transição suave nas bordas da espuma.
    // Combinamos a altura da malha (y) e o ruído.
    float foamFactor = smoothstep(foamThreshold, foamThreshold + 0.2, foamNoise * saturate(input.vertexPos.y * 0.5 + 0.5));

    // Textura/Ruído secundário para "quebrar" a espuma (opcional, se você tiver uma textura)
    // float foamTexture = tex.Sample(samp, input.vertexPos.xz * 0.5).r;
    foamFactor *= FBM(input.vertexPos.xz * 8.0f);
    float3 foamColor = float3(0.8, 0.85, 1.0); // Branco levemente azulado
    
    // --- CÁLCULO DE SUBSURFACE SCATTERING (SSS) ---
    // 1. Consertamos a máscara de altura (agora o SSS só ocorre nas partes altas)
    float SSS_HeightMask = saturate(input.vertexPos.y + 1.3f);

    // 2. O Segredo: Distorcemos a visão da câmera empurrando-a com a Normal da água
    // Isso simula a luz "atravessando" o volume angulado da onda
    float3 sssView = normalize(V - N * 0.3f);
    
    // 3. Calculamos a intensidade baseada na luz vindo por trás (-L)
    float SSS_Intensity = saturate(dot(sssView, -L));
    SSS_Intensity = pow(SSS_Intensity, 5.0f); // Deixa o foco de luz mais concentrado
    
    // 4. Adicionamos um brilho "ambiente" leve nas cristas para a água não ficar morta
    float ambientSSS = saturate(input.vertexPos.y * 0.5f) * 0.15f;

    // Cor esverdeada brilhante típica de água do mar rasa iluminada
    float3 SSS_Color = float3(0.1, 0.6, 0.55) * 0.4f;

    float3 subsurface = SSS_Color * (SSS_Intensity + ambientSSS) * SSS_HeightMask;

    // --- Cálculo de Fresnel e Iluminação ---
    
    float NdotV = saturate(dot(N, V));
    float R0 = 0.02;
    float fresnel = R0 + (1.0 - R0) * pow(1.0 - NdotV, 8.0);

    float NdotL = saturate(dot(N, L));
    float3 waterBaseColor = float3(0.3, 0.6, 1.0)/2.0;
    float3 diffuse = waterBaseColor * (0.2 + 0.8 * NdotL);

    float3 H = normalize(L + V);
    float NdotH = saturate(dot(N, H));
    
    float specularFactor = pow(NdotH, 256.0f);
    float3 specular = float3(0.95, 0.95, 1.0) * specularFactor * 1.5f;

    float3 skyColor = float3(0.4, 0.65, 0.95);
    
    // 1. Mistura a cor base (difusa) com o céu (Fresnel)
    float3 colorWithFresnel = lerp(diffuse, skyColor, fresnel);
    
    // 2. Adiciona o Subsurface Scattering (brilho interno)
    float3 colorWithSSS = colorWithFresnel + subsurface;
    
    // 3. Adiciona o Especular (brilho do sol)
    float3 colorWithSpecular = colorWithSSS + specular;
    
    // 4. APLICA A ESPUMA (sobrepõe tudo o resto)
    float3 finalColor = lerp(colorWithSpecular, foamColor, foamFactor);

    return float4(saturate(finalColor), 1.0f);
}