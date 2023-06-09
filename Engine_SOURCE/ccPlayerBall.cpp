#include "ccPlayerBall.h"
#include "ccInput.h"
#include "ccTime.h"
#include "ccConstantBuffer.h"

namespace cc
{
	using namespace cc::graphics;

	PlayerBall::PlayerBall()
		: x(0.0f)
		, y(0.0f)
		, pos(0.0f, 0.0f, 0.0f, 1.0f)
	{

	}

	PlayerBall::~PlayerBall()
	{

	}

	void PlayerBall::Initialize()
	{
		GameObject::Initialize();

		vertexes[0].pos = Vector3(x - 0.1f, y + 0.1f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(x +0.1f, y + 0.1f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(x + 0.1f, y - 0.1f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[3].pos = Vector3(x - 0.1f, y - 0.1f, 0.0f);
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
	}

	void PlayerBall::Update()
	{
		GameObject::Update();

		if (cc::Input::GetKey(cc::eKeyCode::W))
			pos += Vector4(0.0f, 0.1f * cc::Time::DeltaTime(), 0.0f, 1.0f);
		if (cc::Input::GetKey(cc::eKeyCode::S))
			pos += Vector4(0.0f, -0.1f * cc::Time::DeltaTime(), 0.0f, 1.0f);
		if (cc::Input::GetKey(cc::eKeyCode::A))
			pos += Vector4(-0.1f * cc::Time::DeltaTime(), 0.0f, 0.0f, 1.0f);
		if (cc::Input::GetKey(cc::eKeyCode::D))
			pos += Vector4(0.1f * cc::Time::DeltaTime(), 0.0f, 0.0f, 1.0f);

		renderer::constantBuffer->SetData(&pos);
		renderer::constantBuffer->Bind(eShaderStage::VS);
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
		graphics::GetDevice()->DrawIndexed(renderer::mesh->GetIndexCount(), 0, 0);
	}
}