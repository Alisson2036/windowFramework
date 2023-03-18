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

cbuffer buff
{
    column_major matrix mat;
};


VS_Output main(VS_Input input)
{
    VS_Output output;
    output.position = mul(float4(input.pos, 1.0f), mat);
    output.color = input.color;

    return output;
}