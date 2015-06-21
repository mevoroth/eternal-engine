#include <Windows.h>

#include "d3d11/D3D11Device.hpp"
#include "d3d11/D3D11Renderer.hpp"
//#include "d3d11/D3D11OrthographicCamera.hpp"
//#include "d3d11/D3D11PerspectiveCamera.hpp"
//#include "d3d11/D3D11Material.hpp"
#include "d3d11/D3D11InputLayout.hpp"
#include "d3d11/D3D11VertexShader.hpp"
#include "d3d11/D3D11GeometryShader.hpp"
#include "d3d11/D3D11PixelShader.hpp"
#include "d3d11/D3D11BlendState.hpp"
#include "d3d11/D3D11Viewport.hpp"
#include "d3d11/D3D11RenderTarget.hpp"

//#include "d3d10/D3D10Device.hpp"
//#include "d3d10/D3D10Renderer.hpp"
//#include "d3d10/D3D10OrthographicCamera.hpp"
//#include "d3d10/D3D10PerspectiveCamera.hpp"
//#include "d3d10/D3D10Material.hpp"
//#include "d3d10/D3D10InputLayout.hpp"
//#include "d3d10/D3D10VertexShader.hpp"
//#include "d3d10/D3D10GeometryShader.hpp"
//#include "d3d10/D3D10PixelShader.hpp"
//#include "d3d10/D3D10BlendState.hpp"
//#include "d3d10/D3D10Viewport.hpp"

//#include "opengl/OGLDevice.hpp"
//#include "opengl/OGLRenderer.hpp"

#include "Mesh/Mesh.hpp"
#include "Import/fbx/ImportFbx.hpp"

#include "Input/Win/WinInput.hpp"

#include <stack>
#include <gl/GL.h>

#include "d3d11/D3D11ShaderFactory.hpp"
#include "Parallel/TaskManager.hpp"
#include "RenderingTask.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::Import;
using namespace Eternal::Input;

//void DrawMeshes(D3D11Renderer* renderer, const Mesh* mesh);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	D3D11Device DeviceObj(hInstance, nCmdShow, "Eternal Sandbox", "EternalClass");
	DeviceObj.Create();

	D3D11Renderer RendererObj;

	Eternal::Parallel::TaskManager TaskManagerObj;

	D3D11ShaderFactory ShaderFactoryObj;
	
	//new Eternal::Input::WinInput();

	new ImportFbx();
	Mesh<D3D11PosUVVertexBuffer::PosUVVertex, D3D11PosUVVertexBuffer, D3D11UInt32IndexBuffer> MeshObj;
	ImportFbx::Get()->Import("mesh2.test.fbx", MeshObj);

	for (;;){}
	////printf("test");


	//new D3D11Device(hInstance, nCmdShow, "Eternal Sandbox", "EternalClass");
	//static_cast<D3D11Device*>(Device::Get())->Create();
	////new D3D10Device(hInstance, nCmdShow, "Eternal SandBox", "EternalClass");
	////static_cast<D3D10Device*>(Device::Get())->Create();
	////new OGLDevice(hInstance, nCmdShow, "Eternal SandBox", "EternalClass");
	////static_cast<OGLDevice*>(Device::Get())->Create();

	//D3D11Renderer renderer;
	////D3D10Renderer renderer(Renderer::HARDWARE, Renderer::NO_AA);
	////OGLRenderer renderer;

	////D3D11OrthographicCamera camera;
	////D3D10OrthographicCamera camera;
	////OGLOrthographicCamera camera;
	////D3D11PerspectiveCamera camera;
	////D3D10PerspectiveCamera camera;

	//RenderTarget* backBuffer = renderer.GetBackBuffer();
	//D3D11BlendState blendState(BlendState::SRC_ALPHA, BlendState::INV_SRC_ALPHA, BlendState::OP_ADD,
	//	BlendState::SRC_ALPHA, BlendState::INV_DEST_ALPHA, BlendState::OP_ADD);
	////D3D10BlendState blendState(BlendState::SRC_ALPHA, BlendState::INV_SRC_ALPHA, BlendState::OP_ADD,
	////	BlendState::SRC_ALPHA, BlendState::INV_DEST_ALPHA, BlendState::OP_ADD);
	////renderer.AttachCamera(&camera);
	////renderer.AttachRenderTargets(&backBuffer, 1);
	////renderer.SetBlendMode(&blendState);
	////D3D11Material Mat;
	////D3D10Material mat;
	////D3D11InputLayout inputLayout((D3D11InputLayout::VertexDataType)(D3D11InputLayout::POSITION_T /*| D3D11InputLayout::NORMAL_T*/ | D3D11InputLayout::TEXCOORD_T));
	////D3D10InputLayout inputLayout((D3D10InputLayout::VertexDataType)(D3D10InputLayout::POSITION_T /*| D3D11InputLayout::NORMAL_T*/ | D3D10InputLayout::TEXCOORD_T));
	////D3D11VertexShader vs("default", "default.vs.hlsl", inputLayout);
	////D3D11GeometryShader gs("default", "default.gs.hlsl");
	////D3D11PixelShader ps("default", "default.ps.hlsl");
	////D3D10VertexShader vs("default", "default.vs.hlsl", inputLayout);
	////D3D10GeometryShader gs("default", "default.gs.hlsl");
	////D3D10PixelShader ps("default", "default.ps.hlsl");
	////Mat.AttachInputLayout(&inputLayout);
	////Mat.AttachVertexShader(&vs);
	////Mat.AttachGeometryShader(&gs);
	////Mat.AttachPixelShader(&ps);
	////Mat.Apply();

	//D3D11Viewport viewport(0, 0, Device::WIDTH, Device::HEIGHT);
	////D3D10Viewport viewport(0, 0, Device::WIDTH, Device::HEIGHT);
	////renderer.SetViewport(&viewport);
	////Vertex v[4];

	////v[0].Pos = Vector4(0.f, 0.f, 1.f, 1.f);
	////v[0].Tex = Vector2(0.f, 0.f);
	//////v[0].Norm = XMFLOAT3(0.f, -1.f, 0.f);

	////v[1].Pos = Vector4(0.f, 1.f, 1.f, 1.f);
	////v[1].Tex = Vector2(0.f, 1.f);
	//////v[1].Norm = XMFLOAT3(0.f, -1.f, 0.f);

	////v[2].Pos = Vector4(1.f, 1.f, 1.f, 1.f);
	////v[2].Tex = Vector2(1.f, 1.f);
	//////v[2].Norm = XMFLOAT3(0.f, -1.f, 0.f);

	////v[3].Pos = Vector4(1.f, 0.f, 1.f, 1.f);
	////v[3].Tex = Vector2(1.f, 0.f);
	////v[3].Norm = XMFLOAT3(0.f, -1.f, 0.f);

	//uint16_t indices[6] = {
	//	0, 1, 2,
	//	0, 2, 3
	//};

	////D3D11VertexPosNormTex cube[8];
	////cube[0].Pos = XMVectorSet(-10.f, -10.f, 10.f, 1.f);
	////cube[1].Pos = XMVectorSet(10.f, -10.f, 10.f, 1.f);
	////cube[2].Pos = XMVectorSet(10.f, -10.f, -10.f, 1.f);
	////cube[3].Pos = XMVectorSet(-10.f, -10.f, -10.f, 1.f);
	////cube[4].Pos = XMVectorSet(-10.f, 10.f, 10.f, 1.f);
	////cube[5].Pos = XMVectorSet(10.f, 10.f, 10.f, 1.f);
	////cube[6].Pos = XMVectorSet(10.f, 10.f, -10.f, 1.f);
	////cube[7].Pos = XMVectorSet(-10.f, 10.f, -10.f, 1.f);

	////uint16_t indices[36] = {
	////	0, 1, 2,
	////	0, 1, 3,
	////	5, 1, 2,
	////	5, 1, 6,
	////	4, 5, 6,
	////	4, 6, 7,
	////	0, 4, 7,
	////	0, 7, 3,
	////	0, 1, 5,
	////	0, 5, 4,
	////	7, 6, 2,
	////	7, 2, 3
	////};

	//float i = 0;

	//RenderTarget* DeferredRenderTargets[6] = {
	//	new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT),
	//	new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT),
	//	new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT),
	//	new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT),
	//	new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT),
	//	new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT)
	//};
	////char truc[256];
	////sprintf_s(truc, "%x\n", &D3D11Renderer::DrawDeferred);

	////OutputDebugString(truc);

	//while (true)
	//{
	//	Input::Get()->Update();
	//	XMMATRIX TempMatrix = XMMatrixTranslation(1000.f*Input::Get()->GetAxis(Input::JOY0_LX), 1000.f * Input::Get()->GetAxis(Input::JOY0_LY), 10000.f*Input::Get()->GetAxis(Input::JOY0_RY));
	//	Matrix4x4 ModelMatrix;
	//	XMStoreFloat4x4(&ModelMatrix, TempMatrix);
	//	//camera.SetModelMatrix(ModelMatrix);
	//	//camera.SetModelMatrix(XMMatrixTranslation(10.f*Input::Get()->GetAxis(Input::JOY0_LX), 10.f * Input::Get()->GetAxis(Input::JOY0_LY), 10.f*Input::Get()->GetAxis(Input::JOY0_RY)));

	//	//renderer.AttachRenderTargets(DeferredRenderTargets, 6);
	//	//renderer.BeginDeferred();

	//	//renderer.AttachMaterial(&Mat);
	//	//renderer.PushContext();
	//	//renderer.AttachCamera(&camera);
	//	//DrawMeshes(&renderer, &mesh);
	//	//renderer.DrawDeferred(mesh.GetSubMeshes()[0].GetVertices(), mesh.GetSubMeshes()[0].GetVerticesCount(), sizeof(Vertex), mesh.GetSubMeshes()[0].GetIndices(), mesh.GetSubMeshes()[0].GetIndicesCount());
	//	//renderer.PopContext();

	//	//renderer.EndDeferred();

	//	//camera.SetModelMatrix()
	//	//renderer.ClearRenderTargets(&backBuffer, 1);
	//	//renderer.AttachRenderTargets(&backBuffer, 1);
	//	//renderer.DrawIndexed(cube, 8, sizeof(D3D11VertexPosNormTex), indices, 36);
	//	//renderer.PushContext();
	//	//renderer.LoadMatrix(NewIdentity());
	//	////renderer.MulMatrix(XMMatrixTranslation(10.f*Input::Get()->GetAxis(Input::JOY0_LX), 10.f * Input::Get()->GetAxis(Input::JOY0_LY), 10.f*Input::Get()->GetAxis(Input::JOY0_RY)));
	//	//renderer.DrawIndexed(v, 4, sizeof(Vertex), indices, 6);
	//	//renderer.PopContext();

	//	//renderer.PushContext();
	//	//DrawMeshes(&renderer, &mesh);
	//	//renderer.PopContext();

	//	//renderer.Flush();
	//	++i;
	//}

	return 0;
}

//void DrawMeshes(D3D11Renderer* renderer, const Mesh* mesh)
//{
//	renderer->MulMatrix(mesh->GetTransform().GetModelMatrix());
//	if (mesh->GetVerticesCount() > 0)
//	{
//		renderer->DrawDeferred(mesh->GetVertices(), mesh->GetVerticesCount(), sizeof(Vertex), mesh->GetIndices(), mesh->GetIndicesCount());
//	}
//
//	for (int i = 0, c = mesh->GetSubMeshesCount(); i < c; ++i)
//	{
//		renderer->PushContext();
//		DrawMeshes(renderer, &mesh->GetSubMeshes()[i]);
//		renderer->PopContext();
//	}
//}
