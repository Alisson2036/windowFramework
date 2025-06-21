struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : Color;
};

float4 main(VS_Output input) : SV_TARGET
{
    float4 color = input.color;
    return color;
}