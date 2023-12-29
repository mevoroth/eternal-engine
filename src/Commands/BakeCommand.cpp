#include "Commands/BakeCommand.hpp"
#include "CommandLine/CommandLineParser.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/GraphicsContextFactory.hpp"
#include "Proxy/ProxyGraphicsContext.hpp"
#include "Graphics/Pipeline.hpp"
#include "Proxy/ProxyPipeline.hpp"
#include "Tools/PipelineLibrary.hpp"
#include "GraphicsEngine/Renderer.hpp"
#include "Imgui/Imgui.hpp"
#include <string>
#include <unordered_set>

#include "Graphics/Shader.hpp"
#include "Graphics/RootSignature.hpp"
#include "Graphics/RootSignatureFactory.hpp"
#include "Graphics/PipelineFactory.hpp"

namespace Eternal
{
	namespace Commands
	{
		void BakeCommands::Launch(_In_ const CommandLineParser& InCommandLineParser)
		{
			BakeCommands Command(InCommandLineParser);
			Command.Execute();
		}

		BakeCommands::BakeCommands(_In_ const CommandLineParser& InCommandLineParser)
			: _CommandLineParser(InCommandLineParser)
		{
		}

		void BakeCommands::Execute()
		{
			std::string CommandLineValue = _CommandLineParser.ParseCommandLine("bake");

			if (CommandLineValue == "shaders")
			{
				BakeShaders();
			}
		}

		void BakeCommands::BakeShaders()
		{
			using namespace Eternal::Graphics;
			using namespace Eternal::GraphicsEngine;
			using namespace Eternal::FileSystem;
			using namespace Eternal::ImguiSystem;

			std::string DeviceTypeString = _CommandLineParser.ParseCommandLine("gfx");
			DeviceType BakedDeviceType = ConvertStringToDeviceType(DeviceTypeString);
			ETERNAL_ASSERT(BakedDeviceType != Graphics::DeviceType::DEVICE_TYPE_NULL);

			GraphicsContext* EngineGraphicsContext = CreateGraphicsContext(ProxyGraphicsContextCreateInformation(BakedDeviceType));
			Renderer* EngineRenderer = new Renderer(*EngineGraphicsContext);
			Imgui* ImguiContext = new Imgui(*EngineGraphicsContext, *EngineRenderer, nullptr);

			Shader* DebugScreenVS = EngineGraphicsContext->GetShader(ShaderCreateInformation(ShaderType::SHADER_TYPE_VERTEX, "ScreenVertex", "screen.vertex.hlsl"));
			Shader* DebugScreenPS = EngineGraphicsContext->GetShader(ShaderCreateInformation(ShaderType::SHADER_TYPE_PIXEL, "ScreenPixel", "sampletexture.pixel.hlsl"));

			RootSignature* _RootSignature = CreateRootSignature(*EngineGraphicsContext,
				RootSignatureCreateInformation(
					{
						RootSignatureParameter(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_TEXTURE,	RootSignatureAccess::ROOT_SIGNATURE_ACCESS_PIXEL),
						RootSignatureParameter(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER,	RootSignatureAccess::ROOT_SIGNATURE_ACCESS_PIXEL)
					}
				)
			);

			Pipeline* _Pipeline = CreatePipeline(
				*EngineGraphicsContext,
				GraphicsPipelineCreateInformation(
					_RootSignature,
					EngineGraphicsContext->GetEmptyInputLayout(),
					EngineGraphicsContext->GetBackBufferRenderPass(),
					DebugScreenVS,
					DebugScreenPS
				)
			);

			EngineGraphicsContext->SerializePipelineLibrary(PipelineSerializationMode::PIPELINE_SERIALIZATION_MODE_WRITE);

			DestroyPipeline(_Pipeline);
			DestroyRootSignature(_RootSignature);

			delete ImguiContext;
			ImguiContext = nullptr;
			delete EngineRenderer;
			EngineRenderer = nullptr;
			DestroyGraphicsContext(EngineGraphicsContext);
		}
	}
}
