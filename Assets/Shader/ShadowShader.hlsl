cbuffer ShadowMapCB : register(b0)
{
    float4x4 g_matWorld;
    float4x4 g_matLightViewProj;
};
struct VS_IN
{
    float3 pos : POSITION;
};
struct VS_OUT
{
    float4 pos : SV_POSITION;
};
VS_OUT VS(VS_IN input)
{
    VS_OUT outData;
    float4 worldPos = mul(float4(input.pos, 1), g_matWorld);
    outData.pos = mul(worldPos, g_matLightViewProj);
    return outData;
}
float4 PS(VS_OUT input) : SV_Target
{
    return float4(1, 1, 1, 1); // é¿ç€ÇÕégÇÌÇÍÇ»Ç¢
}