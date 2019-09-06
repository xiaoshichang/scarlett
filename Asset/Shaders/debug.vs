
struct a2v {
	float3 position : POSITION;
	float4 color	: COLOR;
};

struct v2p {
	float4 position : SV_POSITION;
	float4 color	: COLOR;
};


v2p main(a2v input) {
	v2p output;
	output.position = float4(input.position, 1.0);
	output.color = input.color;

	return output;
}

