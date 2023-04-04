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
    matrix mat;
};
cbuffer buff2 : register(b1)
{
    matrix projectionMat;
};

cbuffer light : register(b2)
{
    float3 lightPos;
};


VS_Output main(VS_Input input)
{
    VS_Output output;
    
    //matriz final
    matrix a = mul(projectionMat, mat);
    output.position = mul(float4(input.pos, 1.0f), transpose(a));

    //calculando a posicao da luz
    float3x3 lightMat = mat;
    float3 vertexPos = mul(input.pos, mat);
    

    output.lightNormal = normalize(mul(lightPos,lightMat) - vertexPos);


    output.tex = input.tex;
    output.normals = input.normals;

    return output;
}