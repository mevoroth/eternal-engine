#include <Windows.h>

//#include "d3d11/D3D11Device.hpp"
//#include "d3d11/D3D11Renderer.hpp"
//#include "d3d11/D3D11OrthographicCamera.hpp"
//#include "d3d11/D3D11PerspectiveCamera.hpp"
//#include "d3d11/D3D11Material.hpp"
//#include "d3d11/D3D11InputLayout.hpp"
//#include "d3d11/D3D11VertexShader.hpp"
//#include "d3d11/D3D11GeometryShader.hpp"
//#include "d3d11/D3D11PixelShader.hpp"
//#include "d3d11/D3D11BlendState.hpp"
//#include "d3d11/D3D11Viewport.hpp"

#include "d3d10/D3D10Device.hpp"
#include "d3d10/D3D10Renderer.hpp"
#include "d3d10/D3D10OrthographicCamera.hpp"
#include "d3d10/D3D10PerspectiveCamera.hpp"
#include "d3d10/D3D10Material.hpp"
#include "d3d10/D3D10InputLayout.hpp"
#include "d3d10/D3D10VertexShader.hpp"
#include "d3d10/D3D10GeometryShader.hpp"
#include "d3d10/D3D10PixelShader.hpp"
#include "d3d10/D3D10BlendState.hpp"
#include "d3d10/D3D10Viewport.hpp"

//#include "opengl/OGLDevice.hpp"
//#include "opengl/OGLRenderer.hpp"

#include "Mesh/Mesh.hpp"
#include "Import/fbx/ImportFbx.hpp"

#include "Input/Win/WinInput.hpp"

#include <stack>
#include <gl/GL.h>

using namespace Eternal::Graphics;
using namespace Eternal::Import;
using namespace Eternal::Input;

void DrawMeshes(Renderer* renderer, const Mesh* mesh);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	new Eternal::Input::WinInput();

	new ImportFbx();
	Eternal::Components::Mesh mesh = ImportFbx::Get()->Import("mesh.test.fbx");
	//printf("test");


	//new D3D11Device(hInstance, nCmdShow, "Eternal Sandbox", "EternalClass");
	//static_cast<D3D11Device*>(Device::Get())->Create();
	new D3D10Device(hInstance, nCmdShow, "Eternal SandBox", "EternalClass");
	static_cast<D3D10Device*>(Device::Get())->Create();
	//new OGLDevice(hInstance, nCmdShow, "Eternal SandBox", "EternalClass");
	//static_cast<OGLDevice*>(Device::Get())->Create();

	//D3D11Renderer renderer;
	D3D10Renderer renderer(Renderer::HARDWARE, Renderer::NO_AA);
	//OGLRenderer renderer;

	//D3D11OrthographicCamera camera;
	//D3D10OrthographicCamera camera;
	//OGLOrthographicCamera camera;
	//D3D11PerspectiveCamera camera;
	D3D10PerspectiveCamera camera;

	RenderTarget* backBuffer = renderer.GetBackBuffer();
	//D3D11BlendState blendState(BlendState::SRC_ALPHA, BlendState::INV_SRC_ALPHA, BlendState::OP_ADD,
	//	BlendState::SRC_ALPHA, BlendState::INV_DEST_ALPHA, BlendState::OP_ADD);
	D3D10BlendState blendState(BlendState::SRC_ALPHA, BlendState::INV_SRC_ALPHA, BlendState::OP_ADD,
		BlendState::SRC_ALPHA, BlendState::INV_DEST_ALPHA, BlendState::OP_ADD);
	renderer.AttachCamera(&camera);
	renderer.AttachRenderTargets(&backBuffer, 1);
	renderer.SetBlendMode(&blendState);
	//D3D11Material mat;
	D3D10Material mat;
	//D3D11InputLayout inputLayout((D3D11InputLayout::VertexDataType)(D3D11InputLayout::POSITION_T /*| D3D11InputLayout::NORMAL_T*/ | D3D11InputLayout::TEXCOORD_T));
	D3D10InputLayout inputLayout((D3D10InputLayout::VertexDataType)(D3D10InputLayout::POSITION_T /*| D3D11InputLayout::NORMAL_T*/ | D3D10InputLayout::TEXCOORD_T));
	//D3D11VertexShader vs("default", "default.vs.hlsl", inputLayout);
	//D3D11GeometryShader gs("default", "default.gs.hlsl");
	//D3D11PixelShader ps("default", "default.ps.hlsl");
	D3D10VertexShader vs("default", "default.vs.hlsl", inputLayout);
	D3D10GeometryShader gs("default", "default.gs.hlsl");
	D3D10PixelShader ps("default", "default.ps.hlsl");
	mat.AttachInputLayout(&inputLayout);
	mat.AttachVertexShader(&vs);
	mat.AttachGeometryShader(&gs);
	mat.AttachPixelShader(&ps);
	mat.Apply();

	//D3D11Viewport viewport(0, 0, Device::WIDTH, Device::HEIGHT);
	D3D10Viewport viewport(0, 0, Device::WIDTH, Device::HEIGHT);
	renderer.SetViewport(&viewport);
	Vertex v[4];

	v[0].Pos = XMVectorSet(0.f, 0.f, 1.f, 1.f);
	v[0].Tex = XMFLOAT2(0.f, 0.f);
	//v[0].Norm = XMFLOAT3(0.f, -1.f, 0.f);

	v[1].Pos = XMVectorSet(0.5f, 0.f, 1.f, 1.f);
	v[1].Tex = XMFLOAT2(1.f, 0.f);
	//v[1].Norm = XMFLOAT3(0.f, -1.f, 0.f);

	v[2].Pos = XMVectorSet(0.5f, 0.5f, 1.f, 1.f);
	v[2].Tex = XMFLOAT2(1.f, 1.f);
	//v[2].Norm = XMFLOAT3(0.f, -1.f, 0.f);

	v[3].Pos = XMVectorSet(0.f, 0.5f, 1.f, 1.f);
	v[3].Tex = XMFLOAT2(0.f, 1.f);
	//v[3].Norm = XMFLOAT3(0.f, -1.f, 0.f);

	uint16_t indices[6] = {
		0, 1, 2,
		0, 2, 3
	};

	//D3D11VertexPosNormTex cube[8];
	//cube[0].Pos = XMVectorSet(-10.f, -10.f, 10.f, 1.f);
	//cube[1].Pos = XMVectorSet(10.f, -10.f, 10.f, 1.f);
	//cube[2].Pos = XMVectorSet(10.f, -10.f, -10.f, 1.f);
	//cube[3].Pos = XMVectorSet(-10.f, -10.f, -10.f, 1.f);
	//cube[4].Pos = XMVectorSet(-10.f, 10.f, 10.f, 1.f);
	//cube[5].Pos = XMVectorSet(10.f, 10.f, 10.f, 1.f);
	//cube[6].Pos = XMVectorSet(10.f, 10.f, -10.f, 1.f);
	//cube[7].Pos = XMVectorSet(-10.f, 10.f, -10.f, 1.f);

	//uint16_t indices[36] = {
	//	0, 1, 2,
	//	0, 1, 3,
	//	5, 1, 2,
	//	5, 1, 6,
	//	4, 5, 6,
	//	4, 6, 7,
	//	0, 4, 7,
	//	0, 7, 3,
	//	0, 1, 5,
	//	0, 5, 4,
	//	7, 6, 2,
	//	7, 2, 3
	//};

	float i = 0;

	while (true)
	{
		Input::Get()->Update();

		camera.SetModelMatrix(XMMatrixTranslation(1000.f*Input::Get()->GetAxis(Input::JOY0_LX), 1000.f * Input::Get()->GetAxis(Input::JOY0_LY), 1000.f*Input::Get()->GetAxis(Input::JOY0_RY)));
		//camera.SetModelMatrix()
		renderer.ClearRenderTargets(&backBuffer, 1);
		//renderer.AttachMaterial(&mat);
		//renderer.DrawIndexed(cube, 8, sizeof(D3D11VertexPosNormTex), indices, 36);
		//renderer.PushContext();
		//renderer.LoadMatrix(NewIdentity());
		////renderer.MulMatrix(XMMatrixTranslation(10.f*Input::Get()->GetAxis(Input::JOY0_LX), 10.f * Input::Get()->GetAxis(Input::JOY0_LY), 10.f*Input::Get()->GetAxis(Input::JOY0_RY)));
		//renderer.DrawIndexed(v, 4, sizeof(Vertex), indices, 6);
		//renderer.PopContext();

		renderer.PushContext();
		DrawMeshes(&renderer, &mesh);
		renderer.PopContext();

		renderer.Flush();
		++i;
	}

	return 0;
}

void DrawMeshes(Renderer* renderer, const Mesh* mesh)
{
	renderer->MulMatrix(mesh->GetTransform().GetModelMatrix());
	if (mesh->GetVerticesCount() > 0)
	{
		renderer->DrawIndexed(mesh->GetVertices(), mesh->GetVerticesCount(), sizeof(Vertex), mesh->GetIndices(), mesh->GetIndicesCount());
	}

	for (int i = 0, c = mesh->GetSubMeshesCount(); i < c; ++i)
	{
		renderer->PushContext();
		DrawMeshes(renderer, &mesh->GetSubMeshes()[i]);
		renderer->PopContext();
	}
}
