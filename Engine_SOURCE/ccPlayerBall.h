#pragma once

#include "ccGameObject.h"

namespace cc
{
	class PlayerBall : public GameObject
	{
	public:
		PlayerBall();
		virtual ~PlayerBall();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();


	private:
		Mesh* mesh;
		Shader* shader;
		renderer::Vertex vertexes[4] = {};
		Vector4 pos;
		float x;
		float y;
	};
}


