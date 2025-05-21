cbuffer CBMatrix : register(b0)
{
    float4x4 World;
    float4x4 LightViewProj;
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
    VS_OUT output;
    float4 worldPos = mul(float4(input.pos, 1), World);
    output.pos = mul(worldPos, LightViewProj);
    return output;
}

float PS(VS_OUT input) : SV_Depth
{
    return input.pos.z / input.pos.w;
}