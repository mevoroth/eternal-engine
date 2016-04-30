#include "WaterTask.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/Context.hpp"
#include "Graphics/ShaderFactory.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"

#include "Camera/Camera.hpp"
#include "Mesh/GenericMesh.hpp"
#include "d3d11/D3D11PosUVVertexBuffer.hpp"
#include "d3d11/D3D11UInt32IndexBuffer.hpp"
#include "d3d11/D3D11Constant.hpp"
#include "d3d11/D3D11Renderer.hpp"
#include "d3d11/D3D11Viewport.hpp"
#include "d3d11/D3D11BlendState.hpp"
#include "d3d11/D3D11DepthStencil.hpp"
#include "d3d11/D3D11DepthStencilBuffer.hpp"
#include "d3d11/D3D11RenderTarget.hpp"

using namespace Eternal::Sandbox;
using namespace Eternal::Graphics;

WaterTask::WaterTask(Renderer& RendererObj, Context& ContextObj)
	: _Renderer(RendererObj)
	, _Context(ContextObj)
{
	InputLayout::VertexDataType VertexFormat[] = {
		InputLayout::POSITION_T,
		InputLayout::UV_T
	};

	_VS = ShaderFactory::Get()->CreateVertexShader("Water", "water.vs.hlsl", VertexFormat, ETERNAL_ARRAYSIZE(VertexFormat));
	_PS = ShaderFactory::Get()->CreatePixelShader("Water", "water.ps.hlsl");

	_CameraConstant = new D3D11Constant(sizeof(Types::Matrix4x4) * 2, Graphics::D3D11Resource::DYNAMIC, Graphics::D3D11Resource::WRITE);

	//_BackBuffer = RendererObj.GetBackBuffer();
	_BackBuffer = new D3D11RenderTarget(640, 480);

	_Viewport = new D3D11Viewport(0, 0, 640, 480);

	_BlendState = new Graphics::D3D11BlendState(Graphics::BlendState::SRC_ALPHA, Graphics::BlendState::INV_SRC_ALPHA, Graphics::BlendState::OP_ADD,
		Graphics::BlendState::SRC_ALPHA, Graphics::BlendState::INV_SRC_ALPHA, Graphics::BlendState::OP_ADD);
	_DepthStencilState = new Graphics::D3D11DepthStencil(
		Graphics::DepthTest(Graphics::DepthTest::ALL, Graphics::Comparison::LESS),
		Graphics::StencilTest()
	);
	_DepthStencilBuffer = new Graphics::D3D11DepthStencilBuffer(640, 480);
}

WaterTask::~WaterTask()
{
	delete _CameraConstant;
}

void WaterTask::Setup()
{
	_Executed = false;
}

void WaterTask::Execute()
{
	RenderTarget* CleanRenderTargets[] = {
		nullptr
	};

	_BackBuffer->Clear(&_Context);
	_DepthStencilBuffer->Clear(&_Context);

	Resource::LockedResource LockedResourceObj = ((D3D11Constant*)_CameraConstant)->Lock(_Context, Resource::LOCK_WRITE_DISCARD);
	Matrix4x4* CameraMatrix = (Matrix4x4*)LockedResourceObj.Data;
	XMMATRIX ProjMatrix = XMMatrixTranspose(XMLoadFloat4x4(&_ViewMatrix));
	_Camera->GetProjectionMatrix(*CameraMatrix);
	XMStoreFloat4x4(CameraMatrix + 1, ProjMatrix);
	((Graphics::D3D11Constant*)_CameraConstant)->Unlock(_Context);

	_Context.SetBlendMode(_BlendState);
	_Context.SetViewport(_Viewport);

	_Context.BindConstant<Context::VERTEX>(0, _CameraConstant);

	_Context.BindShader<Context::VERTEX>(_VS);
	_Context.BindShader<Context::PIXEL>(_PS);

	static_cast<Graphics::D3D11Context&>(_Context).GetD3D11Context()->OMSetDepthStencilState(_DepthStencilState->GetD3D11DepthStencilState(), 0);
	_Context.SetDepthBuffer(_DepthStencilBuffer);
	_Context.SetRenderTargets(&_BackBuffer, 1);
	_Context.DrawIndexed(_WaterPlane->GetVertexBuffer(), _WaterPlane->GetIndexBuffer());
	static_cast<Graphics::D3D11Renderer*>(Graphics::Renderer::Get())->Flush();
	_Context.SetRenderTargets(CleanRenderTargets, 1);
	_Context.SetDepthBuffer(nullptr);
	static_cast<Graphics::D3D11Context&>(_Context).GetD3D11Context()->OMSetDepthStencilState(nullptr, 0);

	_Context.UnbindShader<Context::VERTEX>();
	_Context.UnbindShader<Context::PIXEL>();

	_Context.UnbindConstant<Context::VERTEX>(0);

	_Executed = true;
}
