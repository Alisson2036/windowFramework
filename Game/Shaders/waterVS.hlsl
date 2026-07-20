#define DEEP_COLOR float3(0.05, 0.2, 0.45)      // Azul escuro profundo (vales)
#define SHALLOW_COLOR float3(0.1, 0.4, 0.6)   // Azul claro/esverdeado (cristas)
#define REFLECTION_COLOR float3(0.8, 0.9, 1.0) // Cor do "céu" refletido
#define nWaves 8u

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
    double iTimeVS;
};



struct GerstnerWave
{
    float2 direction; // Direção do vento/onda (será normalizada)
    float amplitude; // Altura do pico (A)
    float steepness; // Agudeza / Sharpness Q (0 a 1)
    float wavelength; // Distância entre cristas
    float speed; // Velocidade de propagação
};

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

// Calcula a posição deslocada e acumula Tangente/Binormal para os Normais
void EvaluateGerstnerWave(
    GerstnerWave wave,
    float2 gridPosXZ,
    float time,
    inout float3 offsetPosition,
    inout float3 tangent,
    inout float3 binormal
)
{
    float2 d = normalize(wave.direction);
    float k = 2.0 * 3.14159265 / wave.wavelength;
    float f = k * (dot(d, gridPosXZ) - wave.speed * time);
    
    float q = wave.steepness / (k * wave.amplitude);
    
    //float cosF = cos(f); 
    float cosF = exp(cos(f) - 1.0f) * 2.0 - 1.0;
    // float sinF = sin(f); 
    float sinF = exp(sin(f) - 1.0f) * 2.0 - 1.0;
    
    offsetPosition.x += q * wave.amplitude * d.x * cosF;
    offsetPosition.z += q * wave.amplitude * d.y * cosF;
    offsetPosition.y += wave.amplitude * sinF;
    
    tangent += float3(
        -d.x * d.x * (wave.steepness * sinF),
        d.x * (k * wave.amplitude * cosF),
        -d.x * d.y * (wave.steepness * sinF)
    );

    binormal += float3(
        -d.x * d.y * (wave.steepness * sinF),
        d.y * (k * wave.amplitude * cosF),
        -d.y * d.y * (wave.steepness * sinF)
    );
}

void CalculateOceanSurface(
    float3 worldPos,
    float time,
    GerstnerWave waves[nWaves],
    out float3 finalPosition,
    out float3 finalNormal
)
{
    // Frequência e velocidade do ruído
    float2 noiseUV = worldPos.xz * 0.08 + float2(time * 0.1, time * 0.05);

    // Fffset 2D entre -1.0 e 1.0
    float noiseX = Noise2D(noiseUV) * 2.0 - 1.0;
    float noiseZ = Noise2D(noiseUV + float2(17.5, 31.2)) * 2.0 - 1.0; // Offset de amostragem para desvincular X de Z

    // Posição distorcida 
    worldPos.xz += float2(noiseX, noiseZ) * 0.8;
    
    
    float3 offset = float3(0, 0, 0);
    float3 tangent = float3(1, 0, 0);
    float3 binormal = float3(0, 0, 1);

    // Ondas
    [unroll]
    for (int i = 0; i < nWaves; i++)
    {
        EvaluateGerstnerWave(waves[i], worldPos.xz, time, offset, tangent, binormal);
    }

    finalPosition = worldPos + offset;
    
    finalNormal = normalize(cross(binormal, tangent));
}

VS_Output main(VS_Input input)
{
    VS_Output output;

    GerstnerWave waves[nWaves] =
    {
    // 1. Swell Principal (Onda longa de fundo)
        { float2(1.0, 0.15), 0.35, 0.20, 28.0, 2.8 },
    
    // 2. Swell Secundário (Cruzado em ângulo aberto)
        { float2(-0.7, 0.71), 0.20, 0.15, 16.5, 2.2 },
    
    // 3. Onda Média A (Ondulação regional)
        { float2(0.4, 0.91), 0.12, 0.12, 9.3, 1.8 },
    
    // 4. Onda Média B (Vento de superficie)
        { float2(-0.8, -0.55), 0.08, 0.10, 5.7, 1.4 },
    
    // 5. Marola Local A
        { float2(0.9, -0.32), 0.05, 0.08, 3.1, 1.2 },
    
    // 6. Marola Local B
        { float2(-0.2, -0.98), 0.03, 0.06, 1.8, 1.0 },
    
    // 7. Micro Ondulação A (Quebra a lisura das cristas)
        { float2(0.6, 0.80), 0.02, 0.04, 1.1, 0.8 },
    
    // 8. Micro Ondulação B (Alta frequência para reflexos de luz)
        { float2(-0.95, 0.28), 0.01, 0.02, 0.6, 0.6 }
    };

    float4 pos = mul(float4(input.pos, 1.0f), transpose(mat));
    float3 displacedPos = pos;
    float3 waveNormal = input.normals;
    
    CalculateOceanSurface(pos.xyz, (float) iTimeVS, waves, displacedPos, waveNormal);

    output.vertexPos = displacedPos;
    output.normals = waveNormal;
    output.position = mul(float4(displacedPos, 1.0f), transpose(projectionMat)); // Aplique sua matriz VP aqui
    output.tex = input.tex;

    return output;
}