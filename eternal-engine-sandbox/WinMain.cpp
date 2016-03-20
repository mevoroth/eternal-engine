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
#include "d3d11/D3D11Constant.hpp"

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

#include "Mesh/GenericMesh.hpp"
#include "Import/fbx/ImportFbx.hpp"

#include "Input/XInput/XInput.hpp"
#include "Input/WinInput/WinInput.hpp"

#include <stack>

#include <limits>
#include "d3d11/D3D11ShaderFactory.hpp"
#include "Parallel/TaskManager.hpp"
#include "RenderingTask.hpp"
#include "WaterTask.hpp"
#include "DebugTextTask.hpp"
#include "Camera/PerspectiveCamera.hpp"
#include "Camera/OrthographicCamera.hpp"
#include "Light/Light.hpp"
#include "Transform/Transform.hpp"
#include "Types/Types.hpp"
#include "d3d11/D3D11PosUVVertexBuffer.hpp"
#include "d3d11/D3D11DepthStencilBuffer.hpp"
#include "Core/TransformComponent.hpp"
#include "WindowsProcess.hpp"

#include "Import/tga/ImportTga.hpp"
#include "Resources/TextureFactory.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::Import;
using namespace Eternal::Input;
using namespace Eternal::Components;
using namespace Eternal::Types;
//void DrawMeshes(D3D11Renderer* renderer, const Mesh* mesh);

void D3D12Main();

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	D3D11Device DeviceObj(hInstance, nCmdShow, "Eternal Sandbox", "EternalClass");

	Eternal::Input::WinInput* WinInputObj = new Eternal::Input::WinInput();
	WindowsProcess WinProcObj;
	WindowsProcess::SetInputHandler(Input::Get());
	DeviceObj.Create(WindowsProcess::WindowProc);

	Eternal::Input::XInput* XInputObj = new Eternal::Input::XInput();

	D3D11Renderer RendererObj;

	Eternal::Parallel::TaskManager TaskManagerObj;

	D3D11ShaderFactory ShaderFactoryObj;
	
	new ImportTga();
	uint32_t height, width;
	uint8_t* Content = ImportTga::Get()->Import("test.tga", height, width);
	//uint32_t Content[] = {
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000,
	//	0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF000000
	//};
	
	new Eternal::Resources::TextureFactory();

	new ImportFbx();
	GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer> MeshObj;
	ImportFbx::Get()->Import("sponza.fbx", MeshObj);

	GenericMesh<D3D11PosUVVertexBuffer::PosUVVertex, D3D11PosUVVertexBuffer, D3D11UInt32IndexBuffer> Plane;
	D3D11PosUVVertexBuffer::PosUVVertex PlaneVertices[] = {
		{ Vector4(-1000.f, -1000.f, 0.f, 1.f), Vector2(0.f, 0.f) },
		{ Vector4(-1000.f,  1000.f, 0.f, 1.f), Vector2(0.f, 1.f) },
		{ Vector4( 1000.f,  1000.f, 0.f, 1.f), Vector2(1.f, 1.f) },
		{ Vector4( 1000.f, -1000.f, 0.f, 1.f), Vector2(1.f, 0.f) }
	};
	for (uint32_t VertexIndex = 0; VertexIndex < ETERNAL_ARRAYSIZE(PlaneVertices); ++VertexIndex)
	{
		Plane.PushVertex(PlaneVertices[VertexIndex]);
	}
	
	uint32_t PlaneIndices[][3] = {
		{ 0, 1, 2 },
		{ 0, 2, 3 }
	};
	for (uint32_t IndexIndex = 0; IndexIndex < ETERNAL_ARRAYSIZE(PlaneIndices); ++IndexIndex)
	{
		uint32_t* Triangle = &PlaneIndices[IndexIndex][0];
		Plane.PushTriangle(Triangle[0], Triangle[1], Triangle[2]);
	}

	PerspectiveCamera CameraObj(0.1f, 10000.f, 60.f);
	OrthographicCamera OrthoCamObj(0.f, 1000.f, 1.f);

	std::vector<Light> Lights;
	Lights.push_back(Light(Vector3(0.f, 0.f, 100.f), 1.f));
	Lights.push_back(Light(Vector3(0.f, 0.f, 0.f), 1.f));
	Lights.push_back(Light(Vector3(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()), 1.f));
	Lights.push_back(Light(Vector3(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()), 1.f));
	Lights.push_back(Light(Vector3(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()), 1.f));
	Lights.push_back(Light(Vector3(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()), 1.f));
	Lights.push_back(Light(Vector3(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()), 1.f));
	Lights.push_back(Light(Vector3(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()), 1.f));

	D3D11DepthStencilBuffer DepthStencilBuffers(640, 480);

	D3D11RenderTarget* RenderTargets[] = {
		new D3D11RenderTarget(640, 480),
		new D3D11RenderTarget(640, 480),
		new D3D11RenderTarget(640, 480),
		new D3D11RenderTarget(640, 480),
		new D3D11RenderTarget(640, 480),
		new D3D11RenderTarget(640, 480)
	};

	Eternal::Core::TransformComponent ParentTransform;
	Eternal::Core::TransformComponent CameraTransform;
	CameraTransform.AttachTo(&ParentTransform);
	Eternal::Sandbox::RenderingTask* Rendering = nullptr;

	Eternal::Sandbox::WaterTask* WaterRendering = nullptr;
	Eternal::Sandbox::DebugTextTask* DebugRendering = nullptr;

	ParentTransform.Transform.SetTranslation(Vector3(0.f, -100.f, 0.f));
	CameraTransform.Transform.Rotate(Vector3(0.f, 0.f, 0.f));
	//CameraTransform.Transform.Rotate(Vector3(0.f, 45.f * 3.1415f / 180.f, 0.f));
	//CameraTransform.Transform.Rotate(Vector3(0.f, 0.f, 45.f * 3.1415f / 180.f));

	Rendering = new Eternal::Sandbox::RenderingTask(RendererObj, *RendererObj.GetMainContext(), &CameraObj, Lights, Content);
	DebugRendering = new Eternal::Sandbox::DebugTextTask(RendererObj, *RendererObj.GetMainContext());

	for (;;)
	{
		WindowsProcess::ExecuteMessageLoop();
		WinInputObj->Update();
		XInputObj->Update();

		CameraTransform.Transform.Rotate(Vector3(
			0.f,
			Input::Get()->GetAxis(Input::JOY0_RX) * 0.01f,
			0.f
		));

		Vector3 Forward = CameraTransform.Transform.GetForward();
		Vector3 Right = CameraTransform.Transform.GetRight();
		Vector3 Up = ParentTransform.Transform.GetUp();

		//CameraTransform.Translate(Input::Get()->GetAxis(Input::JOY0_LX) * 1.f * Right - Input::Get()->GetAxis(Input::JOY0_LY) * 1.f * Forward);
		//ParentTransform.Transform.Translate(/*Vector3(
		//	Input::Get()->GetAxis(Input::JOY0_LX) * 1.f,
		//	-Input::Get()->GetAxis(Input::JOY0_RY) * 1.f,
		//	-Input::Get()->GetAxis(Input::JOY0_LY) * 1.f
		//)*/);
		ParentTransform.Transform.Translate(Input::Get()->GetAxis(Input::JOY0_LX) * Right - Input::Get()->GetAxis(Input::JOY0_RY) * Up - Input::Get()->GetAxis(Input::JOY0_LY) * Forward);

		//WaterRendering = new Eternal::Sandbox::WaterTask(RendererObj, *RendererObj.GetMainContext());
		//WaterRendering->SetCamera(&CameraObj);
		//Matrix4x4 Model = ParentTransform.Transform.GetModelMatrix();
		//WaterRendering->SetViewMatrix(ParentTransform.Transform.GetModelMatrix() * CameraTransform.Transform.GetModelMatrix());
		//WaterRendering->SetMesh(&Plane);
		//WaterRendering->Execute();
		//delete WaterRendering;

		//Matrix4x4 blabla;
		////Vector4 posdemerde(-1000.f, -1000.f, 0.f, 1.f);
		//Vector4 posdemerde(-1000.f, 0.f, -1000.f, 1.f);
		//CameraObj.GetProjectionMatrix(blabla);
		//blabla = ParentTransform.Transform.GetModelMatrix() * CameraTransform.Transform.GetModelMatrix();
		//XMStoreFloat4x4(&blabla, XMMatrixTranspose(XMLoadFloat4x4(&blabla)));
		//XMVECTOR v = XMVector4Transform(XMLoadFloat4(&posdemerde), XMLoadFloat4x4(&blabla));
		//ETERNAL_ASSERT(true);

		//Eternal::Sandbox::RenderingTask* PreviousRendering = Rendering;
		////Rendering = new Eternal::Sandbox::RenderingTask(RendererObj, *RendererObj.GetMainContext(), &CameraObj, Lights, Content);
		//Rendering->SetViewMatrix(ParentTransform.Transform.GetModelMatrix() * CameraTransform.Transform.GetModelMatrix());
		////Rendering->SetMesh(&Plane);
		//Rendering->SetMesh(&MeshObj);
		//Rendering->SetDeferredQuad(&Plane);
		//Rendering->SetRenderTargets((RenderTarget**)&RenderTargets, ETERNAL_ARRAYSIZE(RenderTargets));
		//Rendering->SetBackBufferRenderTarget(RendererObj.GetBackBuffer());
		////TaskManagerObj.Push(new FakeTask());
		////Rendering->Setup();
		////TaskManagerObj.Push(Rendering);
		////TaskManagerObj.Barrier();
		//Rendering->Execute();
		////delete Rendering;
		
		Matrix4x4 ViewMat = ParentTransform.Transform.GetModelMatrix() * CameraTransform.Transform.GetModelMatrix();
		//ViewMat.m[0][]
		DebugRendering->Setup();
		//DebugRendering->SetupText(std::string("Camera Position : (") + to_string(ViewMat._41) + std::string(", ") + to_string(ViewMat._42) + std::string(", ") + to_string(ViewMat._43) + std::string(")") /*+ ")"*/);
		DebugRendering->SetupText("abcdefghijklmnopqrstuvwxyz");
		DebugRendering->Execute();
	}

	return 0;
}

//#include "d3d12/D3D12Renderer.hpp"

void D3D12Main()
{
	//D3D12Renderer renderer;
}
