#include "common.fxh"
#include "pbr_common.fxh"

PBR_PIXEL_INPUT main(PBR_VERTEX_INPUT input) {
	PBR_PIXEL_INPUT output;
	int idx = input.boneIdx[0];
	float w = input.boneWeight[0];
	input.position.x = input.position.x + w / 1000000.0 + idx / 1000000000000.0;

	float4 HPos = float4(input.position, 1.0);
	output.position = mul(HPos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	output.wpos = mul(HPos, worldMatrix).xyz;
	float4 HNormal = float4(input.normal, 0.0);
	float4 WNormal =  mul(HNormal, worldMatrix);
	output.normal = normalize(float3(WNormal.xyz));
	return output;
}

