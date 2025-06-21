struct VS_Input
{
    float3 pos : Position;
    float2 tex : TexCoord;
    float3 normals : Normals;
    float3 tangent : Tangents;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
    float3 normals : Normals;
    float3 tangents : Tangents;
    float3 vertexPos : Position;
    float4 shadowPos : shadowPos;
};

cbuffer buff : register(b0)
{
    matrix mat;
};
cbuffer projectionMatrix : register(b1)
{
    matrix projectionMat;
};
cbuffer lightMatBuf : register(b2)
{
    matrix lightMat;
};



VS_Output main(VS_Input input)
{
    VS_Output output;
    
    //matriz final
    matrix a = mul(projectionMat, mat);
    matrix c = mul(lightMat, mat);
    output.position = mul(float4(input.pos, 1.0f), transpose(a));
    output.shadowPos = mul(float4(input.pos, 1.0f), transpose(c));


    float3x3 b = transpose((float3x3)mat);
    output.tex = input.tex;
    output.normals = mul(input.normals, b);
    output.tangents = mul(input.tangent, b);

    output.vertexPos = (float3)mul(float4(input.pos,1.0f), transpose(mat));
    

    return output;
}