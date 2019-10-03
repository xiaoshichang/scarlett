#include "common.fxh"
#include "pbr_common.fxh"

PBR_PIXEL_INPUT main(PBR_VERTEX_INPUT input) {
	PBR_PIXEL_INPUT output;
	
	matrix BoneTransform = boneMatrix[input.boneIdx.x] * input.boneWeight.x;
	BoneTransform += boneMatrix[input.boneIdx.y] * input.boneWeight.y;
	BoneTransform += boneMatrix[input.boneIdx.z] * input.boneWeight.z;
	BoneTransform += boneMatrix[input.boneIdx.w] * input.boneWeight.w;

	float4 HPos = float4(input.position, 1.0);
	HPos = mul(HPos, BoneTransform);
	float4 HNormal = float4(input.normal, 0.0);
	HNormal = mul(HNormal, BoneTransform);

	output.position = mul(HPos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	output.wpos = mul(HPos, worldMatrix).xyz;
	float4 WNormal =  mul(HNormal, worldMatrix);
	output.normal = normalize(float3(WNormal.xyz));
	return output;
}

