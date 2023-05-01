
struct VS_Output
{
    float4 position : SV_POSITION;
    float2 texCoord : TexCoord;
};



Texture2D tex : register(t0);
SamplerState samp;

float4 main(VS_Output input) : SV_TARGET
{
    float4 color = tex.Sample(samp, input.texCoord);
    clip(color.a - 0.1f);
    return color;
}