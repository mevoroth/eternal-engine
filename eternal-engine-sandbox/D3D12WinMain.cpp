#include "Macros/Macros.hpp"

#include "Types/Types.hpp"
#include "WindowsProcess.hpp"
#include "Window/Window.hpp"
#include "Input/WinInput/WinInput.hpp"

#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandQueue.hpp"
#include "d3d12/D3D12State.hpp"
#include "d3d12/D3D12InputLayout.hpp"
#include "d3d12/D3D12ShaderFactory.hpp"
#include "d3d12/D3D12BlendState.hpp"
#include "d3d12/D3D12RenderTarget.hpp"
#include "d3d12/D3D12CommandList.hpp"
#include "d3d12/D3D12Constant.hpp"
#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"
#include "Graphics/Viewport.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::Input;
using namespace Eternal::Types;

struct DebugTextParameters
{
	Vector4 PositionScreenSize;
	Vector2 FontTableSize;
};

void WINAPI D3D12WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	Window WindowObj(hInstance, nCmdShow, "Eternal Sandbox", "EternalClass", 640, 480);

	WinInput* WinInputObj = new WinInput();
	WindowsProcess WinProcObj;
	WindowsProcess::SetInputHandler(Input::Get());
	WindowObj.Create(WindowsProcess::WindowProc);

	D3D12Device DeviceObj;
	D3D12CommandQueue DirectCommandQueue(DeviceObj);
	D3D12ShaderFactory ShaderFactoryObj;

	InputLayout::VertexDataType VerticesFormat[] = {
		InputLayout::POSITION_T,
		InputLayout::UV_T
	};

	D3D12InputLayout InputLayout(VerticesFormat, ETERNAL_ARRAYSIZE(VerticesFormat));
	Shader* VS = ShaderFactoryObj.CreateVertexShader("debugtext", "debugtext.vs.hlsl");
	Shader* PS = ShaderFactoryObj.CreatePixelShader("debugtext", "debugtext.ps.hlsl");
	DepthTest DepthTestObj;
	StencilTest StencilTestObj;
	BlendState BlendStates[] = {
		D3D12BlendState(BlendState::SRC_ALPHA, BlendState::INV_SRC_ALPHA, BlendState::OP_ADD, BlendState::SRC_ALPHA, BlendState::INV_SRC_ALPHA, BlendState::OP_ADD)
	};
	D3D12RenderTarget RenderTargets[] = {
		D3D12RenderTarget(DeviceObj)
	};
	D3D12State State(DeviceObj, InputLayout, VS, PS, DepthTestObj, StencilTestObj, BlendStates, RenderTargets, ETERNAL_ARRAYSIZE(RenderTargets));
	Viewport ViewportObj(0, 0, 640, 480);

	D3D12Constant DebugTextParametersConstant(DeviceObj, sizeof(DebugTextParameters));
	D3D12CommandList CommandList(DeviceObj, DirectCommandQueue, State);

	CommandList.SetViewport(ViewportObj);
	CommandList.SetScissorRectangle(ViewportObj);
	CommandList.BindConstant(0, DebugTextParametersConstant);
	CommandList.DrawPrimitive(6);
}
