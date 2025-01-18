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
			InConfiguration.TargetFileFullExtension = null;

			// Include paths
			InConfiguration.IncludePaths.AddRange(new string[] {
				@"$(ProjectDir)include",
				@"$(SolutionDir)eternal-engine-core\include",
				@"$(SolutionDir)eternal-engine-extern\imgui",
				@"$(SolutionDir)eternal-engine-graphics\include",
				@"$(SolutionDir)eternal-engine-components\include",
				@"$(SolutionDir)eternal-engine-extern\optick\src",
			});

			// Libraries
			InConfiguration.LibraryFiles.AddRange(new string[] {
				"Xinput9_1_0.lib",
				"d3d12.lib",
				"DXGI.lib",
				"dxguid.lib",
				"eternal-engine-core.lib",
				"eternal-engine-components.lib",
				"eternal-engine-extern.lib",
				"eternal-engine-graphics.lib",
				"eternal-engine-utils.lib",
				"libfbxsdk.lib",
				"vulkan-1.lib",
				"shaderc_shared.lib",
				"dxcompiler.lib",
			});

			// Defines
			InConfiguration.Defines.AddRange(new string[] {
				"SHADERC_ENABLE_SHARED_CRT=ON",
				"FBXSDK_SHARED=1",
			});

			if (ExtensionMethods.IsPC(InTarget.Platform) && InTarget.Optimization == Optimization.Debug)
			{
				InConfiguration.LibraryPaths.AddRange(new string[] {
					EternalEngineSettings.VulkanPath + @"\Lib",
					EternalEngineSettings.FBXSDKPath + @"\lib\vs2022\x64\debug",
					@"$(SolutionDir)eternal-engine-extern\dxc\lib\x64"
				});

				InConfiguration.TargetCopyFiles.AddRange(new string[] {
					@"[project.SharpmakeCsPath]\..\eternal-engine-extern\dxc\bin\dxil.dll",
					EternalEngineSettings.FBXSDKPath + @"\lib\vs2022\x64\debug\libfbxsdk.dll",
				});
			}

			if (ExtensionMethods.IsPC(InTarget.Platform))
			{
				InConfiguration.BypassAdditionalDependenciesPrefix = true;
				InConfiguration.Options.Add(Options.Vc.Linker.SubSystem.Console);

				if (InTarget.Optimization == Optimization.Debug)
				{
					InConfiguration.IncludePaths.AddRange(new string[] {
						@"$(SolutionDir)packages\Microsoft.Direct3D.D3D12." + EternalEngineSettings.MicrosoftDirect3DD3D12Version + @"\Include",
						@"$(SolutionDir)packages\WinPixEventRuntime." + EternalEngineSettings.WinPixEventRuntimeVersion + @"\Include",
					});
					InConfiguration.LibraryPaths.AddRange(new string[] {
						@"$(SolutionDir)packages\WinPixEventRuntime." + EternalEngineSettings.WinPixEventRuntimeVersion + @"\bin\x64",
					});
					InConfiguration.LibraryFiles.AddRange(new string[] {
						"WinPixEventRuntime.lib"
					});
					InConfiguration.ReferencesByNuGetPackage.Add("WinPixEventRuntime", EternalEngineSettings.WinPixEventRuntimeVersion);
					InConfiguration.ReferencesByNuGetPackage.Add("Microsoft.Direct3D.D3D12", EternalEngineSettings.MicrosoftDirect3DD3D12Version);

					InConfiguration.TargetCopyFiles.AddRange(new string[] {
						@"[project.SharpmakeCsPath]\..\packages\WinPixEventRuntime." + EternalEngineSettings.WinPixEventRuntimeVersion + @"\bin\x64\WinPixEventRuntime.dll"
					});
				}
			}

			InConfiguration.AddPublicDependency<EternalEngineComponentsProject>(InTarget);
			InConfiguration.AddPublicDependency<EternalEngineCoreProject>(InTarget);
			InConfiguration.AddPublicDependency<EternalEngineExternProject>(InTarget);
			InConfiguration.AddPublicDependency<EternalEngineGraphicsProject>(InTarget);
			InConfiguration.AddPublicDependency<EternalEngineUtilsProject>(InTarget);
		}
	}

	[Sharpmake.Generate]
	public class EternalEngineSolution : Sharpmake.Solution
	{
		public EternalEngineSolution()
		{
			Name = "EternalEngine";

			AddTargets(new Target(
				Platform.win32 | Platform.win64,
				EternalEngineSettings.ProjectDevEnvs,
				EternalEngineSettings.ProjectOptimizations
			));
		}

		[Configure()]
		public void ConfigureAll(Configuration InConfiguration, Target InTarget)
		{
			InConfiguration.SolutionFileName = "[solution.Name]_[target.DevEnv]";
			InConfiguration.SolutionPath = @"[solution.SharpmakeCsPath]";

			InConfiguration.AddProject<EternalEngine.EternalEngineExternProject>(InTarget);
			InConfiguration.AddProject<EternalEngine.EternalEngineGraphicsProject>(InTarget);
			InConfiguration.AddProject<EternalEngine.EternalEngineUtilsProject>(InTarget);
			InConfiguration.AddProject<EternalEngine.EternalEngineCoreProject>(InTarget);
			InConfiguration.AddProject<EternalEngine.EternalEngineProject>(InTarget);
		}
	}
}
