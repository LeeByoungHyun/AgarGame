#pragma once

#include "ccScene.h"

namespace cc
{
	class AgarScene : public Scene
	{
	public:
		AgarScene();
		~AgarScene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Destroy();

	private:
		std::vector<GameObject*> mGameObjects;

	};
}


