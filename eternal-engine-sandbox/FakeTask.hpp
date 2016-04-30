#ifndef _FAKE_WORKER_HPP_
#define _FAKE_WORKER_HPP_

#include "Parallel/Task.hpp"

using namespace Eternal::Parallel;

namespace Eternal
{
	namespace Parallel
	{
		class AtomicInt;
	}
}

class FakeTask : public Task
{
public:
	FakeTask();
	~FakeTask();

	virtual void Setup() override;
	virtual void Execute() override;
	virtual void Reset() override {};

private:
	AtomicInt* _State = nullptr;
};

#endif
