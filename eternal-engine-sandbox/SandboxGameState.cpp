#include "SandboxGameState.hpp"

using namespace Sandbox::Core;

void SandboxGameState::Begin()
{
	_Objects = new GameObject*[1];
	//_Objects[0] = 
}

void SandboxGameState::Update()
{

}

void SandboxGameState::End()
{

}

GameState* SandboxGameState::NextState()
{
	return this;
}
