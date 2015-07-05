#ifndef _RENDERING_TASK_HPP_
#define _RENDERING_TASK_HPP_

#include <vector>
#include "Parallel/Task.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Renderer;
		class Context;
		class Shader;
		class RenderTarget;
		class Constant;
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
			RenderingTask(
				_In_ Graphics::Renderer& RendererObj,
				_In_ Graphics::Context& ContextObj,
				_In_ Components::Camera* CameraObj,
				_In_ const std::vector<Components::Light>& Lights
			);
			~RenderingTask();
			virtual void DoTask() override;

			void SetMesh(Components::Mesh* MeshObj);
			void SetRenderTarget(_In_ Graphics::RenderTarget* RenderTargetObj);

		private:
			void _Draw(Components::Mesh* MeshObj);

			Graphics::Renderer& _Renderer;
			Graphics::Context& _Context;
			Graphics::Shader* _VS;
			Graphics::Shader* _GS;
			Graphics::Shader* _PS;
			Graphics::RenderTarget* _RT;

			Graphics::Constant* _LightsConstants;
			Graphics::Constant* _CameraConstant;

			Components::Camera* _Camera;
			Components::Light* _Lights;
			Components::Mesh* _Mesh;
		};
	}
}

#endif
