#include "RenderingTask.hpp"

#include "Macros/Macros.hpp"
#include "Graphics/ShaderFactory.hpp"
#include "Graphics/Context.hpp"
#include "d3d11/D3D11Constant.hpp"
#include "Light/Light.hpp"
#include "Camera/Camera.hpp"
#include "Types/Types.hpp"
#include "Mesh/Mesh.hpp"
#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal;
using namespace Eternal::Sandbox;

RenderingTask::RenderingTask(
	_In_ Graphics::Renderer& RendererObj,
	_In_ Graphics::Context& ContextObj,
	_In_ Components::Camera* CameraObj,
	_In_ const std::vector<Components::Light>& Lights
)
	: _Renderer(RendererObj)
	, _Context(ContextObj)
{
	Graphics::InputLayout::VertexDataType DataType[] = {
		Graphics::InputLayout::POSITION_T,
		Graphics::InputLayout::UV_T
	};
	_VS = Graphics::ShaderFactory::Get()->CreateVertexShader("Default", "default.vs.hlsl", DataType, ETERNAL_ARRAYSIZE(DataType));
	_GS = Graphics::ShaderFactory::Get()->CreateGeometryShader("Default", "default.gs.hlsl");
	_PS = Graphics::ShaderFactory::Get()->CreatePixelShader("Default", "default.ps.hlsl");

	_LightsConstants = new Graphics::D3D11Constant(sizeof(Components::Light) * Lights.size(), Graphics::D3D11Resource::IMMUTABLE, Graphics::D3D11Resource::NONE, (void*)&Lights[0]);
	Types::Matrix4x4 CameraMatrix;
	CameraObj->GetProjectionMatrix(&CameraMatrix);
	_CameraConstant = new Graphics::D3D11Constant(sizeof(Types::Matrix4x4), Graphics::D3D11Resource::IMMUTABLE, Graphics::D3D11Resource::NONE, (void*)&CameraMatrix);
}

RenderingTask::~RenderingTask()
{
	delete _VS;
	_VS = nullptr;
	delete _GS;
	_GS = nullptr;
	delete _PS;
	_PS = nullptr;
	delete _LightsConstants;
	_LightsConstants = nullptr;
	delete _CameraConstant;
	_CameraConstant = nullptr;
}

void RenderingTask::DoTask()
{
	Graphics::RenderTarget* RenderTargets[] = {
		_RT
	};

	_Context.BindShader<Graphics::Context::VERTEX>(_VS);
	_Context.BindShader<Graphics::Context::GEOMETRY>(_GS);
	_Context.BindShader<Graphics::Context::PIXEL>(_PS);

	_Context.BindConstant<Graphics::Context::VERTEX>(0, _LightsConstants);
	_Context.BindConstant<Graphics::Context::VERTEX>(1, _CameraConstant);
	_Context.BindConstant<Graphics::Context::PIXEL>(0, _LightsConstants);
	_Context.BindConstant<Graphics::Context::PIXEL>(1, _CameraConstant);

	_Context.SetRenderTargets(RenderTargets, ETERNAL_ARRAYSIZE(RenderTargets));
	_Draw(_Mesh);
	static_cast<Graphics::D3D11Renderer*>(Graphics::Renderer::Get())->Flush();
	//_Context.DrawIndexed(_Mesh->GetVertexBuffer(), _Mesh->GetIndexBuffer());
	RenderTargets[0] = nullptr;
	_Context.SetRenderTargets(RenderTargets, ETERNAL_ARRAYSIZE(RenderTargets));

	_Context.UnbindConstant<Graphics::Context::VERTEX>(0);
	_Context.UnbindConstant<Graphics::Context::VERTEX>(1);
	_Context.UnbindConstant<Graphics::Context::PIXEL>(0);
	_Context.UnbindConstant<Graphics::Context::PIXEL>(1);

	_Context.UnbindShader<Graphics::Context::VERTEX>();
	_Context.UnbindShader<Graphics::Context::GEOMETRY>();
	_Context.UnbindShader<Graphics::Context::PIXEL>();
}

void RenderingTask::SetRenderTarget(_In_ Graphics::RenderTarget* RenderTargetObj)
{
	_RT = RenderTargetObj;
}

void RenderingTask::SetMesh(Components::Mesh* MeshObj)
{
	ETERNAL_ASSERT(MeshObj->IsValid());
	_Mesh = MeshObj;
}

void RenderingTask::_Draw(Components::Mesh* MeshObj)
{
	if (MeshObj->IsValidNode())
	{
		_Context.DrawIndexed(MeshObj->GetVertexBuffer(), MeshObj->GetIndexBuffer());
	}
	for (uint32_t SubMeshIndex = 0; SubMeshIndex < MeshObj->GetSubMeshesCount(); ++SubMeshIndex)
	{
		Components::Mesh& SubMesh = MeshObj->GetSubMesh(SubMeshIndex);
		_Draw(&SubMesh);
	}
}
