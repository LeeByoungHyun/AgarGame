#pragma once

#include "ccGameObject.h"

namespace cc
{
	class PlayerBall : public GameObject
	{
	public:
		PlayerBall();
		virtual ~PlayerBall();

		static PlayerBall* GetInstance()
		{
			if (instance == nullptr)
				instance = new PlayerBall();

			return instance;
		}

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		renderer::Info GetInfo() { return mInfo; }
		void IncreaseSize();

	private:
		Mesh* mesh;
		Shader* shader;
		renderer::Vertex vertexes[4];
		renderer::Info mInfo;
		ConstantBuffer* cInfo;

		//float x;
		//float y;
		//float size;
		//float hitbox;

		static PlayerBall* instance;
	};
}


