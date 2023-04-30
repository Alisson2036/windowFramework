struct VS_Input
{
    float2 pos : Position2d;
    float2 texCoord : TexCoord;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float2 texCoord : TexCoord;
};

VS_Output main( VS_Input input)
{
    VS_Output output;

    output.position = float4(input.pos, 0.0f,1.0f);
    output.texCoord = float2(0.0f, 1.0f);

	return output;
}