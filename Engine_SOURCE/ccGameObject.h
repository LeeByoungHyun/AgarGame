#pragma once

#include "ccEntity.h"
#include "ccComponent.h"
#include "ccRenderer.h"

namespace cc
{
	using namespace cc::graphics;

	class GameObject : public Entity
	{
	public:
		enum eState
		{
			Active,
			Puase,
			Dead,
		};

		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();


	private:
		eState mState;
		std::vector<Component*> mComponents;
		float x;
		float y;
	};
}


