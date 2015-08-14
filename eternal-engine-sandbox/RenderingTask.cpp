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
#include "d3d11/D3D11BlendState.hpp"
#include "d3d11/D3D11Viewport.hpp"
#include "Transform/Transform.hpp"

using namespace Eternal;
using namespace Eternal::Sandbox;
using namespace Eternal::Types;

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

	_LightsConstants = new Graphics::D3D11Constant(sizeof(Components::Light) * 8, Graphics::D3D11Resource::IMMUTABLE, Graphics::D3D11Resource::NONE, (void*)&Lights[0]);
	Types::Matrix4x4 CameraMatrix;
	CameraObj->GetProjectionMatrix(CameraMatrix);
	_CameraConstant = new Graphics::D3D11Constant(sizeof(Types::Matrix4x4), Graphics::D3D11Resource::IMMUTABLE, Graphics::D3D11Resource::NONE, (void*)&CameraMatrix);
	_ModelConstant = new Graphics::D3D11Constant(sizeof(Types::Matrix4x4), Graphics::D3D11Resource::DYNAMIC, Graphics::D3D11Resource::WRITE, (void*)&_ContextMatrix);
	_BlendState = new Graphics::D3D11BlendState(Graphics::BlendState::INV_DEST_ALPHA, Graphics::BlendState::DEST_ALPHA, Graphics::BlendState::OP_ADD,
		Graphics::BlendState::INV_DEST_ALPHA, Graphics::BlendState::DEST_ALPHA, Graphics::BlendState::OP_ADD);
	_Viewport = new Graphics::D3D11Viewport(0, 0, 640, 480);
	_ContextMatrix = Types::NewIdentity();
}

RenderingTask::~RenderingTask()
{
	delete _LightsConstants;
	_LightsConstants = nullptr;
	delete _CameraConstant;
	_CameraConstant = nullptr;
}

void RenderingTask::DoTask()
{
	Graphics::RenderTarget* RenderTargets[] = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	};

	_Context.BindShader<Graphics::Context::VERTEX>(_VS);
	_Context.BindShader<Graphics::Context::GEOMETRY>(_GS);
	_Context.BindShader<Graphics::Context::PIXEL>(_PS);

	_Context.BindConstant<Graphics::Context::VERTEX>(0, _LightsConstants);
	_Context.BindConstant<Graphics::Context::VERTEX>(1, _CameraConstant);
	_Context.BindConstant<Graphics::Context::VERTEX>(2, _ModelConstant);
	_Context.BindConstant<Graphics::Context::PIXEL>(0, _LightsConstants);
	_Context.BindConstant<Graphics::Context::PIXEL>(1, _CameraConstant);
	_Context.BindConstant<Graphics::Context::PIXEL>(2, _ModelConstant);

	_Context.SetBlendMode(_BlendState);
	_Context.SetViewport(_Viewport);

	_Context.SetRenderTargets(_RTs, _RTCount);
	_ModelContext.Push(_ContextMatrix);
	_Draw(_Mesh);
	static_cast<Graphics::D3D11Renderer*>(Graphics::Renderer::Get())->Flush();
	_ContextMatrix = _ModelContext.Head();
	_ModelContext.Pop();

	_Context.SetRenderTargets(RenderTargets, ETERNAL_ARRAYSIZE(RenderTargets));

	_Context.UnbindConstant<Graphics::Context::VERTEX>(0);
	_Context.UnbindConstant<Graphics::Context::VERTEX>(1);
	_Context.UnbindConstant<Graphics::Context::VERTEX>(2);
	_Context.UnbindConstant<Graphics::Context::PIXEL>(0);
	_Context.UnbindConstant<Graphics::Context::PIXEL>(1);
	_Context.UnbindConstant<Graphics::Context::PIXEL>(2);

	_Context.UnbindShader<Graphics::Context::VERTEX>();
	_Context.UnbindShader<Graphics::Context::GEOMETRY>();
	_Context.UnbindShader<Graphics::Context::PIXEL>();

	SetFinished();
}

void RenderingTask::SetRenderTargets(_In_ Graphics::RenderTarget** RenderTargets, _In_ uint32_t Count)
{
	_RTs = RenderTargets;
	_RTCount = Count;
}

void RenderingTask::SetMesh(Components::Mesh* MeshObj)
{
	ETERNAL_ASSERT(MeshObj->IsValid());
	_Mesh = MeshObj;
}

void RenderingTask::_Draw(Components::Mesh* MeshObj)
{
	_ContextMatrix *= MeshObj->GetTransform().GetModelMatrix();
	if (MeshObj->IsValidNode())
	{
		Graphics::Resource::LockedResource LockedResourceObj = ((Graphics::D3D11Constant*)_ModelConstant)->Lock(_Context, Graphics::Resource::LOCK_WRITE_DISCARD);
		memcpy(LockedResourceObj.Data, &_ContextMatrix, sizeof(Matrix4x4));
		((Graphics::D3D11Constant*)_ModelConstant)->Unlock(_Context);
		_Context.DrawIndexed(MeshObj->GetVertexBuffer(), MeshObj->GetIndexBuffer());
	}
	for (uint32_t SubMeshIndex = 0; SubMeshIndex < MeshObj->GetSubMeshesCount(); ++SubMeshIndex)
	{
		Components::Mesh& SubMesh = MeshObj->GetSubMesh(SubMeshIndex);
		_ModelContext.Push(_ContextMatrix);
		_Draw(&SubMesh);
		_ContextMatrix = _ModelContext.Head();
		_ModelContext.Pop();
	}
}
