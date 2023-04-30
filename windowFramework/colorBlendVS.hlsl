struct VS_Input
{
    float3 pos : Position;
    float4 color : Coloar;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : Color;
};

cbuffer buff : register(b0)
{
    matrix mat;
};
cbuffer buff2 : register(b1)
{
    matrix projectionMat;
};


VS_Output main(VS_Input input)
{
    VS_Output output;

    matrix a = mul(projectionMat, mat);

    output.position = mul(float4(input.pos, 1.0f), transpose(a));

    output.color = input.color;

    return output;
}