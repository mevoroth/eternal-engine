#include "CommandLine/CommandLineParser.hpp"
#include "Commands/BakeCommand.hpp"
#include "Time/Timer.hpp"
#include "Time/TimeFactory.hpp"
#include "Log/Log.hpp"
#include "Log/LogFactory.hpp"
#include "File/FilePath.hpp"
#include <string>

int main(_In_ int argc, _In_ const char* argv[])
{
	using namespace Eternal::CommandLine;
	using namespace Eternal::Commands;
	using namespace Eternal::Time;
	using namespace Eternal::LogSystem;
	using namespace Eternal::FileSystem;

	CommandLineParser CommandLineArguments(argc, argv);

	Timer* TimeSystem = CreateTimer(TimeType::TIME_TYPE_DEFAULT);
	Log* ConsoleLog = CreateLog(LogType::LOG_TYPE_CONSOLE, "Eternal");
	Log::Initialize(ConsoleLog);

	FilePath::Register("..\\eternal-engine-shaders\\Shaders\\",	FileType::FILE_TYPE_SHADERS);
	FilePath::Register("..\\eternal-engine-shaders\\",			FileType::FILE_TYPE_SHADERS);
	FilePath::Register("..\\assets\\shaders_pdb\\",				FileType::FILE_TYPE_SHADERS_PDB);
	FilePath::Register("..\\assets\\scenes\\",					FileType::FILE_TYPE_LEVELS);
	FilePath::Register("..\\assets\\textures\\",				FileType::FILE_TYPE_TEXTURES);
	FilePath::Register("..\\assets\\fbx\\",						FileType::FILE_TYPE_MESHES);
	FilePath::Register("..\\assets\\fbx\\cache\\",				FileType::FILE_TYPE_CACHED_MESHES);
	FilePath::Register("..\\assets\\pipelines\\",				FileType::FILE_TYPE_CACHED_PIPELINES);

	if (CommandLineArguments.ContainsCommandLine("bake"))
	{
		BakeCommands::Launch(CommandLineArguments);
	}

	return 0;
}
