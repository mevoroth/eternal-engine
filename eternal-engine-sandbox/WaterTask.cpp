#include "WaterTask.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/Context.hpp"
#include "Graphics/ShaderFactory.hpp"

#include "Camera/Camera.hpp"
#include "Mesh/GenericMesh.hpp"
#include "d3d11/D3D11PosUVVertexBuffer.hpp"
#include "d3d11/D3D11UInt32IndexBuffer.hpp"
#include "d3d11/D3D11Constant.hpp"

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

	GenericMesh<D3D11PosUVVertexBuffer::PosUVVertex, D3D11PosUVVertexBuffer, D3D11UInt32IndexBuffer>* Plane = new GenericMesh<D3D11PosUVVertexBuffer::PosUVVertex, D3D11PosUVVertexBuffer, D3D11UInt32IndexBuffer>();
	D3D11PosUVVertexBuffer::PosUVVertex PlaneVertices[] = {
		{ Vector4(-1000.f, -1000.f, 0.f, 1.f), Vector2(0.f, 0.f) },
		{ Vector4(-1000.f, 1000.f, 0.f, 1.f), Vector2(0.f, 1.f) },
		{ Vector4(1000.f, 1000.f, 0.f, 1.f), Vector2(1.f, 1.f) },
		{ Vector4(1000.f, -1000.f, 0.f, 1.f), Vector2(1.f, 0.f) }
	};
	for (uint32_t VertexIndex = 0; VertexIndex < ETERNAL_ARRAYSIZE(PlaneVertices); ++VertexIndex)
	{
		Plane->PushVertex(PlaneVertices[VertexIndex]);
	}

	uint32_t PlaneIndices[][3] = {
		{ 0, 1, 2 },
		{ 0, 2, 3 }
	};
	for (uint32_t IndexIndex = 0; IndexIndex < ETERNAL_ARRAYSIZE(PlaneIndices); ++IndexIndex)
	{
		uint32_t* Triangle = &PlaneIndices[IndexIndex][0];
		Plane->PushTriangle(Triangle[0], Triangle[1], Triangle[2]);
	}
	_WaterPlane = Plane;
	_CameraConstant = new D3D11Constant(sizeof(Types::Matrix4x4) * 2, Graphics::D3D11Resource::DYNAMIC, Graphics::D3D11Resource::WRITE);
}

void WaterTask::DoTask()
{
	RenderTarget* CleanRenderTargets[] = {
		nullptr
	};

	Resource::LockedResource LockedResourceObj = ((D3D11Constant*)_CameraConstant)->Lock(_Context, Resource::LOCK_WRITE_DISCARD);
	Matrix4x4* CameraMatrix = (Matrix4x4*)LockedResourceObj.Data;
	XMMATRIX ProjMatrix = XMMatrixTranspose(XMLoadFloat4x4(&_ViewMatrix));
	_Camera->GetProjectionMatrix(*CameraMatrix);
	XMStoreFloat4x4(CameraMatrix + 1, ProjMatrix);
	((Graphics::D3D11Constant*)_CameraConstant)->Unlock(_Context);

	_Context.BindConstant<Context::VERTEX>(0, _CameraConstant);

	_Context.BindShader<Context::VERTEX>(_VS);
	_Context.BindShader<Context::PIXEL>(_PS);

	_Context.SetRenderTargets(&_RenderTarget, 1);
	_Context.DrawIndexed(_WaterPlane->GetVertexBuffer(), _WaterPlane->GetIndexBuffer());
	_Context.SetRenderTargets(CleanRenderTargets, 1);

	_Context.UnbindShader<Context::VERTEX>();
	_Context.UnbindShader<Context::PIXEL>();

	_Context.UnbindConstant<Context::VERTEX>(0);
}
