#ifndef _DEBUG_TEXT_TASK_HPP_
#define _DEBUG_TEXT_TASK_HPP_

#include "Parallel/Task.hpp"
#include "Types/Types.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Renderer;
		class Context;
		class Shader;
		class Constant;
		class RenderTarget;
		class Viewport;
		class BlendState;
		class D3D11DepthStencil;
		class D3D11DepthStencilBuffer;
	}

	namespace Sandbox
	{
		using namespace Eternal::Parallel;
		using namespace Eternal::Graphics;

		class DebugTextTask : public Task
		{
		public:
			DebugTextTask(Renderer& RendererObj, Context& ContextObj);
			virtual ~DebugTextTask();
			virtual void DoTask() override;

		private:
			Renderer& _Renderer;
			Context& _Context;

			Shader* _VS = nullptr;
			Shader* _PS = nullptr;
			RenderTarget* _BackBuffer = nullptr;

			Viewport* _Viewport = nullptr;
			BlendState* _BlendState = nullptr;
			Constant* _DebugTextConstant = nullptr;
			D3D11DepthStencil* _DepthStencilState = nullptr;;
		};
	}
}

#endif
