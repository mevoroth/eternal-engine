#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define VC_EXTRALEAN
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
#include "FakeTask.hpp"
#include "Camera/PerspectiveCamera.hpp"
#include "Camera/OrthographicCamera.hpp"
#include "Light/Light.hpp"
#include "Transform/Transform.hpp"
#include "Types/Types.hpp"
#include "d3d11/D3D11PosUVVertexBuffer.hpp"
#include "d3d11/D3D11DepthStencilBuffer.hpp"
#include "Core/TransformComponent.hpp"
#include "WindowsProcess.hpp"
#include "Task/ImguiBeginTask.hpp"
#include "Task/ImguiEndTask.hpp"
#include "Task/ControlsTask.hpp"
#include "Task/DebugTask.hpp"

#include "Import/tga/ImportTga.hpp"
#include "Resources/TextureFactory.hpp"
#include "Resources/Pool.hpp"

#include "Tests.hpp"

#include "AppPools.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::Import;
using namespace Eternal::Input;
using namespace Eternal::Components;
using namespace Eternal::Types;
using namespace Eternal::Task;
using namespace Eternal::Resources;

void D3D12Main();

extern Pool<Transform>* g_TransformPool;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	Tests::ExecuteTest();

	Eternal::Core::TransformComponent::Init();

	//Pool<int, 2>* TestPool = new Pool<int, 2>();

	//uint32_t Handle;
	//int* truc = TestPool->Initialize(Handle);
	//*truc = 0xDEAD;

	//uint32_t Handle2;
	//int* truc2 = TestPool->Initialize(Handle2);
	//*truc2 = 0x4000DEAD;

	//*(TestPool->Get(Handle)) = 0xDEAD0000;

	//bool isok = TestPool->IsValid(Handle);
	//TestPool->Release(Handle);
	//isok = TestPool->IsValid(Handle);

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
	//ImportFbx::Get()->Import("Liya.fbx", MeshObj);

	GenericMesh<D3D11PosUVVertexBuffer::PosUVVertex, D3D11PosUVVertexBuffer, D3D11UInt32IndexBuffer> Plane;
	D3D11PosUVVertexBuffer::PosUVVertex PlaneVertices[] = {
		{ Vector4(-1.f, -1.f, 0.f, 1.f), Vector2(0.f, 1.f) },
		{ Vector4(-1.f,  1.f, 0.f, 1.f), Vector2(0.f, 0.f) },
		{ Vector4( 1.f,  1.f, 0.f, 1.f), Vector2(1.f, 0.f) },
		{ Vector4( 1.f, -1.f, 0.f, 1.f), Vector2(1.f, 1.f) }
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

	D3D11DepthStencilBuffer DepthStencilBuffers(Device::WIDTH, Device::HEIGHT);

	D3D11RenderTarget* RenderTargets[] = {
		new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT),
		new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT),
		new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT),
		new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT),
		new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT),
		new D3D11RenderTarget(Device::WIDTH, Device::HEIGHT)
	};

	Eternal::Core::TransformComponent ParentTransform;
	Eternal::Core::TransformComponent CameraTransform;
	//CameraTransform.AttachTo(&ParentTransform);

	Eternal::Sandbox::RenderingTask* Rendering = nullptr;
	Eternal::Sandbox::WaterTask* WaterRendering = nullptr;
	Eternal::Sandbox::DebugTextTask* DebugRendering = nullptr;
	FakeTask* FakeTaskObj = nullptr;
	ImguiBeginTask* ImguiBeginTaskObj = nullptr;
	ImguiEndTask* ImguiEndTaskObj = nullptr;
	ControlsTask* ControlsTaskObj = nullptr;
	DebugTask* DebugTaskObj = nullptr;

	ParentTransform.GetTransform().SetTranslation(Vector3(0.f, 0.f, 0.f));
	//CameraTransform.Transform.Rotate(Vector3(0.f, 0.f, 0.f));
	//CameraTransform.Transform.Rotate(Vector3(0.f, 45.f * 3.1415f / 180.f, 0.f));
	//CameraTransform.Transform.Rotate(Vector3(0.f, 0.f, 45.f * 3.1415f / 180.f));

	Rendering = new Eternal::Sandbox::RenderingTask(RendererObj, *RendererObj.GetMainContext(), &CameraObj, Lights, Content);
	Rendering->SetTaskName("Rendering Task");
	DebugRendering = new Eternal::Sandbox::DebugTextTask(RendererObj, *RendererObj.GetMainContext());
	DebugRendering->SetTaskName("Debug Rendering");
	FakeTaskObj = new FakeTask();
	FakeTaskObj->SetTaskName("Fake Task");
	ImguiBeginTaskObj = new ImguiBeginTask(DeviceObj.GetWindow(), Device::WIDTH, Device::HEIGHT);
	ImguiBeginTaskObj->SetTaskName("Imgui Begin");
	ImguiBeginTaskObj->SetupKeyboard(WinInputObj);
	ImguiEndTaskObj = new ImguiEndTask(*RendererObj.GetMainContext());
	ImguiEndTaskObj->SetTaskName("Imgui End");
	ControlsTaskObj = new ControlsTask();
	ControlsTaskObj->SetTaskName("Controls Task");
	ControlsTaskObj->RegisterInput(XInputObj);
	ControlsTaskObj->RegisterInput(WinInputObj);
	DebugTaskObj = new DebugTask(*RendererObj.GetMainContext());
	DebugTaskObj->SetTaskName("Debug Task");
	DebugTaskObj->SetRenderTarget(RendererObj.GetBackBuffer());

	Rendering->SetMesh(&MeshObj);
	Rendering->SetDeferredQuad(&Plane);
	Rendering->SetRenderTargets((RenderTarget**)&RenderTargets, ETERNAL_ARRAYSIZE(RenderTargets));
	Rendering->SetBackBufferRenderTarget(RendererObj.GetBackBuffer());

	ImguiEndTaskObj->SetRenderTarget(RendererObj.GetBackBuffer());

	TaskManagerObj.GetTaskScheduler().PushTask(ControlsTaskObj);
	TaskManagerObj.GetTaskScheduler().PushTask(ImguiBeginTaskObj, ControlsTaskObj);
	TaskManagerObj.GetTaskScheduler().PushTask(Rendering, ImguiBeginTaskObj);
	TaskManagerObj.GetTaskScheduler().PushTask(DebugRendering, Rendering);
	TaskManagerObj.GetTaskScheduler().PushTask(DebugTaskObj, DebugRendering);
	TaskManagerObj.GetTaskScheduler().PushTask(FakeTaskObj);
	{
		TaskManagerObj.GetTaskScheduler().PushTask(ImguiEndTaskObj, DebugTaskObj);
		TaskManagerObj.GetTaskScheduler().PushTask(ImguiEndTaskObj, FakeTaskObj);
	}

	Vector3 CameraPosition(0.f, 0.f, 0.f);
	Vector3 CameraDirection(0.f, 1.f, 0.f);
	Vector3 CameraUp(0.f, 0.f, 1.f);

	for (;;)
	{
		WindowsProcess::ExecuteMessageLoop();

		ParentTransform.GetTransform().Rotate(Vector3(
			-Input::Get()->GetAxis(Input::JOY0_RY) * 0.1f,
			Input::Get()->GetAxis(Input::JOY0_RX) * 0.1f,
			0.f
		));
		//ParentTransform.Transform.Rotate(Vector3(
		//	0.f,
		//	Input::Get()->GetAxis(Input::JOY0_RX) * 0.1f,
		//	0.f
		//));

		//Vector3 Forward = ParentTransform.Transform.GetForward();
		//Vector3 Right = ParentTransform.Transform.GetRight();
		//Vector3 Up = ParentTransform.Transform.GetUp();

		Matrix4x4 CameraMatrix = ParentTransform.GetTransform().GetModelMatrix();

		Vector4 CameraDirection4(CameraDirection.x, CameraDirection.y, CameraDirection.z, 1.f);
		Vector4 CameraUp4(CameraUp.x, CameraUp.y, CameraUp.z, 1.f);

		Vector4 Forward4 = CameraMatrix * CameraDirection4;
		Vector4 Up4 = CameraMatrix * CameraUp4;

		Vector3 Forward(Forward4.x, Forward4.y, Forward4.z);
		Vector3 Up(Up4.x, Up4.y, Up4.z);
		Vector3 Right = Cross(Up, Forward);

		//- Input::Get()->GetAxis(Input::JOY0_LX) * 10.f * Right - Input::Get()->GetAxis(Input::JOY0_RY) * 10.f * Up - Input::Get()->GetAxis(Input::JOY0_LY) * 10.f * Forward
		Vector3 Vect(Input::Get()->GetAxis(Input::JOY0_LX) * 10.f, 0.f, Input::Get()->GetAxis(Input::JOY0_LY) * 10.f);
		//Vector3 Vect(Input::Get()->GetAxis(Input::JOY0_LX) * 10.f, Input::Get()->GetAxis(Input::JOY0_RY) * 10.f, Input::Get()->GetAxis(Input::JOY0_LY) * 10.f);
		
		//Vector4 Transformed = ParentTransform.Transform.GetModelMatrix() * Vector4(Vect.x, Vect.y, Vect.z, 0.0f);
		//ParentTransform.Transform.Translate(Vector3(Transformed.x, Transformed.y, Transformed.z));

		CameraPosition += Vect;

		Matrix4x4 CameraProjectionMatrix;
		CameraObj.GetProjectionMatrix(CameraProjectionMatrix);

		XMMATRIX ProjMatrix = XMMatrixTranspose(XMLoadFloat4x4(&CameraProjectionMatrix));
		XMStoreFloat4x4(&CameraProjectionMatrix, ProjMatrix);

		//Matrix4x4 ViewMatrix(
		//	1.f, 0.f, 0.f, 0.f,
		//	0.f, 0.f, 1.f, 0.f,
		//	0.f, -1.f, 0.f, 0.f,
		//	0.f, 0.f, 0.f, 1.f
		//);

		Matrix4x4 ViewMatrix = NewLookToLH(CameraPosition, Forward, Up);

		CameraProjectionMatrix = ViewMatrix * CameraProjectionMatrix;
		ProjMatrix = XMMatrixTranspose(XMLoadFloat4x4(&CameraProjectionMatrix));
		XMStoreFloat4x4(&CameraProjectionMatrix, ProjMatrix);

		Rendering->SetViewMatrix(ViewMatrix/* * ParentTransform.Transform.GetModelMatrix()*/);
		DebugTaskObj->SetModelViewProjectionMatrix(CameraProjectionMatrix);

		TaskManagerObj.Schedule();
		TaskManagerObj.Barrier();
		TaskManagerObj.GetTaskScheduler().Reset();
	}

	return 0;
}

//#include "d3d12/D3D12Renderer.hpp"

void D3D12Main()
{
	//D3D12Renderer renderer;
}
