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

Texture2D tex : register(t0);
SamplerState samp;

float4 main(VS_Output input) : SV_TARGET
{
    float3 pos = input.vertexPos;

    float3 lightNormal = normalize(lightPos - pos);



    return tex.Sample( samp, input.tex) * (dot(lightNormal, input.normals));
}