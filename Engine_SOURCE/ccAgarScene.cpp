#include "ccAgarScene.h"
#include "ccPlayerBall.h"

namespace cc
{
	AgarScene::AgarScene()
	{

	}

	AgarScene::~AgarScene()
	{

	}

	void AgarScene::Initialize()
	{
		// ���⼭ �ʱ� ���� �ʵ����͸� ��������� �Ѵ�.

		PlayerBall* mPlayer = new PlayerBall();
		mGameObjects.push_back(mPlayer);

		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Initialize();
		}

	}

	void AgarScene::Update()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Update();
		}
	}

	void AgarScene::LateUpdate()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->LateUpdate();
		}
	}

	void AgarScene::Render()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Render();
		}
	}
}