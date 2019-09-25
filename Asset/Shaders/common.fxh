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