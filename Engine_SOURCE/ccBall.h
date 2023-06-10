#pragma once

#include "ccGameObject.h"

namespace cc
{
	class Ball : public GameObject
	{
	public:
		Ball();
		virtual ~Ball();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();


	private:
		Mesh* mesh;
		Shader* shader;
		renderer::Vertex vertexes[4] = {};
		ConstantBuffer* constantBuffer;

		float x;
		float y;
	};
}
