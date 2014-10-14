#include <Windows.h>

#include "d3d11/D3D11Device.hpp"
#include "d3d11/D3D11Renderer.hpp"
#include "d3d11/D3D11OrthographicCamera.hpp"
#include "d3d11/D3D11PerspectiveCamera.hpp"
#include "d3d11/D3D11Material.hpp"
#include "d3d11/D3D11InputLayout.hpp"
#include "d3d11/D3D11VertexShader.hpp"
#include "d3d11/D3D11GeometryShader.hpp"
#include "d3d11/D3D11PixelShader.hpp"
#include "d3d11/D3D11BlendState.hpp"
#include "d3d11/D3D11Viewport.hpp"
#include "d3d11/D3D11VertexPosNormTex.hpp"

#include "Mesh/Mesh.hpp"
#include "Import/fbx/ImportFbx.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::Import;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	new ImportFbx();
	Eternal::Components::Mesh mesh = ImportFbx::Get()->Import("mesh.test.fbx");
	printf("test");

	D3D11DeviceType device(
		new D3D11Device(hInstance, nCmdShow, "Eternal Sandbox", "EternalClass")
	);
	device.Get()->Create();

	//D3D11Renderer* renderer = new D3D11Renderer();
	D3D11Renderer renderer;

	D3D11OrthographicCamera camera;
	//D3D11PerspectiveCamera camera;
	D3D11Material material;

	RenderTarget* backBuffer = renderer.GetBackBuffer();
	D3D11BlendState blendState(BlendState::SRC_ALPHA, BlendState::INV_SRC_ALPHA, BlendState::OP_ADD,
		BlendState::SRC_ALPHA, BlendState::INV_DEST_ALPHA, BlendState::OP_ADD);
	renderer.AttachCamera(&camera);
	renderer.AttachRenderTargets(&backBuffer, 1);
	renderer.SetBlendMode(&blendState);
	D3D11Material mat;
	D3D11InputLayout inputLayout((D3D11InputLayout::VertexDataType)(D3D11InputLayout::POSITION_T | D3D11InputLayout::NORMAL_T | D3D11InputLayout::TEXCOORD_T));
	D3D11VertexShader vs("default", "default.vs.hlsl", inputLayout);
	D3D11GeometryShader gs("default", "default.gs.hlsl");
	D3D11PixelShader ps("default", "default.ps.hlsl");
	mat.AttachInputLayout(&inputLayout);
	mat.AttachVertexShader(&vs);
	mat.AttachGeometryShader(&gs);
	mat.AttachPixelShader(&ps);
	mat.Apply();
	//mat.
	//MaterialProperty::ParamType::
	//MaterialProperty matProp("",);
	//mat.SetMaterialDesc()
	D3D11Viewport viewport(0, 0, D3D11DeviceType::WIDTH, D3D11DeviceType::HEIGHT);
	renderer.SetViewport(&viewport);

	D3D11VertexPosNormTex v[4];

	v[0].Pos = XMVectorSet(0.f, 100.f, 0.f, 1.f);
	v[0].Tex = XMFLOAT2(0.f, 0.f);
	v[0].Norm = XMFLOAT3(0.f, -1.f, 0.f);

	v[1].Pos = XMVectorSet(0.f, 100.f, 200.f, 1.f);
	v[1].Tex = XMFLOAT2(1.f, 0.f);
	v[1].Norm = XMFLOAT3(0.f, -1.f, 0.f);
	
	v[2].Pos = XMVectorSet(200.f, 100.f, 200.f, 1.f);
	v[2].Tex = XMFLOAT2(1.f, 1.f);
	v[2].Norm = XMFLOAT3(0.f, -1.f, 0.f);

	v[3].Pos = XMVectorSet(200.f, 100.f, 0.f, 1.f);
	v[3].Tex = XMFLOAT2(0.f, 1.f);
	v[3].Norm = XMFLOAT3(0.f, -1.f, 0.f);

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
		camera.SetModelMatrix({ XMMatrixTranslation(0.01f * i, 0.f, 0.f) });
		//camera.SetModelMatrix()
		renderer.ClearRenderTargets(&backBuffer, 1);
		renderer.AttachMaterial(&mat);
		//renderer.DrawIndexed(cube, 8, sizeof(D3D11VertexPosNormTex), indices, 36);
		renderer.DrawIndexed(v, 4, sizeof(D3D11VertexPosNormTex), indices, 6);
		renderer.Flush();
		++i;
	}

	return 0;
}
