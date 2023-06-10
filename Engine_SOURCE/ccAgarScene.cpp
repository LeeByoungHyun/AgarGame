#include "ccAgarScene.h"
#include "ccPlayerBall.h"
#include "ccBall.h"
#include "ccTime.h"

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
		// ���� �ð��� ���� �� ���� ���� �� �ִ� �� ����
		mTime += Time::DeltaTime();
		if (mTime >= 2.0f) 
		{
			Ball* ball = new Ball();
			ball->Initialize();

			mGameObjects.push_back(ball);
			mTime = 0.0f;
		}

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