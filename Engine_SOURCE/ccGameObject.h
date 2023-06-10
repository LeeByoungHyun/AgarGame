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
			Death,
		};

		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		eState GetState() { return mState; }
		void SetState(eState state) { mState = state; }

	private:
		eState mState;
		std::vector<Component*> mComponents;
		float x;
		float y;
	};
}


