struct a2v {
	float3 position : POSITION;
};

struct v2p {
	float4 position : SV_POSITION;
	float3 texcood	: TEXCOOD;
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
	output.texcood = input.position;
	float4 HPos = float4(input.position, 1.0);
	HPos = mul(HPos, viewMatrix);
    HPos = mul(HPos, projectionMatrix);
	output.position = float4( HPos.x, HPos.y, HPos.w, HPos.w);
	return output;
}
