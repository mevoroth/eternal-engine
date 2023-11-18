#pragma once

namespace Eternal
{
	namespace CommandLine
	{
		class CommandLineParser;
	}

	namespace Commands
	{
		using namespace Eternal::CommandLine;

		class BakeCommands
		{
		public:

			static void Launch(_In_ const CommandLineParser& InCommandLineParser);

			void Execute();

		private:

			BakeCommands(_In_ const CommandLineParser& InCommandLineParser);

			void BakeShaders();

			const CommandLineParser& _CommandLineParser;

		};
	}
}
