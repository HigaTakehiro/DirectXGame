#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input)
{
	PSOutput output;
	float4 texcolor = tex.Sample(smp, input.uv);
	//Lambert反射
	float3 light = normalize(float3(-1, 1, 0.5));
	float diffuse = saturate(dot(light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//ADS合成
    float4 color = texcolor;
    float4 ambient = float4(color.x * 0.5f, color.y * 0.5f, color.z * 0.5f, color.w);
    float3 eyedir = normalize(camerapos.xyz - input.worldpos.xyz);
    float3 halfvec = normalize(light + eyedir);
    float intensity = saturate(dot(normalize(input.normal), halfvec));
    float reflect = pow(intensity, 50);
    float4 lightcolor = (1, 1, 1, 1);
    float4 specular = reflect * lightcolor;
	//陰影とテクスチャの色を合成
	output.target0 = shadecolor * texcolor;
    diffuse = color * reflect;
    output.target1 = ambient + diffuse + specular;

	return output;
}