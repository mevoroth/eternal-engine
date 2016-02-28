#ifndef _DEBUG_TEXT_TASK_HPP_
#define _DEBUG_TEXT_TASK_HPP_

#include <string>

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
		class Sampler;
		class Texture;
		class StructuredBuffer;
		class D3D11DepthStencil;
		class D3D11DepthStencilBuffer;
	}

	namespace Sandbox
	{
		using namespace Eternal::Parallel;
		using namespace Eternal::Graphics;
		using namespace Eternal::Types;

		class DebugTextTask : public Task
		{
		public:
			DebugTextTask(Renderer& RendererObj, Context& ContextObj);
			virtual ~DebugTextTask();

			virtual bool TaskIsExecuted() override;
			virtual void Setup() override;
			virtual void Execute() override;

			void SetupText(const std::string& Text);

		private:
			Renderer& _Renderer;
			Context& _Context;

			Shader* _VS = nullptr;
			Shader* _PS = nullptr;
			RenderTarget* _BackBuffer = nullptr;

			Viewport* _Viewport = nullptr;
			BlendState* _BlendState = nullptr;
			Constant* _DebugTextConstant = nullptr;
			Sampler* _Sampler = nullptr;
			Texture* _Texture = nullptr;
			StructuredBuffer* _StringBuffer = nullptr;
			D3D11DepthStencil* _DepthStencilState = nullptr;

			std::string _Text;

			bool _Executed = false;

			struct DebugTextParameters
			{
				Vector4 PositionScreenSize;
				Vector4 FontTableSize;
			};
		};
	}
}

#endif
