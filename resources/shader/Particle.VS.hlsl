#include "Particle.hlsli"
struct TransformationMatrix
{
    //matrix WVP;
    matrix World;
};
struct ViewProjectionMatrix
{
    matrix view;
    matrix projection;
    float3 cameraPos;
};

StructuredBuffer<TransformationMatrix> gTransformationMatrix : register(t0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    matrix matVP = mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection);
    matrix matWVP = mul(gTransformationMatrix[instanceId].World, matVP);
    output.position = mul(input.position, matWVP);
    output.texCoord = input.texCoord;
    output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrix[instanceId].World));
    return output;
}