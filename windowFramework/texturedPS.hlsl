struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
};

Texture2D tex : register(t0);
SamplerState samp;

float4 main(VS_Output input) : SV_TARGET
{
    return tex.Sample( samp, input.tex);
}