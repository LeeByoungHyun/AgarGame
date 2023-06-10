#include "ccPlayerBall.h"
#include "ccInput.h"
#include "ccTime.h"
#include "ccConstantBuffer.h"

namespace cc
{
	using namespace cc::graphics;

	PlayerBall* PlayerBall::instance = nullptr;

	PlayerBall::PlayerBall()
		: x(0.0f)
		, y(0.0f)
		, cPos(nullptr)
		, cSize(nullptr)
		, size(1.0f)
		, hitbox(0.02f)
	{

	}

	PlayerBall::~PlayerBall()
	{

	}

	void PlayerBall::Initialize()
	{
		GameObject::Initialize();

		vertexes[0].pos = Vector3(x - hitbox, y + hitbox, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(x + hitbox, y + hitbox, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(x + hitbox, y - hitbox, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[3].pos = Vector3(x - hitbox, y - hitbox, 0.0f);
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
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");

		cPos = new cc::graphics::ConstantBuffer(eCBType::Transform);
		cPos->Create(sizeof(Vector4));
		cPos->Bind(eShaderStage::VS);

		cSize = new cc::graphics::ConstantBuffer(eCBType::Size);
		cSize->Create(sizeof(Vector4));
		cSize->Bind(eShaderStage::VS);
	}

	void PlayerBall::Update()
	{
		GameObject::Update();

		if (cc::Input::GetKey(cc::eKeyCode::W))
			y += 0.1f * Time::DeltaTime();
		if (cc::Input::GetKey(cc::eKeyCode::S))
			y -= 0.1f * Time::DeltaTime();
		if (cc::Input::GetKey(cc::eKeyCode::A))
			x -= 0.1f * Time::DeltaTime();
		if (cc::Input::GetKey(cc::eKeyCode::D))
			x += 0.1f * Time::DeltaTime();

		Vector4 constancePos = Vector4(x, y, size, 1.0f);
		cPos->SetData(&constancePos);
	}

	void PlayerBall::LateUpdate()
	{
		GameObject::LateUpdate();

	}

	void PlayerBall::Render()
	{
		GameObject::Render();

		mesh->BindBuffer();
		shader->Binds();
		graphics::GetDevice()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}

	void PlayerBall::IncreaseSize()
	{
		size *= 1.2f;
		hitbox *= 1.2f;

		//Vector4 constance = Vector4(size, 0.0f, 0.0f, 0.0f);
		//cSize->SetData(&constance);
	}
}