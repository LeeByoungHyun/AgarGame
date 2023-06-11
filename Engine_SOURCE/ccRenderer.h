#pragma once
#include "CCEngine.h"
#include "ccGraphicDevice_Dx11.h"
#include "ccMesh.h"
#include "ccShader.h"
#include "ccConstantBuffer.h"

using namespace cc::math;
namespace renderer
{
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};

	struct Info
	{
		Vector4 pos;
		Vector4 color;
		Vector4 scale;
	};

	extern Vertex Vertexes[];
	extern cc::Mesh* mesh;
	extern cc::Shader* shader;
	extern cc::graphics::ConstantBuffer* constantBuffer;

	void Initialize();
	void Release();
}
