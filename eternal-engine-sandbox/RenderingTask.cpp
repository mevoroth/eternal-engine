#include "RenderingTask.hpp"

#include "Macros/Macros.hpp"
#include "Graphics/ShaderFactory.hpp"
#include "Graphics/Context.hpp"

using namespace Eternal;
using namespace Eternal::Sandbox;

RenderingTask::RenderingTask(Graphics::Renderer& RendererObj, Graphics::Context& ContextObj)
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
	_Context.BindShader<Graphics::Context::VERTEX>(_VS);
	_Context.BindShader<Graphics::Context::GEOMETRY>(_GS);
	_Context.BindShader<Graphics::Context::PIXEL>(_PS);



	_Context.UnbindShader<Graphics::Context::VERTEX>();
	_Context.UnbindShader<Graphics::Context::GEOMETRY>();
	_Context.UnbindShader<Graphics::Context::PIXEL>();
}
