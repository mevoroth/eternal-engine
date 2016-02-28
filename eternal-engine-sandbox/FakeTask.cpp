#include "FakeTask.hpp"

#include "Parallel/StdAtomicInt.hpp"

using namespace Eternal::Parallel;

FakeTask::FakeTask()
{
	_State = new StdAtomicInt(0);
}

FakeTask::~FakeTask()
{
	delete _State;
}

bool FakeTask::TaskIsExecuted()
{
	return _State->Load() == 1;
}

void FakeTask::Setup()
{
	_State->Store(0);
}

void FakeTask::Execute()
{
	_State->Store(1);
}
