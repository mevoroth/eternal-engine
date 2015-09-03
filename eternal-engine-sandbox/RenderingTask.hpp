#ifndef _RENDERING_TASK_HPP_
#define _RENDERING_TASK_HPP_

#include <cstdint>
#include <vector>
#include "Parallel/Task.hpp"
#include "Container/Stack.hpp"
#include "Types/Types.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Renderer;
		class Context;
		class Shader;
		class RenderTarget;
		class Constant;
		class BlendState;
		class Viewport;
	}
	namespace Components
	{
		class Camera;
		class Light;
		class Mesh;
	}
	namespace Sandbox
	{
		class RenderingTask : public Parallel::Task
		{
		public:
			static const int STACK_SIZE = 64;

			RenderingTask(
				_In_ Graphics::Renderer& RendererObj,
				_In_ Graphics::Context& ContextObj,
				_In_ Components::Camera* CameraObj,
				_In_ const std::vector<Components::Light>& Lights
			);
			~RenderingTask();
			virtual void DoTask() override;

			void SetMesh(Components::Mesh* MeshObj);
			void SetRenderTargets(_In_ Graphics::RenderTarget** RenderTargets, _In_ uint32_t Count);
			void SetBackBufferRenderTarget(_In_ Graphics::RenderTarget* BackBuffer);
			void SetDeferredQuad(_In_ Components::Mesh* DeferredQuad);
			void SetViewMatrix(_In_ const Types::Matrix4x4& ViewMatrix);

		private:
			void _Draw(Components::Mesh* MeshObj);

			Graphics::Renderer& _Renderer;
			Graphics::Context& _Context;
			Graphics::Shader* _VS = nullptr;
			Graphics::Shader* _GS = nullptr;
			Graphics::Shader* _PS = nullptr;
			Graphics::Shader* _DeferredVS = nullptr;
			Graphics::Shader* _DeferredPS = nullptr;
			Graphics::RenderTarget** _RTs = nullptr;
			Graphics::RenderTarget* _BackBuffer = nullptr;
			uint32_t _RTCount = 0;
			Graphics::BlendState* _BlendState = nullptr;
			Graphics::Viewport* _Viewport = nullptr;

			Graphics::Constant* _LightsConstants = nullptr;
			Graphics::Constant* _CameraConstant = nullptr;
			Graphics::Constant* _ModelConstant = nullptr;

			Components::Camera* _Camera = nullptr;
			Components::Light* _Lights = nullptr;
			Components::Mesh* _Mesh = nullptr;
			Components::Mesh* _DeferredQuad = nullptr;

			Container::Stack<Types::Matrix4x4, STACK_SIZE> _ModelContext;
			Types::Matrix4x4 _ContextMatrix;

			Types::Matrix4x4 _ViewMatrix;
		};
	}
}

#endif
