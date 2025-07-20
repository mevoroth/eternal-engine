using Sharpmake;
using System.Reflection;
using System.Xml.Linq;

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

	public class EternalEngineBaseProjectUtils
	{
		protected static readonly string ProjectSourceRootPath = @"[project.SharpmakeCsPath]";

		public static readonly string[] SpecialFilterPrefixes = new string[] {
			@"include",
			@"src",
			@"CorePrivate\include",
			@"CorePrivate\src",
			@"GraphicsPrivate\include",
			@"GraphicsPrivate\src",
			@"UtilsPrivate\include",
			@"UtilsPrivate\src",
		};

		public static string AppendLibraryExtension(Platform InPlatform, string InLibraryName)
		{
			return InLibraryName + (ExtensionMethods.IsPC(InPlatform) ? ".lib" : ".a");
		}

		public static void Construct(Project InProject, System.Type InTargetType, EternalEngineProjectSettings InProjectSettings)
		{
			if (InTargetType == typeof(Target))
			{
				InProject.AddTargets(new Target(
					EternalEngineSettings.ProjectPlatforms,
					EternalEngineSettings.ProjectDevEnvs,
					EternalEngineSettings.ProjectOptimizations
				));
			}

			if (InTargetType == typeof(AndroidTarget))
			{
				InProject.AddTargets(new AndroidTarget(
					Platform.android,
					EternalEngineSettings.ProjectDevEnvs,
					EternalEngineSettings.ProjectOptimizations,
					Blob.NoBlob,
					BuildSystem.MSBuild,
					Android.AndroidBuildType.Gradle
				));
			}

			InProject.SourceRootPath = ProjectSourceRootPath;
		}

		public static void Construct(Project InProject, System.Type InTargetType, string InModule, EternalEngineProjectSettings InProjectSettings)
		{
			InProject.Name = "eternal-engine-" + InModule;

			if (InTargetType == typeof(Target))
			{
				InProject.AddTargets(new Target(
					EternalEngineSettings.ProjectPlatforms,
					EternalEngineSettings.ProjectDevEnvs,
					EternalEngineSettings.ProjectOptimizations
				));
			}

			if (InTargetType == typeof(AndroidTarget))
			{
				InProject.AddTargets(new AndroidTarget(
					Platform.android,
					EternalEngineSettings.ProjectDevEnvs,
					EternalEngineSettings.ProjectOptimizations,
					Blob.NoBlob,
					BuildSystem.MSBuild,
					Android.AndroidBuildType.Gradle
				));
			}

			InProject.SourceRootPath = ProjectSourceRootPath;
		}

		public static void ConfigureAll(Project.Configuration InConfiguration, ITarget InTarget, EternalEngineProjectSettings InProjectSettings, string InModule)
		{
			if (InTarget.GetFragment<Platform>() == Platform.prospero)
			{
				InConfiguration.TargetFileFullExtension = ".a";
			}

			InConfiguration.Output = Project.Configuration.OutputType.Lib;
			InConfiguration.ProjectFileName = "[project.Name]_[target.DevEnv]";
			InConfiguration.ProjectPath = ProjectSourceRootPath;

			// Options
			InConfiguration.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);
			InConfiguration.Options.Add(new Options.Vc.Linker.StackSize(8388608));
			InConfiguration.Options.Add(Options.Vc.General.WarningLevel.Level4);
			//InConfiguration.Options.Add(Options.Vc.General.TreatWarningsAsErrors.Enable);
			InConfiguration.Options.Add(Options.Vc.Compiler.Exceptions.Enable);
			InConfiguration.Options.Add(Options.Vc.Compiler.MultiProcessorCompilation.Enable);
			InConfiguration.Options.Add(Options.Vc.Compiler.RTTI.Enable);
			InConfiguration.Options.Add(Options.Vc.Compiler.FloatingPointModel.Strict);
			InConfiguration.Options.Add(Options.Vc.Compiler.DisableLanguageExtensions.Disable);
			InConfiguration.Options.AddRange(new Options.Vc.Compiler.DisableSpecificWarnings(new string[] {
				"4065",
				"4100",
				"4127",
				"4189",
			}));

			// Include paths
			InConfiguration.IncludePaths.AddRange(new string[] {
				@"[conf.ProjectPath]\include",
				@"$(SolutionDir)eternal-engine-utils\include",
				@"$(SolutionDir)eternal-engine-utils\UtilsPrivate\include",
				@"$(SolutionDir)eternal-engine-extern\optick\src",
			});

			// Forced includes
			InConfiguration.ForcedIncludes.AddRange(new string[] {
				"optick.h",
				"Macros/Macros.hpp",
				"cstdint",
				"vector"
			});

			if (InProjectSettings.IncludeHLSLReflection())
			{
				InConfiguration.ForcedIncludes.Add("Types/HLSLReflection.hpp");
			}

			if (InProjectSettings.IncludeSettingsHeader())
			{
				InConfiguration.ForcedIncludes.Add(InModule[0].ToString().ToUpper() + InModule.Substring(1) + "Settings.hpp");
			}

			// Defines
			InConfiguration.Defines.AddRange(new string[] {
				"ETERNAL_ENABLE_D3D12=(ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT)",
				"ETERNAL_USE_PRIVATE=(!ETERNAL_PLATFORM_WINDOWS)",
				"ETERNAL_USE_DXMATH_TYPES=(ETERNAL_PLATFORM_WINDOWS)",
				"IMGUI_USER_CONFIG=\"Imgui/ImguiConfig.hpp\"",
				"ETERNAL_USE_STD_PARALLEL=1",
				"ETERNAL_PROFILER_USE_VERBOSE=0",
				"NOMINMAX=1",
				"ETERNAL_USE_REVERSED_Z=1",
				"_MBCS",
				"_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
				"VC_EXTRALEAN=1",
				"WIN32_LEAN_AND_MEAN=1",
				"WIN32_EXTRA_LEAN=1",
				"NOGDICAPMASKS=1",
				//"NOVIRTUALKEYCODES=1",
				//"NOWINMESSAGES=1",
				//"NOWINSTYLES=1",
				"NOSYSMETRICS=1",
				"NOMENUS=1",
				"NOICONS=1",
				"NOKEYSTATES=1",
				"NOSYSCOMMANDS=1",
				"NORASTEROPS=1",
				//"NOSHOWWINDOW=1",
				"OEMRESOURCE=1",
				"NOATOM=1",
				//"NOCLIPBOARD=1",
				//"NOCOLOR=1",
				//"NOCTLMGR=1",
				"NODRAWTEXT=1",
				//"NOGDI=1",
				"NOKERNEL=1",
				//"NOUSER=1",
				//"NONLS=1",
				//"NOMB=1",
				"NOMEMMGR=1",
				"NOMETAFILE=1",
				"NOMINMAX=1",
				//"NOMSG=1",
				"NOOPENFILE=1",
				"NOSCROLL=1",
				"NOSERVICE=1",
				"NOSOUND=1",
				"NOTEXTMETRIC=1",
				"NOWH=1",
				"NOWINOFFSETS=1",
				"NOCOMM=1",
				"NOKANJI=1",
				"NOHELP=1",
				"NOPROFILER=1",
				"NODEFERWINDOWPOS=1",
				"NOMCX=1",
				"_HAS_STD_BYTE=0",
				"USE_OPTICK=" + (ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()) ? "0" : "0"),
				"ETERNAL_DEBUG=" + (InTarget.GetFragment<Optimization>() == Optimization.Debug ? "1" : "0"),
				"ETERNAL_USE_NVIDIA_AFTERMATH=(ETERNAL_DEBUG &amp;&amp; 0)",
				"ETERNAL_USE_DEBUG_LAYER=(ETERNAL_DEBUG &amp;&amp; !ETERNAL_USE_NVIDIA_AFTERMATH &amp;&amp; ETERNAL_PLATFORM_WINDOWS)",
				"ETERNAL_USE_PIX=(ETERNAL_DEBUG &amp;&amp; !ETERNAL_USE_NVIDIA_AFTERMATH &amp;&amp; USE_PIX &amp;&amp; (ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT))",
			});

			InConfiguration.Defines.AddRange(new string[] {
				"ETERNAL_PLATFORM_WINDOWS=" + (ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()) ? "1" : "0"),
				"ETERNAL_PLATFORM_PROSPERO=" + ((InTarget.GetFragment<Platform>() == Platform.prospero) ? "1" : "0"),
				"ETERNAL_PLATFORM_SCARLETT=" + ((InTarget.GetFragment<Platform>() == Platform.scarlett) ? "1" : "0"),
			});

			if (InTarget.GetFragment<Optimization>() == Optimization.Debug)
			{
				//InConfiguration.Options.Add(Options.Vc.Compiler.EnableAsan.Enable);
				InConfiguration.Options.Add(Options.Vc.CodeAnalysis.RunCodeAnalysis.Enable);
				InConfiguration.Options.Add(Options.Vc.CodeAnalysis.MicrosoftCodeAnalysis.Enable);
				InConfiguration.Options.Add(Options.Vc.Linker.GenerateDebugInformation.Enable);
				InConfiguration.Options.Add(Options.Clang.Compiler.GenerateDebugInformation.Enable);

				if (InTarget.GetFragment<Platform>() == Platform.prospero)
				{
					InConfiguration.AdditionalCompilerOptions.AddRange(new string[] {
						"-g"
					});
				}
			}
			else
			{
				InConfiguration.Options.Add(Options.Vc.Compiler.EnableAsan.Disable);
				InConfiguration.Options.Add(Options.Vc.CodeAnalysis.RunCodeAnalysis.Disable);
				InConfiguration.Options.Add(Options.Vc.CodeAnalysis.MicrosoftCodeAnalysis.Disable);
				InConfiguration.Options.Add(Options.Vc.Linker.GenerateDebugInformation.Disable);
				InConfiguration.Options.Add(Options.Clang.Compiler.GenerateDebugInformation.Disable);
			}

			if (ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()))
			{
				InConfiguration.ForcedIncludes.AddRange(new string[] {
					"sal.h",
				});
			}

			if (InTarget.GetFragment<Platform>() == Platform.prospero)
			{
				InConfiguration.Defines.AddRange(new string[] {
					"_In_=",
					"_Out_=",
					"_Inout_=",
					"__MACH__=1",
				});
			}

			if (InTarget.GetFragment<Platform>() == Platform.scarlett)
			{
				InConfiguration.Defines.AddRange(new string[] {
					"IMGUI_DISABLE_WIN32_FUNCTIONS=1"
				});
			}
		}

		public static bool InternalResolveFilterPath(string InRelativePath, string[] InSpecialFilterPrefixes, out string OutFilterPath)
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
	}

	public class EternalEngineBaseExecutableProjectUtils
	{
		public static void ConfigureAll(Project.Configuration InConfiguration, ITarget InTarget, string InModule, System.Type InTargetType)
		{
			InConfiguration.Output = Project.Configuration.OutputType.Exe;
			InConfiguration.TargetFileFullExtension = null;

			// Include paths
			InConfiguration.IncludePaths.AddRange(new string[] {
				@"$(SolutionDir)" + InModule,
				@"$(SolutionDir)eternal-engine-core\include",
				@"$(SolutionDir)eternal-engine-graphics\include",
				@"$(SolutionDir)eternal-engine-components\include",
				@"$(SolutionDir)eternal-engine-extern\include",
				@"$(SolutionDir)eternal-engine-extern\imgui",
				@"$(SolutionDir)eternal-engine-shaders"
			});

			if (!ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()))
			{
				InConfiguration.IncludePaths.AddRange(new string[] {
					@"$(SolutionDir)" + InModule,
					@"$(SolutionDir)eternal-engine-core\CorePrivate\include",
					@"$(SolutionDir)eternal-engine-graphics\GraphicsPrivate\include",
				});
			}

			InConfiguration.ForcedIncludes.AddRange(new string[] {
				InModule + ".hpp",
				@"Types/HLSLReflection.hpp",
			});

			// Libraries
			if (ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()))
			{
				InConfiguration.LibraryFiles.AddRange(new string[] {
					"Xinput9_1_0.lib",
					"Shlwapi.lib",
					"d3d12.lib",
					"DXGI.lib",
					"dxguid.lib",
					"libfbxsdk.lib",
					"kernel32.lib",
					"user32.lib",
					"gdi32.lib",
					"winspool.lib",
					"comdlg32.lib",
					"advapi32.lib",
					"shell32.lib",
					"ole32.lib",
					"oleaut32.lib",
					"uuid.lib",
					"odbc32.lib",
					"odbccp32.lib",
					"vulkan-1.lib",
					"shaderc_shared.lib",
					"dxcompiler.lib",
					"Xaudio2.lib",
					"Ws2_32.lib",
				});
			}

			// Defines
			InConfiguration.Defines.AddRange(new string[] {
				"SHADERC_ENABLE_SHARED_CRT=ON",
				"FBXSDK_SHARED=1",
			});

			if (ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()) && InTarget.GetFragment<Optimization>() == Optimization.Debug)
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

			if (ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()))
			{
				InConfiguration.Options.Add(Options.Vc.Linker.SubSystem.Windows);

				if (InTarget.GetFragment<Optimization>() == Optimization.Debug)
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

			if (ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()) || InTarget.GetFragment<Platform>() == Platform.scarlett)
			{
				InConfiguration.SourceFilesBuildExcludeRegex.Add(@".*\\main.cpp$");
			}
			if (InTarget.GetFragment<Platform>() == Platform.prospero || InTarget.GetFragment<Platform>() == Platform.scarlett)
			{
				InConfiguration.SourceFilesBuildExcludeRegex.Add(@".*\\WinMain.cpp");
			}
			if (ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()) || InTarget.GetFragment<Platform>() == Platform.prospero)
			{
				InConfiguration.SourceFilesBuildExcludeRegex.Add(@".*\\XSXMain.cpp");
			}

			if (InTarget.GetFragment<Platform>() == Platform.android)
			{
				InConfiguration.Options.Add(Options.Android.General.AndroidAPILevel.Android29);
			}

			if (InTargetType == typeof(Target))
			{
				InConfiguration.AddPublicDependency<EternalEngineComponentsProject>(InTarget);
				InConfiguration.AddPublicDependency<EternalEngineCoreProject>(InTarget);
				InConfiguration.AddPublicDependency<EternalEngineExternProject>(InTarget);
				InConfiguration.AddPublicDependency<EternalEngineGraphicsProject>(InTarget);
				InConfiguration.AddPrivateDependency<EternalEngineShadersProject>(InTarget);
				InConfiguration.AddPublicDependency<EternalEngineUtilsProject>(InTarget);
			}

			if (InTargetType == typeof(AndroidTarget))
			{
				InConfiguration.AddPublicDependency<EternalEngineComponentsAndroidProject>(InTarget);
				InConfiguration.AddPublicDependency<EternalEngineCoreAndroidProject>(InTarget);
				InConfiguration.AddPublicDependency<EternalEngineExternAndroidProject>(InTarget);
				InConfiguration.AddPublicDependency<EternalEngineGraphicsAndroidProject>(InTarget);
				InConfiguration.AddPrivateDependency<EternalEngineShadersAndroidProject>(InTarget);
				InConfiguration.AddPublicDependency<EternalEngineUtilsAndroidProject>(InTarget);
			}
		}
	}

	public abstract class EternalEngineBaseProject : Project
	{
		public EternalEngineBaseProject(System.Type InTargetType, EternalEngineProjectSettings InProjectSettings = new EternalEngineProjectSettings())
			: base(InTargetType)
		{
			EternalEngineBaseProjectUtils.Construct(this, InTargetType, InProjectSettings);
		}

		public EternalEngineBaseProject(
			System.Type InTargetType,
			string InModule,
			EternalEngineProjectSettings InProjectSettings = new EternalEngineProjectSettings()
		)
			: base(InTargetType)
		{
			Module = InModule;
			ProjectSettings = InProjectSettings;

			EternalEngineBaseProjectUtils.Construct(this, InTargetType, InModule, InProjectSettings);
		}

		[Configure]
		public virtual void ConfigureAll(Configuration InConfiguration, ITarget InTarget)
		{
			EternalEngineBaseProjectUtils.ConfigureAll(InConfiguration, InTarget, ProjectSettings, Module);
		}

		public override bool ResolveFilterPath(string InRelativePath, out string OutFilterPath)
		{
			return EternalEngineBaseProjectUtils.InternalResolveFilterPath(InRelativePath, EternalEngineBaseProjectUtils.SpecialFilterPrefixes, out OutFilterPath);
		}

		private EternalEngineProjectSettings ProjectSettings;
		protected string Module;
	}

	public abstract class EternalEngineBaseAndroidProject : AndroidPackageProject
	{
		public EternalEngineBaseAndroidProject(System.Type InTargetType, EternalEngineProjectSettings InProjectSettings = new EternalEngineProjectSettings())
			: base(InTargetType)
		{
			EternalEngineBaseProjectUtils.Construct(this, InTargetType, InProjectSettings);
		}

		public EternalEngineBaseAndroidProject(
			System.Type InTargetType,
			string InModule,
			EternalEngineProjectSettings InProjectSettings = new EternalEngineProjectSettings()
		)
			: base(InTargetType)
		{
			Module = InModule;
			ProjectSettings = InProjectSettings;

			EternalEngineBaseProjectUtils.Construct(this, InTargetType, InModule, InProjectSettings);
		}

		[Configure]
		public virtual void ConfigureAll(Configuration InConfiguration, ITarget InTarget)
		{
			EternalEngineBaseProjectUtils.ConfigureAll(InConfiguration, InTarget, ProjectSettings, Module);
		}

		public override bool ResolveFilterPath(string InRelativePath, out string OutFilterPath)
		{
			return EternalEngineBaseProjectUtils.InternalResolveFilterPath(InRelativePath, EternalEngineBaseProjectUtils.SpecialFilterPrefixes, out OutFilterPath);
		}

		private EternalEngineProjectSettings ProjectSettings;
		protected string Module;
	}

	public abstract class EternalEngineBaseExecutableProject : EternalEngineBaseProject
	{
		public EternalEngineBaseExecutableProject()
			: base(typeof(Target))
		{
		}

		public override void ConfigureAll(Configuration InConfiguration, ITarget InTarget)
		{
			base.ConfigureAll(InConfiguration, InTarget);
			EternalEngineBaseExecutableProjectUtils.ConfigureAll(InConfiguration, InTarget, Module, Targets.TargetType);
		}
	}

	public abstract class EternalEngineBaseAndroidExecutableProject : EternalEngineBaseAndroidProject
	{
		public EternalEngineBaseAndroidExecutableProject()
			: base(typeof(AndroidTarget))
		{
		}

		public override void ConfigureAll(Configuration InConfiguration, ITarget InTarget)
		{
			base.ConfigureAll(InConfiguration, InTarget);
			EternalEngineBaseExecutableProjectUtils.ConfigureAll(InConfiguration, InTarget, Module, Targets.TargetType);
		}
	}
}
