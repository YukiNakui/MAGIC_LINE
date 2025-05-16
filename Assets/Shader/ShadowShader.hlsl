cbuffer CBMatrix : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Proj;
    float4x4 LightViewProj;
};

struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float4 shadowPos : TEXCOORD2;
};

VS_OUT VS(VS_IN input)
{
    VS_OUT output;
    float4 worldPos = mul(float4(input.pos, 1), World);
    output.worldPos = worldPos.xyz;
    output.normal = mul(float4(input.normal, 0), World).xyz;
    float4 viewPos = mul(worldPos, View);
    output.pos = mul(viewPos, Proj);
    output.shadowPos = mul(worldPos, LightViewProj);
    return output;
}