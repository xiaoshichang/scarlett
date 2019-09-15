struct a2v {
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 texcood	: TEXCOOD;
};

struct v2p {
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
	float2 texcood	: TEXCOOD;
};

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	float4 debugColor;
};

v2p main(a2v input) {
	v2p output;
	float4 HPos = float4(input.position, 1.0);
	output.position = mul(HPos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	output.normal = input.normal;
	output.texcood = input.texcood;
	return output;
}

