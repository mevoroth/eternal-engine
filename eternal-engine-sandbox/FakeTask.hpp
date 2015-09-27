#ifndef _FAKE_WORKER_HPP_
#define _FAKE_WORKER_HPP_

#include "Parallel/Task.hpp"

using namespace Eternal::Parallel;

class FakeTask : public Task
{
public:
	FakeTask()
	{
		machin = new int[1000000];
	}
	~FakeTask()
	{
		delete [] machin;
	}

	virtual void DoTask() override
	{
		OutputDebugString("LEL");
		SetFinished();
	}

private:
	int* machin = nullptr;
};

#endif
