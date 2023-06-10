#include "ccBall.h"
#include "ccInput.h"
#include "ccTime.h"
#include "ccConstantBuffer.h"
#include "ccPlayerBall.h"

namespace cc
{
	using namespace cc::graphics;

	Ball::Ball()
		: x(0.0f)
		, y(0.0f)
		, constantBuffer(nullptr)
	{

	}

	Ball::~Ball()
	{

	}

	void Ball::Initialize()
	{
		GameObject::Initialize();

		//srand((unsigned int)time(nullptr));
		 x = -1.0f + (std::rand()) / (static_cast<float>(RAND_MAX / 2));
		 y = -1.0f + (std::rand()) / (static_cast<float>(RAND_MAX / 2));

		vertexes[0].pos = Vector3(x - 0.01f, y + 0.01f, 0.0f);
		vertexes[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(x + 0.01f, y + 0.01f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(x + 0.01f, y - 0.01f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[3].pos = Vector3(x - 0.01f, y - 0.01f, 0.0f);
		vertexes[3].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

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

		if (this->x - PlayerBall::GetInstance()->GetSize() - 0.01f <= PlayerBall::GetInstance()->GetPlayerBallPosX()
			&& this->x + PlayerBall::GetInstance()->GetSize() + 0.01f >= PlayerBall::GetInstance()->GetPlayerBallPosX()
			&& this->y - PlayerBall::GetInstance()->GetSize() - 0.01f <= PlayerBall::GetInstance()->GetPlayerBallPosY()
			&& this->y + PlayerBall::GetInstance()->GetSize() + 0.01f >= PlayerBall::GetInstance()->GetPlayerBallPosY())
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