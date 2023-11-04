using Sharpmake;

namespace EternalEngine
{
	[System.Flags]
	public enum EternalEngineProjectSettingsFlags
	{
		EEPSF_None					= 0x0,
		EEPSF_IncludeSettingsHeader	= 0x1,
		EEPSF_IncludeHLSLReflection	= 0x2,
	}

	public struct EternalEngineProjectSettings
	{
		public EternalEngineProjectSettings(EternalEngineProjectSettingsFlags InFlags)
		{
			this.Flags = InFlags;
		}

		public bool IncludeSettingsHeader()
		{
			return (Flags & EternalEngineProjectSettingsFlags.EEPSF_IncludeSettingsHeader) == EternalEngineProjectSettingsFlags.EEPSF_IncludeSettingsHeader;
		}

		public bool IncludeHLSLReflection()
		{
			return (Flags & EternalEngineProjectSettingsFlags.EEPSF_IncludeHLSLReflection) == EternalEngineProjectSettingsFlags.EEPSF_IncludeHLSLReflection;
		}

		private EternalEngineProjectSettingsFlags Flags = EternalEngineProjectSettingsFlags.EEPSF_None;
	}

	public abstract class EternalEngineProject : Project
	{
		protected static readonly string ProjectSourceRootPath = @"[project.SharpmakeCsPath]";

		static readonly string[] SpecialFilterPrefixes = new string[] {
			@"include",
			@"src",
		};

		public EternalEngineProject(EternalEngineProjectSettings InProjectSettings = new EternalEngineProjectSettings())
		{
			AddTargets(new Target(
				EternalEngineSettings.ProjectPlatforms,
				EternalEngineSettings.ProjectDevEnvs,
				EternalEngineSettings.ProjectOptimizations
			));

			SourceRootPath = ProjectSourceRootPath;
		}

		public EternalEngineProject(
			string InModule,
			EternalEngineProjectSettings InProjectSettings = new EternalEngineProjectSettings()
		)
		{
			Module = InModule;
			ProjectSettings = InProjectSettings;

			Name = "eternal-engine-" + InModule;

			AddTargets(new Target(
				EternalEngineSettings.ProjectPlatforms,
				EternalEngineSettings.ProjectDevEnvs,
				EternalEngineSettings.ProjectOptimizations
			));

			SourceRootPath = ProjectSourceRootPath;
		}

		[Configure]
		public virtual void ConfigureAll(Configuration InConfiguration, Target InTarget)
		{
			InConfiguration.Output = Configuration.OutputType.Lib;
			InConfiguration.ProjectFileName = "[project.Name]_[target.DevEnv]";
			InConfiguration.ProjectPath = ProjectSourceRootPath;

			// Options
			InConfiguration.Options.Add(Options.Vc.General.TreatWarningsAsErrors.Enable);
			InConfiguration.Options.Add(Options.Vc.Compiler.Exceptions.Enable);
			InConfiguration.Options.Add(Options.Vc.Compiler.RTTI.Enable);
			InConfiguration.Options.Add(Options.Vc.Compiler.FloatingPointModel.Strict);
			InConfiguration.Options.Add(new Options.Vc.Compiler.DisableSpecificWarnings(new string[] {
				"4065",
				"4100",
				"4127",
				"4189",
			}));

			// Include paths
			InConfiguration.IncludePaths.Add(new string[] {
				@"[conf.ProjectPath]\include",
				@"$(SolutionDir)eternal-engine-utils\include",
				@"$(SolutionDir)eternal-engine-extern\optick\src",
			});

			// Forced includes
			InConfiguration.ForcedIncludes.Add(new string[] {
				"sal.h",
				"optick.h",
			});

			if (ProjectSettings.IncludeHLSLReflection())
			{
				InConfiguration.ForcedIncludes.Add("Types/HLSLReflection.hpp");
			}

			if (ProjectSettings.IncludeSettingsHeader())
			{
				InConfiguration.ForcedIncludes.Add(Module[0].ToString().ToUpper() + Module.Substring(1) + "Settings.hpp");
			}

			// Defines
			InConfiguration.Defines.Add(new string[] {
				"ETERNAL_USE_DXMATH_TYPES=1",
				"IMGUI_USER_CONFIG=\"Imgui/ImguiConfig.hpp\"",
				"ETERNAL_USE_STD_PARALLEL=1",
				"ETERNAL_PROFILER_USE_VERBOSE=0",
				"NOMINMAX=1",
				"ETERNAL_USE_REVERSED_Z=1",
				"_MBCS",
			});

			if (InTarget.Optimization == Optimization.Debug)
			{
				InConfiguration.Options.Add(Options.Vc.Compiler.EnableAsan.Enable);
				InConfiguration.Options.Add(Options.Vc.CodeAnalysis.RunCodeAnalysis.Enable);
				InConfiguration.Options.Add(Options.Vc.CodeAnalysis.MicrosoftCodeAnalysis.Enable);

				InConfiguration.Defines.Add(new string[] {
					"ETERNAL_DEBUG=1",
				});
			}
		}

		public override bool ResolveFilterPath(string InRelativePath, out string OutFilterPath)
		{
			return InternalResolveFilterPath(InRelativePath, SpecialFilterPrefixes, out OutFilterPath);
		}

		protected bool InternalResolveFilterPath(string InRelativePath, string[] InSpecialFilterPrefixes, out string OutFilterPath)
		{
			for (int SpecialFilterPrefixIndex = 0; SpecialFilterPrefixIndex < InSpecialFilterPrefixes.Length; SpecialFilterPrefixIndex++)
			{
				if (InRelativePath.StartsWith(InSpecialFilterPrefixes[SpecialFilterPrefixIndex]))
				{
					string RemovedPrefix = InRelativePath.StartsWith(InSpecialFilterPrefixes[SpecialFilterPrefixIndex] + "\\") ? InSpecialFilterPrefixes[SpecialFilterPrefixIndex] + "\\" : InSpecialFilterPrefixes[SpecialFilterPrefixIndex];
					OutFilterPath = InRelativePath.Replace(RemovedPrefix, "");
					return true;
				}
			}

			OutFilterPath = null;
			return false;
		}

		private EternalEngineProjectSettings ProjectSettings;
		private string Module;
	}
}
