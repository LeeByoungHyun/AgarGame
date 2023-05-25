#pragma once
#include "CCEngine.h"
#include "ccGraphicDevice_Dx11.h"

using namespace cc::math;
namespace cc::renderer
{
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};

	extern Vertex vertexes[];
	extern ID3D11Buffer* triangleBuffer;
	extern ID3DBlob* errorBlob;
	extern ID3DBlob* triangleVSBlob;
	extern ID3D11VertexShader* triangleVSShader;
	extern ID3DBlob* trianglePSBlob;
	extern ID3D11PixelShader* trianglePSShader;


	void Initialize();
}
