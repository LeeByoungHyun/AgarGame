#include "ccBall.h"
#include "ccInput.h"
#include "ccTime.h"
#include "ccConstantBuffer.h"
#include "ccPlayerBall.h"

namespace cc
{
	using namespace cc::graphics;

	Ball::Ball()
	{

	}

	Ball::~Ball()
	{

	}

	void Ball::Initialize()
	{
		GameObject::Initialize();

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> pos(-1.0f, 1.0f);
		std::uniform_real_distribution<float> color(0.0f, 1.0f);

		mInfo.pos = Vector4(pos(gen), pos(gen), 0.0f, 1.0f);
		mInfo.color = Vector4(color(gen), color(gen), color(gen), 1.0f);
		mInfo.scale = Vector4(0.5f, 0.5f, 0.0f, 0.0f);

		vertexes[0].pos = Vector3(mInfo.pos.x - 0.01f, mInfo.pos.y + 0.02f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(mInfo.pos.x + 0.01f, mInfo.pos.y + 0.02f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(mInfo.pos.x + 0.01f, mInfo.pos.y - 0.02f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[3].pos = Vector3(mInfo.pos.x - 0.01f, mInfo.pos.y - 0.02f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		// Vertex Buffer
		mesh = new cc::Mesh();
		mesh->CreateVertexBuffer(vertexes, 4);

		std::vector<UINT> indexes = {};
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);
		mesh->CreateIndexBuffer(indexes.data(), indexes.size());

		shader = new cc::Shader();
		shader->Create(eShaderStage::VS, L"RectangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"RectanglePS.hlsl", "main");
	}

	void Ball::Update()
	{
		GameObject::Update();

		/*
		if (this->x - PlayerBall::GetInstance()->GetSize() - 0.01f <= PlayerBall::GetInstance()->GetPlayerBallPosX()
			&& this->x + PlayerBall::GetInstance()->GetSize() + 0.01f >= PlayerBall::GetInstance()->GetPlayerBallPosX()
			&& this->y - PlayerBall::GetInstance()->GetSize() - 0.01f <= PlayerBall::GetInstance()->GetPlayerBallPosY()
			&& this->y + PlayerBall::GetInstance()->GetSize() + 0.01f >= PlayerBall::GetInstance()->GetPlayerBallPosY())
		{
			PlayerBall::GetInstance()->IncreaseSize();
			this->SetState(Death);
		}
		*/

		Vector2 PlayerSize = Vector2(0.05 * PlayerBall::GetInstance()->GetInfo().scale.x, 0.1 * PlayerBall::GetInstance()->GetInfo().scale.y);
		Vector2 MonsterSize = Vector2(0.05 * (mInfo.scale.x), 0.1 * (mInfo.scale.y));

		if (fabs(PlayerBall::GetInstance()->GetInfo().pos.x - mInfo.pos.x) < (PlayerSize.x + MonsterSize.x)
			&& fabs(PlayerBall::GetInstance()->GetInfo().pos.y - mInfo.pos.y) < (PlayerSize.y + MonsterSize.y))
		{
			PlayerBall::GetInstance()->IncreaseSize();
			this->SetState(Death);
		}
	}

	void Ball::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void Ball::Render()
	{
		GameObject::Render();

		mesh->BindBuffer();
		shader->Binds();
		graphics::GetDevice()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}
}