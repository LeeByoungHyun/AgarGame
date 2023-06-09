#include "ccGameObject.h"
#include "ccRenderer.h"
#include "ccGraphicDevice_Dx11.h"

namespace cc
{
	GameObject::GameObject()
		: mState(eState::Active)
	{
		
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Initialize()
	{
		// ���⼭ ���� ����
	}

	void GameObject::Update()
	{
		// �� ������ ����



		// ����

	}

	void GameObject::LateUpdate()
	{

	}

	void GameObject::Render()
	{
		//������۷� ��ġ���� ũ������, ����, ������Ʈ ������Ѵ�.

		renderer::mesh->BindBuffer();
		renderer::shader->Binds();
		graphics::GetDevice()->DrawIndexed(renderer::mesh->GetIndexCount(), 0, 0);
	}
}
