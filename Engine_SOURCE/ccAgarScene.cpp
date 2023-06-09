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
		// 여기서 초기 게임 맵데이터를 세팅해줘야 한다.

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