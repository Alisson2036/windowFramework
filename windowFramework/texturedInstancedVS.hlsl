struct VS_Input
{
    float3 pos : Position;
    float2 tex : TexCoord;
    float3 normals : Normals;
    float3 tangent : Tangents;
    float3 instancePos : InstPos;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
    float3 normals : Normals;
    float3 tangents : Tangents;
    float3 vertexPos : Position;
};

cbuffer buff : register(b0)
{
    row_major matrix mat;
};
cbuffer projectionMatrix : register(b1)
{
    row_major matrix projectionMat;
};



VS_Output main(VS_Input input)
{
    VS_Output output;
    
    //matriz final
    //matrix a = mul(projectionMat, mat);
    output.position = mul(float4(input.pos, 1.0f), mat);
    output.position += float4(input.instancePos, 0.0f);
    output.position = mul(output.position, projectionMat);


    float3x3 b = (float3x3) mat;
    output.tex = input.tex;
    output.normals = mul(input.normals, b);
    output.tangents = mul(input.tangent, b);

    output.vertexPos = (float3) mul(float4(input.pos, 1.0f), mat);
    output.vertexPos += input.instancePos;
    

    return output;
}