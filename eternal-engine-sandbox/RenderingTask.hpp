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
		class RenderTarget;
		class VertexBuffer;
		class IndexBuffer;
		class Constant;
	}
	namespace Components
	{
		class Camera;
		class Light;
	}
	namespace Sandbox
	{
		class RenderingTask : public Parallel::Task
		{
		public:
			RenderingTask(_In_ Graphics::Renderer& RendererObj, _In_ Graphics::Context& ContextObj);
			virtual void DoTask() override;

			void SetVerticesAndIndices(Graphics::VertexBuffer* VerticesBuffer, Graphics::IndexBuffer* IndicesBuffer);
			void SetRenderTarget(_In_ Graphics::RenderTarget* RenderTargetObj);
			void SetCamera(_In_ Components::Camera* CameraObj);
			void SetLights(_In_ Components::Light* Lights);

		private:
			Graphics::Renderer& _Renderer;
			Graphics::Context& _Context;
			Graphics::Shader* _VS;
			Graphics::Shader* _GS;
			Graphics::Shader* _PS;
			Graphics::RenderTarget* _RT;
			Graphics::VertexBuffer* _VerticesBuffer;
			Graphics::IndexBuffer* _IndicesBuffer;

			Graphics::Constant* _LightsConstants;
			Graphics::Constant* _CameraConstant;

			Components::Camera* _Camera;
			Components::Light* _Lights;
		};
	}
}

#endif
