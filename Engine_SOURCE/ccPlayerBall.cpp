#include "ccPlayerBall.h"
#include "ccInput.h"
#include "ccTime.h"
#include "ccConstantBuffer.h"

namespace cc
{
	using namespace cc::graphics;

	PlayerBall* PlayerBall::instance = nullptr;

	PlayerBall::PlayerBall()
	{

	}

	PlayerBall::~PlayerBall()
	{

	}

	void PlayerBall::Initialize()
	{
		GameObject::Initialize();

		mInfo.pos = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		mInfo.color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		mInfo.scale = Vector4(0.5f, 0.5f, 0.0f, 0.0f);

		vertexes[0].pos = Vector3(-0.05f, 0.1f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(0.05f, 0.1f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(0.05f, -0.1f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.05f, -0.1f, 0.0f);
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

		cInfo = new cc::graphics::ConstantBuffer(eCBType::Transform);
		cInfo->Create(sizeof(renderer::Info));
		cInfo->Bind(eShaderStage::VS);
		cInfo->SetData(&mInfo);
	}

	void PlayerBall::Update()
	{
		GameObject::Update();

		if (cc::Input::GetKey(cc::eKeyCode::W))
			mInfo.pos.y += 0.3f * Time::DeltaTime();
		if (cc::Input::GetKey(cc::eKeyCode::S))
			mInfo.pos.y -= 0.3f * Time::DeltaTime();
		if (cc::Input::GetKey(cc::eKeyCode::A))
			mInfo.pos.x -= 0.3f * Time::DeltaTime();
		if (cc::Input::GetKey(cc::eKeyCode::D))
			mInfo.pos.x += 0.3f * Time::DeltaTime();

		//Vector4 constancePos = Vector4(mInfo.pos.x, mInfo.pos.y, mInfo.scale, 1.0f);
		cInfo->SetData(&mInfo);
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
		mInfo.scale *= 1.05f;
		cInfo->SetData(&mInfo);
	}
}