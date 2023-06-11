
//struct Vertex[3]
//{
//    Vector3 pos;
//    Vector4 color;
//};

struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
};

cbuffer Transform : register(b0)
{
    float4 Pos;
    float4 Color;
    float4 Scale;
}

VSOut main(VSIn In)
{
    VSOut Out = (VSOut)0.0f;

    Out.Pos = float4(In.Pos, 1.0f);
    
    Out.Pos.x *= Scale.x;
    Out.Pos.y *= Scale.y;
    
    Out.Pos.x += Pos.x;
    Out.Pos.y += Pos.y;
    
    Out.Color = In.Color;
    
    return Out;
}