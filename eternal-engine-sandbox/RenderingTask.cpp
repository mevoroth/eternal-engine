#include "RenderingTask.hpp"

#include <cmath>

#include "Macros/Macros.hpp"
#include "Graphics/Device.hpp"
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
#include "d3d11/D3D11Sampler.hpp"
#include "d3d11/D3D11DepthStencil.hpp"
#include "d3d11/D3D11DepthStencilBuffer.hpp"
#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"

#include "d3d11/D3D11Context.hpp"
#include "d3d11/D3D11Texture.hpp"

#include "Resources/TextureFactory.hpp"

using namespace Eternal;
using namespace Eternal::Sandbox;
using namespace Eternal::Types;

RenderingTask::RenderingTask(
	_In_ Graphics::Renderer& RendererObj,
	_In_ Graphics::Context& ContextObj,
	_In_ Components::Camera* CameraObj,
	_In_ const std::vector<Components::Light>& Lights,
	_In_ const void* TextureData
)
	: _Renderer(RendererObj)
	, _Context(ContextObj)
	, _Camera(CameraObj)
{
	Graphics::InputLayout::VertexDataType MeshDataType[] = {
		Graphics::InputLayout::POSITION_T,
		Graphics::InputLayout::NORMAL_T,
		Graphics::InputLayout::UV_T
	};
	_VS = Graphics::ShaderFactory::Get()->CreateVertexShader("Default", "default.vs.hlsl", MeshDataType, ETERNAL_ARRAYSIZE(MeshDataType));
	_GS = Graphics::ShaderFactory::Get()->CreateGeometryShader("Default", "default.gs.hlsl");
	_PS = Graphics::ShaderFactory::Get()->CreatePixelShader("Default", "default.ps.hlsl");

	Graphics::InputLayout::VertexDataType PostProcessDataType[] = {
		Graphics::InputLayout::POSITION_T,
		Graphics::InputLayout::UV_T
	};
	_DeferredVS = Graphics::ShaderFactory::Get()->CreateVertexShader("Deferred", "deferred.vs.hlsl", PostProcessDataType, ETERNAL_ARRAYSIZE(PostProcessDataType));
	_DeferredPS = Graphics::ShaderFactory::Get()->CreatePixelShader("Deferred", "deferred.ps.hlsl");
	
	_LightsConstants = new Graphics::D3D11Constant(sizeof(Components::Light) * 8, Graphics::D3D11Resource::IMMUTABLE, Graphics::D3D11Resource::NONE, (void*)&Lights[0]);
	Types::Matrix4x4 CameraMatrix;
	CameraObj->GetProjectionMatrix(CameraMatrix);
	_CameraConstant = new Graphics::D3D11Constant(sizeof(Types::Matrix4x4) * 2, Graphics::D3D11Resource::DYNAMIC, Graphics::D3D11Resource::WRITE, (void*)&CameraMatrix);
	_ModelConstant = new Graphics::D3D11Constant(sizeof(Types::Matrix4x4), Graphics::D3D11Resource::DYNAMIC, Graphics::D3D11Resource::WRITE, (void*)&_ContextMatrix);
	_StandardSampler = new Graphics::D3D11Sampler(true, true, false, false, Graphics::Sampler::WRAP, Graphics::Sampler::WRAP, Graphics::Sampler::WRAP);
	_BlendState = new Graphics::D3D11BlendState(Graphics::BlendState::SRC_ALPHA, Graphics::BlendState::INV_SRC_ALPHA, Graphics::BlendState::OP_ADD,
		Graphics::BlendState::SRC_ALPHA, Graphics::BlendState::INV_SRC_ALPHA, Graphics::BlendState::OP_ADD);
	_Viewport = new Graphics::D3D11Viewport(0, 0, Graphics::Device::WIDTH, Graphics::Device::HEIGHT);
	_DepthStencilState = new Graphics::D3D11DepthStencil(
		Graphics::DepthTest(Graphics::DepthTest::ALL, Graphics::Comparison::LESS),
		Graphics::StencilTest()
	);
	_DepthStencilBuffer = new Graphics::D3D11DepthStencilBuffer(Graphics::Device::WIDTH, Graphics::Device::HEIGHT);
	_ContextMatrix = Types::NewIdentity();

	uint32_t Size = 256;
	for (uint32_t TextureSizeIndex = 0; TextureSizeIndex < SIZE_COUNT; ++TextureSizeIndex)
	{
		_Textures[TextureSizeIndex] = new Graphics::D3D11Texture(Graphics::Texture::BGRA8888, Graphics::D3D11Resource::DYNAMIC, Graphics::Resource::WRITE, Size, Size);
		Size *= 2;
	}

	//_Texture = new Graphics::D3D11Texture(Graphics::Texture::BGRA8888, 2, 2, TextureData);

	OutputDebugString("BEGINNING RENDERING\n");
}

RenderingTask::~RenderingTask()
{
	OutputDebugString("ENDING RENDERING\n");
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
	delete _StandardSampler;
	_StandardSampler = nullptr;
	delete _DepthStencilState;
	_DepthStencilState = nullptr;
	delete _DepthStencilBuffer;
	_DepthStencilBuffer = nullptr;
	for (uint32_t TextureSizeIndex = 0; TextureSizeIndex < SIZE_COUNT; ++TextureSizeIndex)
	{
		delete _Textures[TextureSizeIndex];
		_Textures[TextureSizeIndex] = nullptr;
	}
}

void RenderingTask::Setup()
{
	SetState(SETUP);
}

void RenderingTask::Execute()
{
	SetState(EXECUTING);
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
	_DepthStencilBuffer->Clear(&_Context);

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

	_Context.BindConstant<Graphics::Context::VERTEX>(0, _CameraConstant);
	_Context.BindConstant<Graphics::Context::VERTEX>(1, _ModelConstant);
	_Context.BindConstant<Graphics::Context::PIXEL>(0, _CameraConstant);
	_Context.BindConstant<Graphics::Context::PIXEL>(1, _ModelConstant);

	_Context.SetBlendMode(_BlendState);
	_Context.SetViewport(_Viewport);

	_Context.BindSampler<Graphics::Context::PIXEL>(0, _StandardSampler);

	static_cast<Graphics::D3D11Context&>(_Context).GetD3D11Context()->OMSetDepthStencilState(_DepthStencilState->GetD3D11DepthStencilState(), 0);
	_Context.SetDepthBuffer(_DepthStencilBuffer);
	_Context.SetRenderTargets(_RTs, _RTCount);

	_ModelContext.Push(_ContextMatrix);
	//XMStoreFloat4x4(&_ContextMatrix, XMMatrixIdentity());
	//_ContextMatrix.m[3][0] = _ViewMatrix.m[3][0];
	//_ContextMatrix.m[3][1] = _ViewMatrix.m[3][1];
	//_ContextMatrix.m[3][2] = _ViewMatrix.m[3][2];
	//_Mesh->GetTransform().SetTranslation(Vector3(_ViewMatrix.m[3][0], _ViewMatrix.m[3][1], _ViewMatrix.m[3][2]));
	//_ContextMatrix = _ViewMatrix;
	_Draw(_Mesh);
	_ContextMatrix = _ModelContext.Head();
	_ModelContext.Pop();

	_Context.SetRenderTargets(RenderTargets, ETERNAL_ARRAYSIZE(RenderTargets));
	_Context.SetDepthBuffer(nullptr);
	static_cast<Graphics::D3D11Context&>(_Context).GetD3D11Context()->OMSetDepthStencilState(nullptr, 0);

	_Context.UnbindSampler<Graphics::Context::PIXEL>(0);

	_Context.UnbindConstant<Graphics::Context::VERTEX>(0);
	_Context.UnbindConstant<Graphics::Context::VERTEX>(1);
	_Context.UnbindConstant<Graphics::Context::PIXEL>(0);
	_Context.UnbindConstant<Graphics::Context::PIXEL>(1);

	_Context.UnbindShader<Graphics::Context::VERTEX>();
	_Context.UnbindShader<Graphics::Context::GEOMETRY>();
	_Context.UnbindShader<Graphics::Context::PIXEL>();

	_Context.BindShader<Graphics::Context::VERTEX>(_DeferredVS);
	_Context.BindShader<Graphics::Context::PIXEL>(_DeferredPS);

	_Context.BindBuffer<Graphics::Context::PIXEL>(0, (Graphics::D3D11RenderTarget*)_RTs[0]);
	_Context.BindBuffer<Graphics::Context::PIXEL>(1, (Graphics::D3D11RenderTarget*)_RTs[1]);
	_Context.BindBuffer<Graphics::Context::PIXEL>(2, (Graphics::D3D11RenderTarget*)_RTs[2]);
	_Context.BindBuffer<Graphics::Context::PIXEL>(3, (Graphics::D3D11RenderTarget*)_RTs[3]);
	//_Context.BindBuffer<Graphics::Context::PIXEL>(3, (Graphics::D3D11Texture*)_Texture);
	_Context.BindBuffer<Graphics::Context::PIXEL>(4, (Graphics::D3D11RenderTarget*)_RTs[4]);
	_Context.BindBuffer<Graphics::Context::PIXEL>(5, (Graphics::D3D11RenderTarget*)_RTs[5]);

	_Context.BindConstant<Graphics::Context::PIXEL>(0, _LightsConstants);

	_Context.BindSampler<Graphics::Context::PIXEL>(0, _StandardSampler);

	_Context.SetRenderTargets(&_BackBuffer, 1);

	_Context.DrawIndexed(_DeferredQuad->GetVertexBuffer(), _DeferredQuad->GetIndexBuffer());
	//static_cast<Graphics::D3D11Renderer*>(Graphics::Renderer::Get())->Flush();

	_Context.SetRenderTargets(RenderTargets, ETERNAL_ARRAYSIZE(RenderTargets));

	_Context.UnbindSampler<Graphics::Context::PIXEL>(0);

	_Context.UnbindConstant<Graphics::Context::PIXEL>(0);

	_Context.UnbindBuffer<Graphics::Context::PIXEL>(0);
	_Context.UnbindBuffer<Graphics::Context::PIXEL>(1);
	_Context.UnbindBuffer<Graphics::Context::PIXEL>(2);
	_Context.UnbindBuffer<Graphics::Context::PIXEL>(3);
	_Context.UnbindBuffer<Graphics::Context::PIXEL>(4);
	_Context.UnbindBuffer<Graphics::Context::PIXEL>(5);

	_Context.UnbindShader<Graphics::Context::VERTEX>();
	_Context.UnbindShader<Graphics::Context::PIXEL>();

	//static_cast<Graphics::D3D11Context&>(_Context).GetD3D11Context()->Flush();

	OutputDebugString("RENDERED\n");
	SetState(DONE);
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

		if (MeshObj->_Texture.length())
		{
			_Context.BindBuffer<Graphics::Context::PIXEL>(0, (Graphics::D3D11Texture*)Eternal::Resources::TextureFactory::Get()->GetTexture(MeshObj->_Texture));
			_Context.DrawIndexed(MeshObj->GetVertexBuffer(), MeshObj->GetIndexBuffer());
			_Context.UnbindBuffer<Graphics::Context::PIXEL>(0);
			//uint32_t W, H;
			//uint8_t* Data = Eternal::Resources::TextureFactory::Get()->GetTexture(MeshObj->_Texture, H, W);
			//ETERNAL_ASSERT(Data);
			//if (H >= 256)
			//{
			//	uint32_t TextureIndex = (uint32_t)(log2(H) - 8);
			//	//Graphics::D3D11Texture* TempTex = new Graphics::D3D11Texture(Graphics::Texture::BGRA8888, Graphics::D3D11Resource::DYNAMIC, Graphics::D3D11Resource::WRITE, W, H, Data);
			//	ETERNAL_ASSERT(TextureIndex < SIZE_COUNT);
			//	Graphics::D3D11Resource::LockedResource LockedData = _Textures[TextureIndex]->Lock(_Context, Graphics::Resource::LOCK_WRITE_DISCARD);
			//	memcpy(LockedData.Data, Data, H * W * 4);
			//	_Textures[TextureIndex]->Unlock(_Context);
			//	_Context.BindBuffer<Graphics::Context::PIXEL>(0, _Textures[TextureIndex]);
			//	_Context.DrawIndexed(MeshObj->GetVertexBuffer(), MeshObj->GetIndexBuffer());
			//	_Context.UnbindBuffer<Graphics::Context::PIXEL>(0);
			//}
		}
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

void RenderingTask::Reset()
{
	SetState(IDLE);
}
