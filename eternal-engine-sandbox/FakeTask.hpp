#ifndef _FAKE_WORKER_HPP_
#define _FAKE_WORKER_HPP_

#include "Parallel/Task.hpp"

using namespace Eternal::Parallel;

namespace Eternal
{
	namespace Parallel
	{
		class AtomicS32;
	}
}

class FakeTask : public Task
{
public:
	FakeTask();
	virtual ~FakeTask() override;

	virtual void Setup() override;
	virtual void Execute() override;
	virtual void Reset() override;

private:
	AtomicS32* _State = nullptr;
};

#endif
