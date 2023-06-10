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

		float GetPlayerBallPosX() { return x; }
		float GetPlayerBallPosY() { return y; }
		float GetSize() { return hitbox; }
		void IncreaseSize();

	private:
		Mesh* mesh;
		Shader* shader;
		renderer::Vertex vertexes[4] = {};
		ConstantBuffer* cPos;
		ConstantBuffer* cSize;

		float x;
		float y;
		float size;
		float hitbox;

		static PlayerBall* instance;
	};
}


