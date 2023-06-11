#pragma once

#include "ccGameObject.h"

namespace cc
{
	using namespace renderer;
	class Ball : public GameObject
	{
	public:
		Ball();
		virtual ~Ball();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		Info GetInfo() { return mInfo; }

	private:
		Mesh* mesh;
		Shader* shader;
		renderer::Vertex vertexes[4] = {};
		ConstantBuffer* constantBuffer;

		Info mInfo;

	};
}
