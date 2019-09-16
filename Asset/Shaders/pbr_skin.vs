struct a2v {
	float3 position : POSITION;
	float3 normal	: NORMAL;
};

struct v2p {
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
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
	float4 HNormal = float4(input.normal, 0.0);
	float4 WNormal =  mul(HNormal, worldMatrix);
	output.normal = float3(WNormal.x, WNormal.y, WNormal.z);
	return output;
}

