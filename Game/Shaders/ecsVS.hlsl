struct VS_Input
{
    unsigned int instance : SV_InstanceID;
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

StructuredBuffer<matrix> instanceMat : register(t0);


VS_Output main(VS_Input input)
{
    VS_Output output;
    
    float3 pos = input.pos;
    matrix tMat = instanceMat[input.instance];
    
    //matriz final
    matrix a = mul(projectionMat, tMat);
    matrix c = mul(lightMat, tMat);
    
    output.position = mul(float4(pos, 1.0f), transpose(tMat));
    //output.position += float4(instancesPos[input.instance], 0.0);
    output.position = mul(output.position, transpose(projectionMat));
    
    
    output.shadowPos = mul(float4(pos, 1.0f), transpose(tMat));
    //output.shadowPos += float4(instancesPos[input.instance], 0.0);
    output.shadowPos = mul(output.shadowPos, transpose(lightMat));
    
    //output.shadowPos = mul(float4(pos, 1.0f), transpose(c));
    

    float3x3 b = transpose((float3x3) tMat);
    output.tex = input.tex;
    output.normals = mul(input.normals, b);
    output.tangents = mul(input.tangent, b);

    output.vertexPos = (float3) mul(float4(pos, 1.0f), transpose(mat));
    

    return output;
}