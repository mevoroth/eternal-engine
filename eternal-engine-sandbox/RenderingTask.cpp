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
#include "d3d11/D3D11RenderTarget.hpp"

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
	, _Camera(CameraObj)
{
	Graphics::InputLayout::VertexDataType DataType[] = {
		Graphics::InputLayout::POSITION_T,
		Graphics::InputLayout::UV_T
	};
	_VS = Graphics::ShaderFactory::Get()->CreateVertexShader("Default", "default.vs.hlsl", DataType, ETERNAL_ARRAYSIZE(DataType));
	_GS = Graphics::ShaderFactory::Get()->CreateGeometryShader("Default", "default.gs.hlsl");
	_PS = Graphics::ShaderFactory::Get()->CreatePixelShader("Default", "default.ps.hlsl");

	_DeferredVS = Graphics::ShaderFactory::Get()->CreateVertexShader("Deferred", "deferred.vs.hlsl", DataType, ETERNAL_ARRAYSIZE(DataType));
	_DeferredPS = Graphics::ShaderFactory::Get()->CreatePixelShader("Deferred", "deferred.ps.hlsl");

	_LightsConstants = new Graphics::D3D11Constant(sizeof(Components::Light) * 8, Graphics::D3D11Resource::IMMUTABLE, Graphics::D3D11Resource::NONE, (void*)&Lights[0]);
	Types::Matrix4x4 CameraMatrix;
	CameraObj->GetProjectionMatrix(CameraMatrix);
	_CameraConstant = new Graphics::D3D11Constant(sizeof(Types::Matrix4x4) * 2, Graphics::D3D11Resource::DYNAMIC, Graphics::D3D11Resource::WRITE, (void*)&CameraMatrix);
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
	delete _ModelConstant;
	_ModelConstant = nullptr;
	delete _BlendState;
	_BlendState = nullptr;
	delete _Viewport;
	_Viewport = nullptr;
	delete _VS;
	delete _GS;
	delete _PS;
	delete _DeferredVS;
	delete _DeferredPS;
}

void RenderingTask::DoTask()
{
	static Graphics::D3D11BlendState BlendStateObj(Graphics::BlendState::ZERO, Graphics::BlendState::ONE, Graphics::BlendState::OP_MAX,
		Graphics::BlendState::ZERO, Graphics::BlendState::ONE, Graphics::BlendState::OP_ADD);

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

	_BackBuffer->Clear(&_Context);
	for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < _RTCount; ++RenderTargetIndex)
	{
		_RTs[RenderTargetIndex]->Clear(&_Context);
	}

	Graphics::Resource::LockedResource LockedResourceObj = ((Graphics::D3D11Constant*)_CameraConstant)->Lock(_Context, Graphics::Resource::LOCK_WRITE_DISCARD);
	Matrix4x4* CameraMatrix = (Matrix4x4*)LockedResourceObj.Data;
	XMMATRIX ProjMatrix = XMMatrixTranspose(XMLoadFloat4x4(&_ViewMatrix));
	//XMMATRIX ProjMatrix = XMMatrixIdentity();
	_Camera->GetProjectionMatrix(*CameraMatrix);
	XMStoreFloat4x4(CameraMatrix + 1, ProjMatrix);
	((Graphics::D3D11Constant*)_CameraConstant)->Unlock(_Context);

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

	//_Context.DrawIndexed(_DeferredQuad->GetVertexBuffer(), _DeferredQuad->GetIndexBuffer());
	_ModelContext.Push(_ContextMatrix);
	_Draw(_Mesh);
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

	_Context.BindShader<Graphics::Context::VERTEX>(_DeferredVS);
	_Context.BindShader<Graphics::Context::PIXEL>(_DeferredPS);

	_Context.BindBuffer<Graphics::Context::PIXEL>(0, (Graphics::D3D11RenderTarget*)_RTs[0]);
	_Context.BindBuffer<Graphics::Context::PIXEL>(1, (Graphics::D3D11RenderTarget*)_RTs[1]);
	_Context.BindBuffer<Graphics::Context::PIXEL>(2, (Graphics::D3D11RenderTarget*)_RTs[2]);
	_Context.BindBuffer<Graphics::Context::PIXEL>(3, (Graphics::D3D11RenderTarget*)_RTs[3]);
	_Context.BindBuffer<Graphics::Context::PIXEL>(4, (Graphics::D3D11RenderTarget*)_RTs[4]);
	_Context.BindBuffer<Graphics::Context::PIXEL>(5, (Graphics::D3D11RenderTarget*)_RTs[5]);

	_Context.SetRenderTargets(&_BackBuffer, 1);

	_Context.DrawIndexed(_DeferredQuad->GetVertexBuffer(), _DeferredQuad->GetIndexBuffer());
	static_cast<Graphics::D3D11Renderer*>(Graphics::Renderer::Get())->Flush();

	_Context.UnbindBuffer<Graphics::Context::PIXEL>(0);
	_Context.UnbindBuffer<Graphics::Context::PIXEL>(1);
	_Context.UnbindBuffer<Graphics::Context::PIXEL>(2);
	_Context.UnbindBuffer<Graphics::Context::PIXEL>(3);
	_Context.UnbindBuffer<Graphics::Context::PIXEL>(4);
	_Context.UnbindBuffer<Graphics::Context::PIXEL>(5);

	_Context.SetRenderTargets(RenderTargets, ETERNAL_ARRAYSIZE(RenderTargets));

	_Context.UnbindShader<Graphics::Context::VERTEX>();
	_Context.UnbindShader<Graphics::Context::PIXEL>();

	SetFinished();
}

void RenderingTask::SetRenderTargets(_In_ Graphics::RenderTarget** RenderTargets, _In_ uint32_t Count)
{
	_RTs = RenderTargets;
	_RTCount = Count;
}

void RenderingTask::SetBackBufferRenderTarget(_In_ Graphics::RenderTarget * BackBuffer)
{
	_BackBuffer = BackBuffer;
}

void RenderingTask::SetDeferredQuad(_In_ Components::Mesh * DeferredQuad)
{
	_DeferredQuad = DeferredQuad;
}

void RenderingTask::SetViewMatrix(const Types::Matrix4x4 & ViewMatrix)
{
	_ViewMatrix = ViewMatrix;
}

void RenderingTask::SetMesh(_In_ Components::Mesh* MeshObj)
{
	ETERNAL_ASSERT(MeshObj->IsValid());
	_Mesh = MeshObj;
}

void RenderingTask::_Draw(_In_ Components::Mesh* MeshObj)
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

