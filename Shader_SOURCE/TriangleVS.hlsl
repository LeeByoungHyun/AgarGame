
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

}

cbuffer Size : register(b1)
{
    float4 Size;
}

VSOut main(VSIn In)
{
    VSOut Out = (VSOut)0.0f;

    Out.Pos = float4(In.Pos, 1.0f);
    Out.Pos.x += Pos.x;
    Out.Pos.y += Pos.y;
    Out.Color = In.Color;
    
    //Out.Pos.x *= Size.x;
    //Out.Pos.y *= Size.x;
    Out.Pos.x *= Pos.z;
    Out.Pos.y *= Pos.z;
    
    return Out;
}