using Sharpmake;

[module: Sharpmake.Include(@"..\eternal-engine\eternal-engine-project.sharpmake.cs")]

namespace EternalEngine
{
	[Sharpmake.Generate]
	public class EternalEngineProject : EternalEngineBaseProject
	{
		public EternalEngineProject()
			: base(
				new EternalEngineProjectSettings(
					EternalEngineProjectSettingsFlags.EEPSF_IncludeSettingsHeader |
					EternalEngineProjectSettingsFlags.EEPSF_IncludeHLSLReflection
				)
			)
		{
			Name = "eternal-engine";
		}

		public override void ConfigureAll(Configuration InConfiguration, Target InTarget)
		{
			base.ConfigureAll(InConfiguration, InTarget);

			InConfiguration.Output = Configuration.OutputType.Exe;

			// Include paths
			InConfiguration.IncludePaths.AddRange(new string[] {
				@"$(ProjectDir)include",
				@"$(SolutionDir)eternal-engine-core\include",
				@"$(SolutionDir)eternal-engine-extern\imgui",
				@"$(SolutionDir)eternal-engine-graphics\include",
				@"$(SolutionDir)eternal-engine-utils\include",
			});

			// Libraries
			InConfiguration.LibraryFiles.AddRange(new string[] {
				"eternal-engine-core.lib",
				"eternal-engine-extern.lib",
				"eternal-engine-graphics.lib",
				"eternal-engine-utils.lib",
			});

			InConfiguration.AddPublicDependency<EternalEngineCoreProject>(InTarget);
			InConfiguration.AddPublicDependency<EternalEngineExternProject>(InTarget);
			InConfiguration.AddPublicDependency<EternalEngineGraphicsProject>(InTarget);
			InConfiguration.AddPublicDependency<EternalEngineUtilsProject>(InTarget);
		}
	}
}
