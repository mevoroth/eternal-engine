#include "FakeTask.hpp"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

#include "Parallel/StdAtomicS32.hpp"

using namespace Eternal::Parallel;

FakeTask::FakeTask()
{
	_State = new StdAtomicS32(0);
}

FakeTask::~FakeTask()
{
	delete _State;
}

void FakeTask::Setup()
{
	SetState(SETTINGUP);
	_State->Store(0);
	SetState(SETUP);
}

void FakeTask::Execute()
{
	SetState(EXECUTING);
	_State->Store(1);

	OutputDebugString("YOLO\n");
	SetState(DONE);
}

void FakeTask::Reset()
{
	SetState(IDLE);
}
