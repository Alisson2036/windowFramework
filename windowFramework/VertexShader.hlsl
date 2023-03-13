float4 main(float2 pos : Position) : SV_Position
{

	return (pos[0], pos[1], 0.0f, 1.0f);
}