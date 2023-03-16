struct VS_Input
{
    float2 pos : Position;
    float4 color : Color;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : Color;
};

cbuffer buff
{
    matrix mat;
};


VS_Output main(VS_Input input)
{
    VS_Output output;
    output.position = mul(float4(input.pos, 0.0f, 1.0f), mat);
    output.color = input.color;

    return output;
}