
struct a2v {
	float3 position : POSITION;
};

struct v2p {
	float4 position : SV_POSITION;
};

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

v2p main(a2v input) {
	v2p output;
	output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	return output;
}

