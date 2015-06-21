#ifndef _RENDERING_TASK_HPP_
#define _RENDERING_TASK_HPP_

#include "Parallel/Task.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Renderer;
		class Context;
		class Shader;
	}
	namespace Sandbox
	{
		class RenderingTask : public Parallel::Task
		{
		public:
			RenderingTask(_In_ Graphics::Renderer& RendererObj, _In_ Graphics::Context& ContextObj);
			virtual void DoTask() override;

			void SetRenderTarget(_In_ RenderTarget* RenderTargetObj);

		private:
			Graphics::Renderer& _Renderer;
			Graphics::Context& _Context;
			Graphics::Shader* _VS;
			Graphics::Shader* _GS;
			Graphics::Shader* _PS;
			Graphics::RenderTarget* _RT;
		};
	}
}

#endif
