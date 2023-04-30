
struct VS_Output
{
    float4 position : SV_POSITION;
    float2 texCoord : TexCoord;
};


float4 main(VS_Output input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}