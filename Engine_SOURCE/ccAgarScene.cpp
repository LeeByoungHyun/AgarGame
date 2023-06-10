#include "ccAgarScene.h"
#include "ccPlayerBall.h"
#include "ccBall.h"

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

		mGameObjects.push_back(PlayerBall::GetInstance());

		//Ball* ball = new Ball();
		mGameObjects.push_back(new Ball());
		mGameObjects.push_back(new Ball());
		mGameObjects.push_back(new Ball());
		mGameObjects.push_back(new Ball());
		mGameObjects.push_back(new Ball());
		mGameObjects.push_back(new Ball());
		mGameObjects.push_back(new Ball());
		mGameObjects.push_back(new Ball());
		mGameObjects.push_back(new Ball());
		mGameObjects.push_back(new Ball());

		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Initialize();
		}

	}

	void AgarScene::Update()
	{
		// 일정 시간이 지날 때 마다 먹을 수 있는 공 생성


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

	void AgarScene::Destroy()
	{
		std::vector<GameObject*> deleteGameObjects = {};

		for (std::vector<GameObject*>::iterator iter = mGameObjects.begin()
			; iter != mGameObjects.end(); )
		{
			if ((*iter)->GetState() == GameObject::eState::Death)
			{
				deleteGameObjects.push_back((*iter));
				iter = mGameObjects.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		for (GameObject* deathObj : deleteGameObjects)
		{
			delete deathObj;
			deathObj = nullptr;
		}
	}
}