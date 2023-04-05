struct VS_Output
{
    float4 position : SV_POSITION;
    float2 tex : TexCoord;
    float3 normals : Normals;
    float3 vertexPos : Position;
};


cbuffer light : register(b0)
{
    float3 lightPos;
};
cbuffer cam : register(b1)
{
    float3 cameraPos;
};

Texture2D tex : register(t0);
SamplerState samp;

float4 main(VS_Output input) : SV_TARGET
{
    float3 pos = input.vertexPos;
    float3 directionLight = normalize(lightPos - pos);
    float3 cameraVertexDif = cameraPos - input.vertexPos;
    
    //calcula especular 
    float3 refraction = normalize(reflect(cameraVertexDif, input.normals));
    float specular = max(0.0f, dot(refraction, -directionLight) )/2;


    //calcula brilho da face
    float factor = max(0.0f, dot(directionLight, input.normals) );



    return saturate((tex.Sample(samp, input.tex) * (factor+0.2)) + float4(1.0f,1.0f,1.0f,1.0f)*specular);
}