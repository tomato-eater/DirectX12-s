cbuffer CameraBuffer : register(b0)
{
    matrix view;
    matrix projection;
}

cbuffer ObjectBuffer : register(b1)
{
    matrix world;
}

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

struct Input
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct Output
{
    float4 svPos : SV_POSITION;
    float2 uv : TEXCOORD;
};

Output vs(Input input)
{
    float4 pos = float4(input.pos.xyz, 1.0f);
    float4 worldPos = mul(pos, world);
    float4 viewPos = mul(worldPos, view);
    
    Output output;
    output.svPos = mul(viewPos, projection);
    output.uv = input.uv;
    return output;
}

float4 ps(Output input) : SV_TARGET
{
    return tex.Sample(smp, input.uv);
}