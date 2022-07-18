#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);        // 0番スロットに設定されたサンプラー

float gaussian(float2 drawUV, float2 pickUV, float sigma) {
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex0.Sample(smp, input.uv);
    float4 color = texcolor;
   
    return float4(color.rgb, 1);
}