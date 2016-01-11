#include "DebugTextTask.hpp"

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

DebugTextTask::DebugTextTask(Renderer& RendererObj, Context& ContextObj)
	: _Renderer(RendererObj)
	, _Context(ContextObj)
{
	_VS = ShaderFactory::Get()->CreateVertexShader("debugtext", "debugtext.vs.hlsl");
	_PS = ShaderFactory::Get()->CreatePixelShader("debugtext", "debugtext.ps.hlsl");
}

DebugTextTask::~DebugTextTask()
{

}

void DebugTextTask::DoTask()
{
	_Context.DrawPrimitive(6);
}
