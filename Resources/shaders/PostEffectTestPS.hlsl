#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);   // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);   // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);        // 0番スロットに設定されたサンプラー

float gaussian(float2 drawUV, float2 pickUV, float sigma) {
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input)
{
    PSOutput output;
    float totalweight = 0;
    float sigma = 0.01f;
    float stepwidth = 0.001f;
    float4 texcolor0 = tex0.Sample(smp, input.uv);
    float4 texcolor1 = tex1.Sample(smp, input.uv);

    float4 color = texcolor0;
    for (float py = -sigma * 2; py <= sigma * 2; py += stepwidth) {
        for (float px = -sigma * 2; px <= sigma * 2; px += stepwidth) {
            float2 pickUV = input.uv + float2(px, py);
            float weight = gaussian(input.uv, pickUV, sigma);
            color += tex0.Sample(smp, pickUV) * weight;
            totalweight += weight;
        }
    }
    
    color.rgb = color.rgb / totalweight;
    output.target0 = color;

    return output;
}