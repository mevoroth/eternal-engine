#include "Commands/BakeCommand.hpp"
#include "CommandLine/CommandLineParser.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/GraphicsContextFactory.hpp"
#include "Graphics/Pipeline.hpp"
#include "Proxy/ProxyPipeline.hpp"
#include "Tools/PipelineDependency.hpp"
#include "GraphicsEngine/Renderer.hpp"
#include "File/FileFactory.hpp"
#include "File/File.hpp"
#include "File/FilePath.hpp"
#include <string>
#include <unordered_set>

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

			string PipelinesLibraryFileName = FilePath::FindOrCreate("pipelines_library", FileType::FILE_TYPE_CACHED_PIPELINES);

			File* PipelinesLibrary = CreateFileHandle(PipelinesLibraryFileName);
			PipelinesLibrary->Open(File::OpenMode::WRITE);

			GraphicsContext* EngineGraphicsContext = CreateGraphicsContext(DeviceType::DEVICE_TYPE_PROXY);
			Renderer* EngineRenderer = new Renderer(*EngineGraphicsContext);

			auto SerializePipelines = [PipelinesLibrary](_In_ vector<Pipeline*>& Pipelines) -> void
			{
				PipelinesLibrary->Serialize(
					Pipelines,
					[]() -> Pipeline* { return nullptr; },
					[](_Inout_ Pipeline* InOutPipeline)
					{
						ProxyPipeline* CurrentProxyPipeline = static_cast<ProxyPipeline*>(InOutPipeline);
					}
				);
			};

			const unordered_set<Pipeline*>& PipelinesSet = EngineGraphicsContext->GetPipelineDependency().GetPipelines();
			vector<Pipeline*> Pipelines;
			Pipelines.insert(Pipelines.end(), PipelinesSet.begin(), PipelinesSet.end());
			SerializePipelines(Pipelines);

			delete EngineRenderer;
			EngineRenderer = nullptr;
			DestroyGraphicsContext(EngineGraphicsContext);

			PipelinesLibrary->Close();
			DestroyFileHandle(PipelinesLibrary);
		}
	}
}
