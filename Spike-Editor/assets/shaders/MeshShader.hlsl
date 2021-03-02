#type vertex
#pragma pack_matrix(row_major)

cbuffer Camera : register(b0)
{
    matrix u_ViewProjection;
}

cbuffer Mesh : register(b1)
{
    matrix u_Transform;
}

struct vsIn
{
    float3 v_Position : M_POSITION;
    float3 v_Normal : M_NORMAL;
    float2 v_TexCoord : M_TEXCOORD;
};

struct vsOut
{
    float4 v_Position : SV_POSITION;
    float3 v_Normal : M_NORMAL;
    float2 v_TexCoord : M_TEXCOORD;
};

vsOut main(vsIn input)
{
    vsOut output;
    output.v_Position = mul(float4(input.v_Position, 1.0f), u_Transform);
    output.v_Position = mul(output.v_Position, u_ViewProjection);

    output.v_Normal = input.v_Normal;
    output.v_TexCoord = input.v_TexCoord;
    return output;
}

#type pixel

struct vsOut
{
    float4 v_Position : SV_POSITION;
    float3 v_Normal : M_NORMAL;
    float2 v_TexCoord : M_TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState sampleType;

float4 main(vsOut input) : SV_TARGET
{
    return tex.Sample(sampleType, input.v_TexCoord);
    //return float4(input.v_TexCoord.x, input.v_TexCoord.y, 0.2f, 1.0f);

}