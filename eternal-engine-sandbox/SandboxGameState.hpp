#ifndef _SANDBOX_GAMESTATE_HPP_
#define _SANDBOX_GAMESTATE_HPP_

#include "Core/GameState.hpp"
#include "Core/GameObject.hpp"

namespace Sandbox
{
	namespace Core
	{
		using namespace Eternal::Core;
		class SandboxGameState : public GameState
		{
		public:
			virtual void Begin() override;
			virtual void Update() override;
			virtual GameState* NextState() override;
			virtual void End() override;

		private:
			GameObject** _Objects;
		};
	}
}

#endif
