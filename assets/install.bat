cd ..\Debug

mklink debug.ps.hlsl ..\assets\debug.ps.hlsl
mklink debug.vs.hlsl ..\assets\debug.vs.hlsl
mklink debug.gs.hlsl ..\assets\debug.gs.hlsl
mklink default.constants.hlsl ..\assets\default.constants.hlsl
mklink default.ps.hlsl ..\assets\default.ps.hlsl
mklink default.vs.hlsl ..\assets\default.vs.hlsl
mklink default.gs.hlsl ..\assets\default.gs.hlsl
mklink deferred.ps.hlsl ..\assets\deferred.ps.hlsl
mklink deferred.vs.hlsl ..\assets\deferred.vs.hlsl
mklink lighting.hlsl ..\assets\lighting.hlsl
mklink definitions.hlsl ..\assets\definitions.hlsl
mklink functions.hlsl ..\assets\functions.hlsl
mklink libfbxsdk.dll "C:\Program Files\Autodesk\FBX\FBX SDK\2015.1\lib\vs2013\x86\debug\libfbxsdk.dll"
mklink mesh.test.fbx ..\assets\mesh.test.fbx
mklink sponza.fbx ..\assets\sponza.fbx
mklink sphere.fbx ..\assets\sphere.fbx

cd ..\x64\Debug

mklink debug.ps.hlsl ..\..\assets\debug.ps.hlsl
mklink debug.vs.hlsl ..\..\assets\debug.vs.hlsl
mklink debug.gs.hlsl ..\..\assets\debug.gs.hlsl
mklink default.constants.hlsl ..\..\assets\default.constants.hlsl
mklink default.ps.hlsl ..\..\assets\default.ps.hlsl
mklink default.vs.hlsl ..\..\assets\default.vs.hlsl
mklink default.gs.hlsl ..\..\assets\default.gs.hlsl
mklink deferred.ps.hlsl ..\..\assets\deferred.ps.hlsl
mklink deferred.vs.hlsl ..\..\assets\deferred.vs.hlsl
mklink lighting.hlsl ..\..\assets\lighting.hlsl
mklink definitions.hlsl ..\..\assets\definitions.hlsl
mklink functions.hlsl ..\..\assets\functions.hlsl
mklink libfbxsdk.dll "C:\Program Files\Autodesk\FBX\FBX SDK\2015.1\lib\vs2013\x64\debug\libfbxsdk.dll"
mklink mesh.test.fbx ..\..\assets\mesh.test.fbx
mklink sponza.fbx ..\..\assets\sponza.fbx
mklink sphere.fbx ..\..\assets\sphere.fbx

pause
