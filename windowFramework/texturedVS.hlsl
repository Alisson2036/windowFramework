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
    float3 vertexPos : Position;
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
    
    //matriz final
    matrix a = mul(projectionMat, mat);
    output.position = mul(float4(input.pos, 1.0f), transpose(a));


    float3x3 b = transpose(mat);
    output.tex = input.tex;
    output.normals = mul(input.normals, b);

    output.vertexPos = mul(float4(input.pos,1.0f), transpose(mat));
    

    return output;
}