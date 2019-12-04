#include "common.fxh"

struct TERRAIN_VERTEX_INPUT {
	float3 position : POSITION;
	float2 texcood	: TEXCOOD;
};

struct TERRAIN_PIXEL_INPUT {
	float4 position : SV_POSITION;
	float2 texcood	: TEXCOOD;
};

TERRAIN_PIXEL_INPUT main(TERRAIN_VERTEX_INPUT input) {
	TERRAIN_PIXEL_INPUT output;
	float4 HPos = float4(input.position, 1.0);
	output.position = mul(HPos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	output.texcood = input.texcood;
	return output;
}