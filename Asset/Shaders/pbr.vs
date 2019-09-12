
struct a2v {
	float3 position : POSITION;
};

struct v2p {
	float4 position : SV_POSITION;
	float4 color : Color;
};

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

v2p main(a2v input) {
	v2p output;
	float4 HPos = float4(input.position, 1.0);
	output.position = mul(HPos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	output.color = float4(input.position, 1.0);
	return output;
}

