#include "RenderingTask.hpp"

#include "Macros/Macros.hpp"
#include "Graphics/ShaderFactory.hpp"
#include "Graphics/Context.hpp"

using namespace Eternal;
using namespace Eternal::Sandbox;

RenderingTask::RenderingTask(_In_ Graphics::Renderer& RendererObj, _In_ Graphics::Context& ContextObj)
	: _Renderer(RendererObj)
	, _Context(ContextObj)
{
	Graphics::InputLayout::VertexDataType DataType[] = {
		Graphics::InputLayout::POSITION_T,
		Graphics::InputLayout::UV_T
	};
	_VS = Graphics::ShaderFactory::Get()->CreateVertexShader("Default", "default.vs.hlsl", DataType, ETERNAL_ARRAYSIZE(DataType));
	_GS = Graphics::ShaderFactory::Get()->CreateGeometryShader("Default", "default.gs.hlsl");
	_PS = Graphics::ShaderFactory::Get()->CreateGeometryShader("Default", "default.ps.hlsl");

	
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
	_Context.DrawIndexed(_VerticesBuffer, _IndicesBuffer);
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
