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

cbuffer buff : register(b0)
{
    row_major matrix mat;
};
cbuffer projectionMatrix : register(b1)
{
    matrix projectionMat;
};

VS_Output main( VS_Input input)
{
    VS_Output output;
    
    output.texCoord = input.texCoord;
    
    output.position = mul(float4(input.pos, 0.0f, 1.0f), mat);

	return output;
}