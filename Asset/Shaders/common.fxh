#define PI 3.1415926

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	float4 debugColor;
	float4 pbrParameter;
	float4 camPos;
};

cbuffer ConstantBufferLighting
{
	float4 sunLightDir;
	float4 sunLightColor;

	float4 pointLightDir[4];
	float4 pointLightColor[4];
};

cbuffer ConstantBufferAnimation{
	matrix boneMatrix[32];
};