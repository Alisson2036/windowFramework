struct VS_Input
{
    float3 pos : Position;
    float2 tex : TexCoord;
    float3 normals : Normals;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
    float3 normals : Normals;
    float3 lightNormal : light;
};

cbuffer buff : register(b0)
{
    row_major matrix mat;
};

cbuffer light : register(b2)
{
    float3 lightPos;
};


VS_Output main(VS_Input input)
{
    VS_Output output;

    output.position = mul(float4(input.pos, 1.0f), mat);
    output.tex = input.tex;
    output.normals = input.normals;
    output.lightNormal = lightPos;

    return output;
}