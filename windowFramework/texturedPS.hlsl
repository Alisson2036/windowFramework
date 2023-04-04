struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
    float3 normals : Normals;
    float3 lightNormal : light;
};

Texture2D tex : register(t0);
SamplerState samp;

float4 main(VS_Output input) : SV_TARGET
{

    return tex.Sample( samp, input.tex) * (dot(input.lightNormal, input.normals));
}