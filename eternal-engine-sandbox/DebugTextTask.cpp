#include "DebugTextTask.hpp"

#include "Graphics/Device.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Context.hpp"
#include "Graphics/ShaderFactory.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"

#include "Camera/Camera.hpp"
#include "Mesh/GenericMesh.hpp"
#include "Resources/TextureFactory.hpp"

#include "d3d11/D3D11Sampler.hpp"
#include "d3d11/D3D11PosUVVertexBuffer.hpp"
#include "d3d11/D3D11UInt32IndexBuffer.hpp"
#include "d3d11/D3D11Constant.hpp"
#include "d3d11/D3D11Renderer.hpp"
#include "d3d11/D3D11Viewport.hpp"
#include "d3d11/D3D11BlendState.hpp"
#include "d3d11/D3D11DepthStencil.hpp"
#include "d3d11/D3D11DepthStencilBuffer.hpp"
#include "d3d11/D3D11RenderTarget.hpp"
#include "d3d11/D3D11StructuredBuffer.hpp"

using namespace Eternal::Sandbox;
using namespace Eternal::Resources;

DebugTextTask::DebugTextTask(Renderer& RendererObj, Context& ContextObj)
	: _Renderer(RendererObj)
	, _Context(ContextObj)
{
	_VS = ShaderFactory::Get()->CreateVertexShader("debugtext", "debugtext.vs.hlsl");
	_PS = ShaderFactory::Get()->CreatePixelShader("debugtext", "debugtext.ps.hlsl");

	_Viewport = new D3D11Viewport(0, 0, Device::WIDTH, Device::HEIGHT);

	_Sampler = new D3D11Sampler(false, false, false, false, Sampler::WRAP, Sampler::WRAP, Sampler::WRAP);

	_Texture = TextureFactory::Get()->GetTexture("debugfont.tga");

	DebugTextParameters Parameters;
	Parameters.PositionScreenSize = Vector4(5.f, 5.f, 640.f, 480.f);
	Parameters.FontTableSize = Vector4(80.f, 55.f, 0.f, 0.f);

	_DebugTextConstant = new D3D11Constant(sizeof(DebugTextParameters), D3D11Resource::DEFAULT, Resource::NONE, &Parameters);

	_StringBuffer = new D3D11StructuredBuffer(D3D11Resource::DYNAMIC, Resource::WRITE, 4, 256);
}

DebugTextTask::~DebugTextTask()
{
}

void DebugTextTask::Execute()
{
	RenderTarget* BackBuffer = static_cast<D3D11Renderer*>(Renderer::Get())->GetBackBuffer();
	RenderTarget* NullRenderTarget = nullptr;

	D3D11Resource::LockedResource LockedResource = ((D3D11StructuredBuffer*)_StringBuffer)->Lock(_Context, Resource::LOCK_WRITE_DISCARD);
	uint32_t Content[256];
	memset(Content, 0x0, sizeof(uint32_t) * 256);
	memcpy(Content, _Text.c_str(), _Text.length());
	memcpy(LockedResource.Data, Content, sizeof(uint32_t) * 256);
	((D3D11StructuredBuffer*)_StringBuffer)->Unlock(_Context);

	_Context.SetViewport(_Viewport);
	_Context.BindShader<Context::VERTEX>(_VS);
	_Context.BindShader<Context::PIXEL>(_PS);
	_Context.BindBuffer<Context::PIXEL>(0, (D3D11Texture*)_Texture);
	_Context.BindConstant<Context::VERTEX>(0, _DebugTextConstant);
	_Context.BindSampler<Context::PIXEL>(0, _Sampler);
	_Context.BindBuffer<Context::VERTEX>(1, (D3D11StructuredBuffer*)_StringBuffer);
	_Context.SetRenderTargets(&BackBuffer, 1);
	_Context.DrawPrimitive(6*_Text.length());
	static_cast<Graphics::D3D11Renderer*>(Graphics::Renderer::Get())->Flush();
	_Context.SetRenderTargets(&NullRenderTarget, 1);
	_Context.UnbindSampler<Context::PIXEL>(0);
	_Context.UnbindConstant<Context::VERTEX>(0);
	_Context.UnbindBuffer<Context::VERTEX>(1);
	_Context.UnbindBuffer<Context::PIXEL>(0);
	_Context.UnbindShader<Context::VERTEX>();
	_Context.UnbindShader<Context::PIXEL>();

	_Executed = true;
}

bool DebugTextTask::TaskIsExecuted()
{
	return _Executed;
}

void DebugTextTask::Setup()
{
	_Executed = false;
}

void DebugTextTask::SetupText(const std::string& Text)
{
	_Text = Text;
}
