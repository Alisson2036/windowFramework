struct VS_Input
{
    float3 pos : Position;
    float2 tex : TexCoord;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
};

cbuffer buff
{
    row_major matrix mat;
};


VS_Output main(VS_Input input)
{
    VS_Output output;
    output.position = mul(float4(input.pos, 1.0f), mat);
    output.tex = input.tex;

    return output;
}