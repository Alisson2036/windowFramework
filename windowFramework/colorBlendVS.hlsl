struct VS_Input
{
    float3 pos : Position;
    float4 color : Color;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : Color;
};

cbuffer buff : register(b0)
{
    row_major matrix mat;
};
cbuffer buff2 : register(b1)
{
    row_major matrix projectionMatrix;
};


VS_Output main(VS_Input input)
{
    VS_Output output;
    output.position = mul(float4(input.pos, 1.0f), mat);
    output.color = input.color;

    return output;
}