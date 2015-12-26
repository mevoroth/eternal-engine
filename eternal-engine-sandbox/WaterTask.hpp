#ifndef _WATER_TASK_HPP_
#define _WATER_TASK_HPP_

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
	}

	namespace Components
	{
		class Mesh;
		class Camera;
	}

	using namespace Eternal::Parallel;
	using namespace Eternal::Graphics;
	using namespace Eternal::Components;
	using namespace Eternal::Types;
	
	namespace Sandbox
	{
		class WaterTask : public Task
		{
		public:
			WaterTask(Renderer& RendererObj, Context& ContextObj);
			virtual void DoTask() override;
			void SetCamera(Camera* CameraObj)
			{
				_Camera = CameraObj;
			}
			void SetViewMatrix(const Matrix4x4& ViewMatrix)
			{
				_ViewMatrix = ViewMatrix;
			}

		private:
			Renderer& _Renderer;
			Context& _Context;

			Shader* _VS = nullptr;
			Shader* _PS = nullptr;

			Constant* _CameraConstant = nullptr;
			Mesh* _WaterPlane = nullptr;
			RenderTarget* _RenderTarget = nullptr;

			Camera* _Camera = nullptr;
			Matrix4x4 _ViewMatrix;
		};
	}
}

#endif
