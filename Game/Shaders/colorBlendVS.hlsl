struct VS_Input
{
    unsigned int instance : SV_InstanceID;
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
    matrix mat;
};
cbuffer buff2 : register(b1)
{
    matrix projectionMat;
};

StructuredBuffer<matrix> instanceMat : register(t0);

VS_Output main(VS_Input input)
{
    VS_Output output;
    
    matrix tMat = instanceMat[input.instance];

    matrix a = mul(projectionMat, tMat);

    output.position = mul(float4(input.pos, 1.0f), transpose(a));

    output.color = input.color;

    return output;
}