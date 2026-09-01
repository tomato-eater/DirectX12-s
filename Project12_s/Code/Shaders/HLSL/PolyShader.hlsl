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
    Output output;
    output.svPos = input.pos;
    output.uv = input.uv;
    return output;
}

float4 ps(Output input) : SV_TARGET
{
    return float4(tex.Sample(smp, input.uv));
}